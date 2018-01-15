import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4

Popup {
    id: popupConfirmTransfer
    x: (parent.width - width)/2
    y: (parent.height - height)/4
    focus: true
    modal: true
    closePolicy: Popup.NoAutoClose
    width: 400
    height: 400
    topPadding: 30
    rightPadding: 30
    leftPadding: 30
    bottomPadding: 30

    Rectangle {
        height: 400
        width: 400
        anchors.centerIn: parent
        color: mainViewBorderColor
        ColumnLayout {
            id: appOptionColumn
            anchors.centerIn: parent

            Label {
                text: "SỐ TIỀN"
                Layout.alignment: Qt.AlignHCenter
                font.bold: true
                font.pixelSize: 16
                color: "#4A4A4A"
            }

            Row {
                Layout.alignment: Qt.AlignHCenter
                Label {
                    id: txtAmount
                    text: g_total_amount
                    Layout.alignment: Qt.AlignHCenter
                    font.bold: true
                    color: "#F26526"
                    font.pixelSize: 24
                    Connections {
                        target: mainController
                        onView_tatal_money: {
                            console.log('====onView_tatal_money====')
                            txtAmount.text = mainController.formatMoney(total_amount)
                        }
                    }
                }
                Label {
                    text: " VNĐ"
                    Layout.alignment: Qt.AlignHCenter
                    font.bold: true
                    color: "#F26526"
                    font.pixelSize: 24
                }
            }

            Image {
                fillMode: Image.Pad
                Layout.alignment: Qt.AlignHCenter
                source: g_zalopay_avatar
            }
            Row {
                Rectangle {
                    height: 10
                    width: 1
                }
            }

            Label {
                text: g_zalopay_name
                Layout.alignment: Qt.AlignHCenter
                font.bold: true
                font.pixelSize: 16
            }

            Label {
                text: "(" + g_phone_number_or_zalopayid + ")"
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 16
            }


            Row {
                Layout.alignment: Qt.AlignHCenter
                Text {
                    id: txtZaloPayMsg
                    width: 210
                    font.bold: true
                    font.pixelSize: fntsize + 2
                    color: "red"
                    horizontalAlignment: TextField.AlignHCenter
                    text: g_ZaloPayMsg
                }
            }

            Row {                
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Button {
                    id: okButton
                    text: "ĐỒNG Ý"
                    width: 150
                    height: 60
                    highlighted: true
                    font.pixelSize: 16
                    Material.primary: "#ef8422"
                    Material.accent: "#ef8422"
                    onClicked: {
                        g_total_amount = mainController.getCurrMoney();
                        console.log('g_total_amount: ' + g_total_amount);
                        var _type;
                        if(g_type_transfer === 0) {
                            _type = '3';
                            mainController.tranferMoneyByPhone(g_phone_number_or_zalopayid, g_total_amount, _type)
                        } else if(g_type_transfer === 1) {
                            _type = '0';
                            mainController.tranferMoneyByZaloPayId(g_phone_number_or_zalopayid, g_total_amount, _type)
                        } else {
                            //view warning
                        }
                        idPopupConfirmTransfer.close()
                        idLoadingPage.open();
                    }
                }
                Button {
                    id: cancelButton
                    text: "HUỶ"
                    width: 150
                    height: 60
                    highlighted: true
                    font.pixelSize: 16
                    Material.primary: "#4A4A4A"
                    Material.accent: "#4A4A4A"
                    onClicked: {
                        idPopupConfirmTransfer.close();
                    }
                }
            }
        }
    }
}
