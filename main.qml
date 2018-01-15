import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import MainController 1.0

ApplicationWindow {
    id: idWindows
    visible: true
    width: 720
    height: 480
    //visibility: "Maximized"
    visibility: mainController.isDebugmode() ? "Maximized" : Window.FullScreen
    flags: mainController.isDebugmode() ? Qt.Window : Qt.FramelessWindowHint | Qt.Window
    title: qsTr("GBC TEAM - MÁY NẠP TIỀN ZALOPAY")

    property string mainViewBorderColor: "white"
    property int borderWidth: (width/60)
    property int fntsize: 14
    property bool timerswitch: true
    property int openLoginform: 0
    property string appTitle: "GBC TEAM - MÁY NẠP TIỀN ZALOPAY"
    property int iTitleClick: 0

    property string g_phone_number_or_zalopayid: qsTr("")
    property string g_total_amount: qsTr("0")
    property string g_zalopay_name: qsTr("")
    property string g_zalopay_avatar: qsTr("")
    property int g_type_transfer: 1
    property string g_ZaloPayMsg: qsTr("")

    header: ToolBar {
        Rectangle {
            anchors.fill: parent
            LinearGradient {
                anchors.fill: parent
                start: Qt.point(0, 0)
                end: Qt.point(parent.width, parent.height)

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#ad3906" }
                    GradientStop { position: 1.0; color: "#ef8422" }
                }

                RowLayout {
                    anchors.fill: parent

                    FontLoader {
                        id: idFont
                        source: "qrc:/fonts/UnisectVnuBold.ttf"
                        name: "UnisectVnu"
                    }

                    ToolButton {
                        id: iconApp
                        visible: !toolButtonBack.visible
                        ToolTip.visible: pressed
                        ToolTip.text: "Logo VPOS"
                        ToolTip.timeout: 3000
                        contentItem: Image {
                            fillMode: Image.Pad
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            source: "qrc:/image/vpos_logo.png"
                            scale: 0.7
                        }
                    }

                    ToolButton {
                        id: toolButtonBack
                        visible: false
                        contentItem: Image {
                            fillMode: Image.Pad
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            source: "qrc:/image/imgback.svg"
                        }
                        onClicked: {
                            stackView.pop()
                        }
                    }

                    Label {
                        id: titleLabel
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        width: contentWidth
                        height: parent.height
                        color: "white"
                        font.bold: true
                        font.pixelSize: 17
                        text: appTitle

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                iTitleClick = iTitleClick + 1;
                                if( iTitleClick ==  3)
                                {
                                    stackView.push(idPageArea)
                                    iTitleClick = 0;
                                }
                            }
                        }
                    }

                    ToolButton {
                        id: toolButtonBle
                        ToolTip.visible: pressed
                        ToolTip.text: "Trạng thái kết nối BLE"
                        ToolTip.timeout: 3000
                        anchors.right: parent.right
                        anchors.rightMargin: 35
                        contentItem: Image {
                            id: imageBleStatus
                            fillMode: Image.Pad
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            source: "qrc:/icons/bluetooth_disabled.png"
                            scale: 0.9
                            Connections {
                                target: mainController
                                onDevice_connect: {
                                    console.log("===ble status===: " + _status);
                                    imageBleStatus.updateStatus(_status)
                                }
                            }

                            function updateStatus(_status) {
                                if (_status) {
                                    source = "qrc:/icons/bluetooth.png"
                                    timerBLE.running = false;
                                } else {
                                    source = "qrc:/icons/bluetooth_disabled.png"
                                    timerBLE.running = true;
                                }
                            }
                        }
                    }

                    ToolButton {
                        id: toolButtonCloud
                        ToolTip.visible: pressed
                        ToolTip.text: "Trạng thái kết nối đến máy chủ VPOS"
                        ToolTip.timeout: 3000

                        contentItem: Image {
                            id: imageCloudStatus
                            fillMode: Image.Pad
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
//                            source: "qrc:/icons/ic_cloud_queue_white_24px.svg"
                            source: "qrc:/icons/ic_cloud_done_white_24px.svg"
                            scale: 0.9
//                            Connections {
//                                target: mainController
//                                onCloudConnectionChange: {
//                                    imageBleStatus.updateStatus(connected)
//                                }
//                            }

                            function updateStatus(connected) {
                                if (connected) {
                                    source = "qrc:/icons/ic_cloud_done_white_24px.svg"
                                } else {
                                    source = "qrc:/icons/ic_cloud_off_white_24px.svg"
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    MainController {
        id: mainController
    }

    AreaPage {
        id: idPageArea
    }

    RemoteCtrller {
        id: idPageRemoteCtrller
    }

    PopupConfirmTransfer {
        id: idPopupConfirmTransfer
    }

    LoadingPage {
        id: idLoadingPage
    }

    PopupFinish {
        id: idPopupFinish
    }

    BlankPage {
        // KHÔNG ĐƯỢC XÓA BỎ DÒNG CODE NÀY
        // TRANG FIX LỖI AreaPage, Lightonareas page tác động bởi mouse click ...
        id: iPageBlank
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: MainView {
            onVisibleChanged: {
                toolButtonBack.visible = !visible
            }
        }
    }

    Timer  {
        id: timerBLE
        interval: 5000;
        running: true;
        repeat: true;
        onTriggered: {
            var pong_count = mainController.get_pong_rcv();
            if(pong_count === false) {
                mainController.connect_to_device();
            }
        }
    }
}
