import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as QtControl14
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4

Popup {
    // show bảng báo nhận hóa đơn
    id: getBillAlert
    modal: true
    focus: true
    x: (parent.width - width) / 2
    y: Math.abs(parent.height - getBillAlert.height) / 3
    closePolicy: Popup.NoAutoClose

    Column {
        id: billAlertColumn
        width: 350
        height: implicitHeight
        spacing: 10

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            topPadding: 20
            bottomPadding: 20
            width: 350
            height: implicitHeight

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: fntsize + 4
                text: "NẠP TIỀN THÀNH CÔNG"
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                width: childrenRect.width + 2
                height: childrenRect.height + 2
                Image {
                    x: 1
                    y: 1
                    source: "qrc:image/billonprinter.png"
                }
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Vui lòng kiểm tra tài khoản ZaloPay"
                font.pixelSize: fntsize + 8
            }

            Column {
                anchors.horizontalCenter: parent.horizontalCenter
                width: implicitWidth
                spacing: 5
                Label {
                    id: labelTimerBillAlert
                    text: "Thời gian còn lại:   "
                    font.pixelSize: fntsize
                }

                QrTimer {
                    id: getBillTimer
                    anchors.left: labelTimerBillAlert.right
                    anchors.verticalCenter: parent.verticalCenter
                    delayTimer:5000  //1000 = 1s
                }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                width: billAlertColumn.width
                highlighted: true
                height: 60
                Material.primary: "#ef8422"
                Material.accent: "#ef8422"
                font.pixelSize: fntsize + 2
                text: "Đóng"
                onClicked: {
                    getBillAlert.close()
                }
            }
        }
    }

    onOpened: {
        getBillTimer.parentId = 1;
        getBillTimer.restartTimerCounter();
    }

    onClosed: {
        getBillTimer.stopTimerCounter();
    }
}
