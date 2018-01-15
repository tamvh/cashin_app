package com.wordpress.ebc81.esc_pos_android;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.wordpress.ebc81.esc_pos_lib.ESC_POS_EPSON_ANDROID;
import com.wordpress.ebc81.esc_pos_lib.USBPort;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.security.auth.callback.Callback;

/**
 * Created by haint3 on 04/06/2016.
 */
public class ReceiptESCPrinter {
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
    private static final String TAG = "ReceiptESCPrinter";
    private static Gson gson = new Gson();

    private static final int    VENDOR_ID_EXCLUSE   = 4292;
    private static final int    VENDOR_ID_1027      = 1027;     //
    private static final int    PRODUCT_ID_1027     = 24577;    //

    private static final int    VENDOR_ID_1046      = 1046;     //xprinter
    private static final int    PRODUCT_ID_1046     = 20497;    //xprinter

    private static final int    VENDORID_49745      = 49745;    //pos-58b
    private static final int    PRODCUT_ID_49745    = 4869;     //pos-58b

    private Activity mainContext = null;
    private PendingIntent mPermissionIntent;
    private UsbManager mUsbManager = null;
    private USBPort mUsbPort = null;
    private ESC_POS_EPSON_ANDROID mEscPos = null;
    private String _connectDataPrint = "";

    private int _vendorId = VENDOR_ID_1046; //default
    private int _productId = PRODUCT_ID_1046;

    public ReceiptESCPrinter() {}

    public void showToast(final String toast)
    {
        if (mainContext != null) {
            mainContext.runOnUiThread(new Runnable() {
                public void run() {
                    Toast.makeText(mainContext, toast, Toast.LENGTH_SHORT).show();
                }
            });
        }
    }

    private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver()
    {
        public void onReceive(Context context, Intent intent)
        {
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action))
            {
                synchronized (this)
                {
                    UsbDevice device = (UsbDevice) intent
                            .getParcelableExtra(UsbManager.EXTRA_DEVICE);
                    if (device != null && device.getVendorId() == _vendorId && device.getProductId() == _productId) {
                        if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                            showToast("Receipt BroadcastReceiver: Permission granted");
                            connectToDevice();
                        } else {
                            showToast("Receipt BroadcastReceiver: Permission was not granted");
                        }
                    }
                }
            }

            if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action))
            {
                UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                if (device != null && device.getVendorId() == _vendorId && device.getProductId() == _productId)
                {
                    // call your method that cleans up and closes communication with the device
                    showToast("Receipt BroadcastReceiver: ACTION_USB_DEVICE_ATTACHED");
                    mUsbPort.SetUSBConnectionFlag(false);
                }
            }

            if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action))
            {
                UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                if (device != null && device.getVendorId() == _vendorId && device.getProductId() == _productId)
                {
                    // call your method that cleans up and closes communication with the device
                    showToast("Receipt BroadcastReceiver: ACTION_USB_DEVICE_DETACHED");
                    mUsbPort.SetUSBConnectionFlag(false);

                }
            }
        }
    };

    public List<String> getUsbDeviceList() {
        return mUsbPort.get_usb_devices_list();
    }

    public void setVendorIdProductId(int vendorId, int productId) {
        _vendorId = vendorId;
        _productId = productId;
    }

    public void initialize(Activity activity) {

        mainContext = activity;
        mUsbManager = (UsbManager) mainContext.getSystemService(Context.USB_SERVICE);
        mUsbPort = new USBPort(mUsbManager);
        mUsbPort.setActivity(activity);
        mEscPos  = new ESC_POS_EPSON_ANDROID(mUsbPort);


        mPermissionIntent = PendingIntent.getBroadcast(mainContext, 0, new Intent(ACTION_USB_PERMISSION), 0);
        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        mainContext.registerReceiver(mUsbReceiver, filter);
        filter = new IntentFilter(mUsbManager.ACTION_USB_DEVICE_DETACHED);
        mainContext.registerReceiver(mUsbReceiver, filter);
        filter = new IntentFilter(mUsbManager.ACTION_USB_DEVICE_ATTACHED);
        mainContext.registerReceiver(mUsbReceiver, filter);
    }

    public boolean connectToDevice() {

        showToast("Receipt connectToDevice: vendorId = " + _vendorId + ", productId = " + _productId);
        UsbDevice foundDevice = mUsbPort.search_device(_vendorId, _productId);
        if ( foundDevice == null) {
            showToast("Receipt connectToDevice: return false 1");
            return false;
        }

        try {
            if ( !mUsbManager.hasPermission(foundDevice) ) {
                showToast("Receipt connectToDevice: permission deny");
                mUsbManager.requestPermission(foundDevice, mPermissionIntent);
            }

            showToast("Receipt connectToDevice: before connect to device");
            if ( mUsbPort.connect_device(foundDevice)) {
                showToast("Receipt connectToDevice: connect success");
                if (_connectDataPrint.isEmpty() == false && mEscPos != null) {
                    showToast("Receipt connectToDevice: print data");
                    executeReceiptPrint(_connectDataPrint);
                } else {
                    showToast("Receipt connectToDevice: not print data");
                }
                return true;
            }
        }
        catch ( Exception  e ) {
        }

        showToast("Receipt connectToDevice: return false 2");
        return false;
    }

    public void closeConnection() {
        try {
            if (mUsbReceiver != null) {
                mainContext.unregisterReceiver(mUsbReceiver);
            }

            if (mUsbPort != null) {
                mUsbPort.close();
            }
        } catch (InterruptedException e) {

        }
    }

    private void executeReceiptPrint(String data) {
        if (_vendorId == VENDOR_ID_1027) {
            mEscPos.print_receipt_1027(data);
        } else {
            mEscPos.print_receipt_1046(data);
        }
        _connectDataPrint = "";
    }

    public void receiptPrint(String data) {
        if (mUsbPort != null) {
            if (mUsbPort.GetUSBConnectionFlag()) {
                executeReceiptPrint(data);
            } else {
                _connectDataPrint = data;
                connectToDevice();
            }
        }
    }

    public String getDeviceStringList() {
        JsonObject jobject = new JsonObject();
        JsonArray jarr = new JsonArray();
        HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
        for (Map.Entry<String, UsbDevice> entry : deviceList.entrySet()) {
            if (entry.getValue().getVendorId() != VENDOR_ID_EXCLUSE) {
                JsonObject item = new JsonObject();
                item.addProperty("device_name", entry.getKey());
                item.addProperty("vendor_id", entry.getValue().getVendorId());
                item.addProperty("product_id", entry.getValue().getProductId());
                jarr.add(item);
            }
        }

        jobject.add("device_list", jarr);
        return gson.toJson(jobject);
    }
}
