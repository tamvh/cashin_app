import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

import MainController 1.0

Rectangle {
    id: mainView
    Connections {
        target: mainController
    }

    function defaultRectBorderColor() {
        rectZalopay.border.color = "#DDDDDD";
        rectPhone.border.color = "#DDDDDD";
    }
    Row {
        height: parent.height
        width: parent.width
        Rectangle {
            height: parent.height
            width: (parent.width*2)/5


            Rectangle {
                anchors.fill: parent
                color: "#F77711"
            }

            Item {
                anchors.fill: parent
                anchors.bottomMargin: 30

                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width*8.5/6
                    height: width
                    radius: width/2
                    color: "#F87C0F"
                }

                Rectangle {
                    id:clockStatus
                    anchors.centerIn: parent
                    width: parent.width
                    height: width
                    radius: width/2
                    color: "#F9830D"
                }

                Rectangle {
                    id: clockRound
                    anchors.centerIn: parent
                    width: parent.width*4/6
                    height: width
                    radius: width/2
                    color: "#FB8B0B"
                    ColumnLayout {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Label {
                            id: txtAmount
                            Layout.alignment: Qt.AlignHCenter
                            text: "0"
                            font.bold: true
                            font.pixelSize: 70
                            color: "white"
                            Connections {
                                target: mainController
                                onView_tatal_money: {
                                    console.log('====onView_tatal_money====')
                                    txtAmount.text = mainController.formatMoney(total_amount)
                                }
                            }
                        }
                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            text: "VNĐ"
                            font.bold: true
                            font.pixelSize: 70
                            color: "white"
                        }
                    }
                }
                // Event title
                Component {
                    id: titleComponent
                    Item {
                        y: clockRound.y + clockRound.height
                        height: clockRound.y - clockStatus.y
                        width: root.width

                        Label {
                            id: txtTitle
                            width: root.width - 40
                            text: event.title
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            font.family: montserrat.name
                            font.pixelSize: 22
                            elide: Label.ElideRight
                            wrapMode: Label.WordWrap
                            maximumLineCount: 4
                            color: "white"
                            opacity: 0.75
                        }
                    }
                }
            }
        }
        Rectangle {
            height: parent.height
            width: (parent.width*3)/5
            ColumnLayout {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10
                Row {
                    Layout.alignment: Qt.AlignHCenter
                    Text {
                        id: txtWarning
                        font.bold: true
                        font.pixelSize: fntsize + 4
                        horizontalAlignment: TextField.AlignHCenter
                        color: "red"
                    }
                }
                Row {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 30
                    Rectangle {
                        id: rectZalopay
                        width: 200
                        height: 200
                        radius: 10
                        border.color: "#F26526"
                        border.width: 5
                        ColumnLayout {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 20
                            Image {
                                fillMode: Image.Pad
                                Layout.alignment: Qt.AlignHCenter
                                source: "qrc:/image/icon_zalopay.svg"
                                scale: 1.5
                            }
                            Text {
                                text: qsTr("ZaloPay ID")
                                color: "gray"
                                font.pixelSize: 25
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                g_type_transfer = 1;
                                defaultRectBorderColor();
                                rectZalopay.border.color = "#F26526";
                                txtPhoneOrZalopayId.placeholderText = 'NHẬP ZALOPAY ID';
                            }
                        }
                    }
                    Rectangle {
                        id: rectPhone
                        width: 200
                        height: 200
                        radius: 10
                        border.color: "#DDDDDD"
                        border.width: 5
                        ColumnLayout {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 20
                            Image {
                                fillMode: Image.Pad
                                Layout.alignment: Qt.AlignHCenter
                                source: "qrc:/image/icon_phonenumber.svg"
                                scale: 1.5
                            }
                            Text {
                                text: qsTr("Số điện thoại")
                                color: "gray"
                                font.pixelSize: 25
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                g_type_transfer = 0;
                                defaultRectBorderColor();
                                rectPhone.border.color = "#F26526";
                                txtPhoneOrZalopayId.placeholderText = 'NHẬP SỐ ĐIỆN THOẠI';
                            }
                        }
                    }
                }

                Row {
                    Rectangle {
                        width: 1
                        height: 15
                    }
                }

                Row {
                    Layout.alignment: Qt.AlignHCenter
                    TextField {
                        id: txtPhoneOrZalopayId
                        width: 430
                        placeholderText: "NHẬP ZALOPAY ID"
                        font.bold: true
                        font.pixelSize: 36
                        horizontalAlignment: TextField.AlignHCenter
                        Material.primary: "#ef8422"
                        Material.accent: "#ef8422"
                    }
                }            

                Row {
                    Layout.alignment: Qt.AlignHCenter
                    Button {
                        id: okButton
                        text: "ĐỒNG Ý"
                        width: 430
                        height: 100
                        font.bold: true
                        font.pixelSize: 36
                        highlighted: true
                        Material.primary: "#F26526"
                        Material.accent: "#F26526"
                        onClicked: {
                            g_ZaloPayMsg = '';
                            g_phone_number_or_zalopayid = txtPhoneOrZalopayId.text
                            g_total_amount = mainController.getCurrMoney();
                            if(g_phone_number_or_zalopayid.trim() === '') {
                                if(g_type_transfer === 0) {
                                    txtWarning.text = "(*) Vui lòng nhập số điện thoại"
                                } else if(g_type_transfer === 1) {
                                    txtWarning.text = "(*) Vui lòng nhập Zalopay ID"
                                } else {
                                    txtWarning.text = "(*) Có lỗi xảy ra trong quá trình chuyển tiền"
                                }
                                return;
                            } else {
                                txtWarning.text = ''
                            }

                            if(mainController.getCurrMoney() <= 0) {
                                txtWarning.text = '(*) Số tiền không hợp lệ'
                                return;
                            } else {
                                txtWarning.text = ''
                            }

                            //get zalopay user info
                            if(g_type_transfer === 0) {
                                mainController.getZaloPayUserInfoByPhone(g_phone_number_or_zalopayid)
                            }
                            if(g_type_transfer === 1) {
                                mainController.getZaloPayUserInfoByZpid(g_phone_number_or_zalopayid)
                            }

                            idLoadingPage.open()
                        }
                    }
                    Connections {
                        target: mainController
                        onDoneUserInfoByPhone: {
                            idLoadingPage.close()
                            var returncode = 0;
                            var returnmessage = '';
                            var jdata = JSON.parse(data);
                            console.log('onDoneUserInfoByPhone: ' + JSON.stringify(jdata));
                            returncode = jdata.returncode;
                            returnmessage = jdata.returnmessage;
                            if(returncode === 1) {
                                g_zalopay_avatar = jdata.avatar;
                                g_zalopay_name = jdata.displayname;
                                idPopupConfirmTransfer.open();
                            } else {
                                txtWarning.text = "(*) " + returnmessage;
                            }
                        }
                        onDoneUserInfoByZpID: {
                            idLoadingPage.close()
                            var returncode = 0;
                            var returnmessage = '';
                            var jdata = JSON.parse(data);
                            console.log('onDoneUserInfoByZpID: ' + JSON.stringify(jdata));
                            returncode = jdata.returncode;
                            returnmessage = jdata.returnmessage;
                            if(returncode === 1) {
                                g_zalopay_avatar = jdata.avatar;
                                g_zalopay_name = jdata.displayname;
                                idPopupConfirmTransfer.open();
                            } else {
                                txtWarning.text = "(*) " + returnmessage;
                            }
                        }

                        onDoneTransferMoneyByPhone: {
                            idLoadingPage.close()
                            var returncode;
                            var returnmessage;
                            var jdata = JSON.parse(data);
                            console.log('onDoneTransferMoneyByPhone: ' + JSON.stringify(jdata));
                            returncode = jdata.returncode;
                            returnmessage = jdata.returnmessage;
                            if(returncode === 10) {
                                //chuyen tien thanh cong
                                mainController.resetMoney()
                                idPopupConfirmTransfer.close();
                                g_ZaloPayMsg = '';
                                txtPhoneOrZalopayId.text = ''
                                idPopupFinish.open()
                            } else {
                                //chuyen tien that bai. vui long thu lai
                                g_ZaloPayMsg = "(*) Hệ thống có lỗi. Vui lòng thử lại.";
                                idPopupConfirmTransfer.open()
                            }
                        }

                        onDoneTransferMoneyByZpID: {
                            idLoadingPage.close()
                            var returncode;
                            var returnmessage;
                            var jdata = JSON.parse(data);
                            console.log('onDoneTransferMoneyByZpID: ' + JSON.stringify(jdata));
                            returncode = jdata.returncode;
                            returnmessage = jdata.returnmessage;
                            if(returncode === 10) {
                                //chuyen tien thanh cong
                                mainController.resetMoney()
                                idPopupConfirmTransfer.close();
                                g_ZaloPayMsg = '';
                                txtPhoneOrZalopayId.text = ''
                                idPopupFinish.open()
                            } else {
                                //chuyen tien that bai. vui long thu lai
                                g_ZaloPayMsg = "(*) Hệ thống có lỗi. Vui lòng thử lại.";
                                idPopupConfirmTransfer.open()
                            }
                        }
                    }
                }


            }
        }
    }
}
