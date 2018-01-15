package com.vng.gbc;

import android.os.Bundle;
import android.util.Log;

/*
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.widget.TextView;
import android.widget.Button;
*/

import android.widget.Toast;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Looper;

import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;

import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.driver.UsbSerialProber;
import com.hoho.android.usbserial.util.HexDump;
import com.hoho.android.usbserial.util.SerialInputOutputManager;

import java.util.ArrayList;
import java.util.List;
import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.wordpress.ebc81.esc_pos_android.ReceiptESCPrinter;

import org.qtproject.qt5.android.bindings.QtActivity;

public class MyActivity extends QtActivity
{
    private final String TAG = MyActivity.class.getSimpleName();
    private static final String ACTION_USB_PERMISSION = "com.vng.gbc.vpos.activity.USB_PERMISSION";
    private static final String ACTION_USB_PERMISSION2 = "com.vng.gbc.vpos.activity.USB_PERMISSION2";

    private UsbManager mUsbManager;
    private static UsbSerialPort sPort = null;
    private boolean bConnect = false;

    private final ExecutorService mExecutor = Executors.newSingleThreadExecutor();

    private SerialInputOutputManager mSerialIoManager;

    private static native void nativeDeviceNewData(byte[] dataA);

    private final int DEVICE_READER_ID = 4292;
    private final int DEVICE_PRINTER_ID = 1027;

    ReceiptESCPrinter posPrinter;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //ReceiptESCPrinter.requestPermission(this);

        //posPrinter = new ReceiptESCPrinter();
        //posPrinter.initialize(this);
        //FindReaderPort();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        //ReceiptESCPrinter.closeConnection();

        //posPrinter.closeConnection();
        //stopIoManager();
        //unregisterReceiver(mUsbReceiver);
    }


    public void printBill(final String billDetail) {
        runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(getApplicationContext(), "Đang in hóa đơn...", Toast.LENGTH_SHORT).show();
                    posPrinter.receiptPrint(billDetail);
                }
        });
    }

    final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    public static byte[] hexStringToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
                                 + Character.digit(s.charAt(i+1), 16));
        }
        return data;
    }

    public void sendToDevice(final String billDetail) {
        final byte[] arrayData = hexStringToByteArray(billDetail);
        final String test = bytesToHex(arrayData);
        runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (mSerialIoManager != null)  {
                        Toast.makeText(getApplicationContext(), "Đang gửi thông tin tới thiết bị...", Toast.LENGTH_SHORT).show();
                        mSerialIoManager.writeAsync(arrayData);
                    }

                    Toast.makeText(getApplicationContext(), "Hex: " + test, Toast.LENGTH_SHORT).show();
                }
        });
    }

    public void stopDevice(final String stopData) {
        final byte[] arrayData = hexStringToByteArray(stopData);
        runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (mSerialIoManager != null)  {
                        Toast.makeText(getApplicationContext(), "Dừng thiết bị đọc...", Toast.LENGTH_SHORT).show();
                        mSerialIoManager.writeAsync(arrayData);
                }
            }
        });
    }

    PendingIntent mPermissionIntent;
    private BroadcastReceiver mUsbReceiver;
    class UsbReceiver extends BroadcastReceiver {
        UsbReceiver() {
        }

        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (MyActivity.ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                    if (!intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        Toast.makeText(MyActivity.this.getApplicationContext(), "Permission denied", Toast.LENGTH_SHORT).show();
                    } else if (device != null) {
                        if (false == bConnect) {
                            Toast.makeText(MyActivity.this.getApplicationContext(), "Obtain permission SUCCESS", Toast.LENGTH_SHORT).show();
                            UpdatePortFromDevice(device);
                        }
                    }
                }
            } else if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action)) {
                UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                if (device.getVendorId() == DEVICE_READER_ID) {
                    Toast.makeText(MyActivity.this.getApplicationContext(), "USB Reader ATTACHED", Toast.LENGTH_SHORT).show();
                    if (!mUsbManager.hasPermission(device)) {
                        mUsbManager.requestPermission(device, mPermissionIntent);
                        Toast.makeText(getApplicationContext(), "Found reader device => Request permission ", 0).show();
                    } else {
                        if (false == bConnect) {
                            UpdatePortFromDevice(device);
                        }
                    }
                }
            } else if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
                UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                if (device.getVendorId() == DEVICE_READER_ID) {
                    Toast.makeText(MyActivity.this.getApplicationContext(), "USB Reader DETACHED", Toast.LENGTH_SHORT).show();
                    MyActivity.this.stopIoManager();
                }
            }
        }
    }

    public void FindReaderPort() {
        mUsbManager = (UsbManager)getSystemService(Context.USB_SERVICE);
        mUsbReceiver = new UsbReceiver();

        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        getApplicationContext().registerReceiver(this.mUsbReceiver, filter);
        mPermissionIntent = PendingIntent.getBroadcast(getApplicationContext(), 0, new Intent(ACTION_USB_PERMISSION), PendingIntent.FLAG_CANCEL_CURRENT);

        registerReceiver(mUsbReceiver, filter);
        filter = new IntentFilter(UsbManager.ACTION_USB_DEVICE_DETACHED);
        registerReceiver(mUsbReceiver, filter);
        filter = new IntentFilter(UsbManager.ACTION_USB_DEVICE_ATTACHED);
        registerReceiver(mUsbReceiver, filter);

        for (final UsbDevice usbDevice : mUsbManager.getDeviceList().values()) {
            if (usbDevice.getVendorId() == DEVICE_READER_ID) {
                if (!mUsbManager.hasPermission(usbDevice)) {
                    mUsbManager.requestPermission(usbDevice, mPermissionIntent);
                    Toast.makeText(getApplicationContext(), "Found reader device => Request permission ", 0).show();
                } else {
                    UpdatePortFromDevice(usbDevice);
                    Toast.makeText(getApplicationContext(), "Found reader device => Already have permission ", 0).show();
                }
                return;
            }
        }

        Toast.makeText(getApplicationContext(), "Not fount reader device", 0).show();
    }

    public void UpdatePortFromDevice(final UsbDevice usbDevice) {
        final List<UsbSerialPort> ports = UsbSerialProber.getDefaultProber().probeDevice(usbDevice).getPorts();
        sPort = ports.get(0);
        OpenReaderDevice();
        onDeviceStateChange();
    }

    public void OpenReaderDevice() {
        if (sPort == null) {
            Toast.makeText(getApplicationContext(), "Port null", 0).show();
        } else {
            UsbDeviceConnection connection = mUsbManager.openDevice(sPort.getDriver().getDevice());
            if (connection == null) {
                Toast.makeText(getApplicationContext(), "Connection null", 0).show();
                return;
            }

            try {
                sPort.open(connection);
                sPort.setParameters(38400, 8, UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);
            } catch (IOException e) {
                Log.e(TAG, "Error setting up device: " + e.getMessage(), e);
                return;
            }

            bConnect = true;
            Toast.makeText(getApplicationContext(), "Connection success", 0).show();
        }
    }



    @Override
    protected void onResume() {
        super.onResume();
    }

    private final SerialInputOutputManager.Listener mListener =
            new SerialInputOutputManager.Listener() {

        @Override
        public void onRunError(Exception e) {
            Log.d(TAG, "Runner stopped.");
        }

        @Override
        public void onNewData(final byte[] data) {
            MyActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    //MyActivity.this.updateReceivedData(data);
                    //nativeDeviceNewData(data);
                    Toast.makeText(getApplicationContext(), "Receive data:" + new String(data), 0).show();
                }
            });
        }
    };

    public void stopIoManager() {
        if (mSerialIoManager != null) {
            Log.i(TAG, "Stopping io manager ..");
            mSerialIoManager.stop();
            mSerialIoManager = null;
            bConnect = false;
        }
    }

    public void startIoManager() {
        if (sPort != null) {
            //Log.i(TAG, "Starting io manager ..");
            Toast.makeText(getApplicationContext(), "Starting io manager ..", 0).show();
            mSerialIoManager = new SerialInputOutputManager(sPort, mListener);
            mExecutor.submit(mSerialIoManager);
        }
    }

    private void onDeviceStateChange() {
        stopIoManager();
        startIoManager();
    }

    /*
    private static final String ACTION_USB_PERMISSION = "com.vng.gbc.myacitivity.activity.USB_PERMISSION";
    UsbDevice device;
    UsbManager manager;
    PendingIntent mPermissionIntent;
    private boolean mPermissionRequestPending;
    private int mVendor_id;
    TextView mTextInfo;
    Button mButtonPrint;

    private BroadcastReceiver mUsbReceiver;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        init();
    }

    private boolean forceClaim;
    private int mBaud;
    private int mBaudFigure;
    private CharSequence[] baudString;
    private byte[] sendBytes;
    private static int TIMEOUT;

    public void init() {
        this.forceClaim = true;
        this.mPermissionRequestPending = false;
        this.mBaud = 57600;
        this.mBaudFigure = 52;
        this.mVendor_id = 1027;
        this.baudString = new CharSequence[]{"300", "600", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600"};
        this.mUsbReceiver = new C01851();

        getApplicationContext().registerReceiver(this.mUsbReceiver, new IntentFilter(ACTION_USB_PERMISSION));

        this.mPermissionIntent = PendingIntent.getBroadcast(getApplicationContext(), 0, new Intent(ACTION_USB_PERMISSION), 0);
        this.manager = (UsbManager)getSystemService("usb");

        checkInfo();
    }

    public void checkInfo2() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                checkInfo();
            }
        });
    }

    public void checkInfo() {
        String msg = null;
        for (UsbDevice usbDevice : this.manager.getDeviceList().values()) {
            this.device = usbDevice;
            if (!this.manager.hasPermission(this.device)) {
                this.manager.requestPermission(this.device, this.mPermissionIntent);
            }
            if (this.manager.hasPermission(this.device)) {
                if (this.device.getVendorId() == this.mVendor_id) {
                    msg = "\nDeviceID: " + this.device.getDeviceId() + "\n" + "DeviceName: " + this.device.getDeviceName() + "\n" + "DeviceClass: " + this.device.getDeviceClass() + " - " + "DeviceSubClass: " + this.device.getDeviceSubclass() + "\n" + "VendorID: " + this.device.getVendorId() + "\n" + "ProductID: " + this.device.getProductId() + "\n";
                    Toast.makeText(getApplicationContext(), "Connected", 0).show();
                    break;
                }
            }
            Toast.makeText(getApplicationContext(), "no Device", 0).show();
            //return;
        }
        if (msg != null) {
            //this.mTextInfo.setText(msg);
            //this.mButtonPrint.setEnabled(true);
            return;
        }
        //this.mTextInfo.setText("No device");
        Toast.makeText(getApplicationContext(), "Please, check printer.", 0).show();
    }

    public void checkPrinter2(final String str) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                checkPrinter(str);
            }
        });
    }

    public void checkPrinter(final String str) {
        Toast.makeText(getApplicationContext(), "Begin checkPrinter", 0).show();

        boolean claim;
        //this.sendBytes = (new String(new char[]{'\u001b', '@'}) + "a\nb\nc\nd\ne\nf\ng\nh\ni\nj\nk\nl" + new String(new char[]{'\u001d', 'V', '\u0001'})).getBytes();
        //this.sendBytes = (new String("Ten Hang: Ba con soi 50000\r\nTen Hang: Ba con soi 50000\r\nTen Hang: Ba con soi 50000\r\nTen Hang: Ba con soi 50000\r\nTen Hang: Ba con soi 50000\r\n") + new String(new char[]{'\u001d', 'V', '\u0001'})).getBytes();
        this.sendBytes = (str + new String(new char[]{'\u001d', 'V', '\u0001'})).getBytes();
        UsbInterface intf = this.device.getInterface(0);
        UsbEndpoint endpoint = null;
        UsbDeviceConnection connection = this.manager.openDevice(this.device);
        int result = 0;
        try {
            claim = connection.claimInterface(intf, this.forceClaim);
        } catch (IllegalArgumentException e) {
            claim = false;
        } catch (NullPointerException e2) {
            claim = false;
        }
        if (claim) {
            int i = 0;
            while (i < intf.getEndpointCount()) {
                if (intf.getEndpoint(i).getType() == 2 && intf.getEndpoint(i).getDirection() == 0) {
                    endpoint = intf.getEndpoint(i);
                }
                i++;
            }
            connection.controlTransfer(64, 3, 52, 0, null, 0, 0);
            result = connection.bulkTransfer(endpoint, this.sendBytes, this.sendBytes.length, TIMEOUT);
        } else {
            Toast.makeText(getApplicationContext(), "Can't claim interface", 0).show();
        }

        if (result > 0) {
            //this.mTextAlert.setVisibility(0);
            //this.mButtonSuccess.setEnabled(true);
            return;
        }
        Toast.makeText(getApplicationContext(), "Please, check printer.", 0).show();
        //this.mButtonPrint.setEnabled(false);
        //this.mButtonSuccess.setEnabled(false);
    }


    class C01851 extends BroadcastReceiver {
        C01851() {
        }

        public void onReceive(Context context, Intent intent) {
            if (MyActivity.ACTION_USB_PERMISSION.equals(intent.getAction())) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice) intent.getParcelableExtra("device");
                    if (!intent.getBooleanExtra("permission", false)) {
                        Toast.makeText(MyActivity.this.getApplicationContext(), "permission denied for device", 0).show();
                    } else if (device != null) {
                        Toast.makeText(MyActivity.this.getApplicationContext(), "permission", 0).show();
                        //MyActivity.this.checkPrinter();
                    }
                }
            }
        }
    }
    */
}
