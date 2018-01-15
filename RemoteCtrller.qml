import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQml.Models 2.1
//import QtQuick.Controls.Material 2.0

Component {
    Page {
        id: idRemoteCtrll

        property int itemHeight: 60
        property int btnHeight0: 70
        property int btnWidth0: 0

        property int rowHeight: 80
        property int rowpHeight: 120

        property int colWidth: ((idRemoteCtrll.width - borderWidth*2 - 40) / 3)
        property int btnWidth1: 0       // menu, input
        property int btnWidth2: 0       // reset, return
        property int btnWidth3: 0       // volumn

        property string btnColor: "red" //"#8a4ea4"
        property string bkColor: "blue" //"white"
        property real btnOpacity: 0.15

        property double lastAction;

        Connections {
            target: mainController

            onRefreshDeviceList: {
                listmodelDevs.clear();

                var count = 0;
                var JsDev = JSON.parse(data);
                for (var dev in JsDev.dt.remote_devices)
                {
                    listmodelDevs.append({
                                             devcode:JsDev.dt.remote_devices[dev].remote_device_code,
                                             devtype:JsDev.dt.remote_devices[dev].remote_type,
                                             devbutn:JsDev.dt.remote_devices[dev].remote_button_type,
                                             devname:JsDev.dt.remote_devices[dev].remote_button_name,
                                             devvali:JsDev.dt.remote_devices[dev].remote_value_num,
                                             devvals:JsDev.dt.remote_devices[dev].remote_value_string,
                                         })
                    count++;
                }
            }
        }

        function remoteKeyPress(keyname) {

            lastAction = new Date().getTime();

            for( var i=0; i<listmodelDevs.count; i++ )
            {
                if( keyname === listmodelDevs.get(i).devname)
                {
                    //console.log(listmodelDevs.get(i).devname + ", " + listmodelDevs.get(i).devcode)

                    mainController.callRemoteCtrll(listmodelDevs.get(i).devcode,
                                                   listmodelDevs.get(i).devtype,
                                                   listmodelDevs.get(i).devbutn,
                                                   listmodelDevs.get(i).devvali)
                }
            }
        }

        function createRemoteKeyBtnRow(isBtn, parentId, wi, hi, phi, btnName, dispName, iconName) {
            var compbtn = isBtn ?
                        Qt.createComponent("RemoteBtnrow.qml") :
                        Qt.createComponent("RemoteItem.qml");

            var rembtn = compbtn.createObject(parentId, {"btnId":btnName,
                                                  "displayName":dispName,
                                                  "btnIcon":iconName,
                                                  "btnWidth":wi,
                                                  "btnHeight":hi,
                                                  "parentHeight":phi});
            rembtn.clicked.connect(remoteKeyPress);

            return rembtn;
        }

        function createRemoteKeyBtnCol(isBtn, parentId, wi, hi, phi, btnName, dispName, iconName) {
            var compbtn = Qt.createComponent("RemoteBtncol.qml");
            var rembtn = compbtn.createObject(parentId, {"btnId":btnName,
                                                  "displayName":dispName,
                                                  "btnIcon":iconName,
                                                  "btnWidth":wi,
                                                  "btnHeight":hi,
                                                  "parentHeight":phi});
            rembtn.clicked.connect(remoteKeyPress);

            return rembtn;
        }

        function createRemoteKeyBtnTxt(rRound, parentId, wi, hi, phi, btnName, dispName, iconName) {
            var compbtn = Qt.createComponent("RemoteBtntext.qml");

            var rembtn = compbtn.createObject(parentId, {"btnId":btnName,
                                                  "displayName":dispName,
                                                  "btnIcon":iconName,
                                                  "btnWidth":wi,
                                                  "btnHeight":hi,
                                                  "parentHeight":phi,
                                                  "rRound":rRound});
            rembtn.clicked.connect(remoteKeyPress);

            return rembtn;
        }

        function createRemoteBlankBtn(parentId) {
            var compbtn = Qt.createComponent("RemoteBlank.qml");
            var rembtn = compbtn.createObject(parentId);
        }

        ListModel {
            id: listmodelDevs
        }

        Component {
            id: componentDevs

            MouseArea {
                id: dragArea
                anchors { left: parent.left; right: parent.right; }
                height: content.height

                Rectangle {
                    id: content
                    anchors { left: parent.left; right: parent.right }
                    width: parent.width
                    height: itemHeight
                    border.color: "lightsteelblue"
                    border.width: 1
                    radius: 5

                    Row {
                        anchors.fill:parent
                        spacing: 5
                        anchors.margins: 5
                        width: parent.width
                        height: parent.height

                        Button {
                            width: 100
                            text: " [ PRESS ] "
                            onClicked: {
                                remoteKeyPress(devname)
                            }
                        }

                        Text {
                            text: devname + ",  " + devcode + ",  " + devtype + ",  " + devbutn + ",  " + devvali
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignHCenter
                            height: parent.height
                        }
                    }
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            //anchors.margins: borderWidth

            gradient: Gradient {
                      GradientStop { position: 0.0; color: "lightsteelblue" }
                      GradientStop { position: 1.0; color: "green" }
                  }

            Flickable {
                anchors.fill: parent
                anchors.margins: borderWidth
                //contentHeight: column.height

                MouseArea {
                    // update lastAction time if touch on screen
                    anchors.fill: parent
                    onEntered: lastAction = new Date().getTime();
                }

                Column {
                    id: column
                    width: parent.width

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20

                    Column {
                        spacing: 10
                        width: parent.width

                        // menu, reset, power, apa, echo
                        RowLayout {
                            width: parent.width
                            Row {
                                spacing: 10
                                width: parent.width
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                id: idRow01
                            }
                        }

                        Row {
                            width: parent.width
                            spacing: 20
                            Column {
                                spacing: 10
                                Row {
                                    spacing: 10
                                    id: idRow11
                                }
                                Row {
                                    spacing: 10
                                    id: idRow12
                                }
                                Row {
                                    spacing: 10
                                    id: idRow13
                                }
                            }

                            Rectangle {
                                id:idRectCenter
                                radius: 10
                                border.color: "white"
                                border.width: 1
                                color: "transparent" //"#976495"
                                opacity: 0.65

                                Column {
                                    spacing: 10
                                    anchors.fill: parent
                                    RowLayout {
                                        Row {
                                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                            id: idRow21
                                        }
                                    }
                                    Row {
                                        spacing: 10
                                        id: idRow22
                                    }
                                    RowLayout {
                                        Row {
                                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                            id: idRow23
                                        }
                                    }
                                }
                            }

                            Column {
                                spacing: 10
                                Row {
                                    spacing: 10
                                    id: idRow31
                                }
                                Row {
                                    spacing: 10
                                    id: idRow32
                                }
                                Row {
                                    spacing: 10
                                    id: idRow33
                                }
                            }
                        }
                    }
                }
            }
        }

        Timer  {
            interval: 10000;
            running: true;
            repeat: true;
            onTriggered: {
                // 300 second = 5 minute
                var freeHand = new Date().getTime() - lastAction;
                if( freeHand > 300000 ) {
                    stackView.pop();
                }
            }
        }

        Component.onCompleted: {

            lastAction = new Date().getTime();

            if( idRemoteCtrll.height < (rowHeight * 5) ) {
                rowHeight = idRemoteCtrll.height / 5;
                rowpHeight = rowHeight * 1.5;
            }

            btnWidth0 = ((idRemoteCtrll.width - borderWidth*2) / 2)
            btnWidth1 = ((idRemoteCtrll.width - borderWidth*2 - 40) / 5)

            // menu, reset, power, apa, echo
            //idRow01.width = ((btnWidth1 * 5) + (idRow01.spacing * 4))
            createRemoteKeyBtnRow(true, idRow01, btnWidth1, rowHeight, rowpHeight, "MENU", "Menu",           "qrc:/image/remmenu.png");
            createRemoteKeyBtnRow(true, idRow01, btnWidth1, rowHeight, rowpHeight, "INPUT", "Input",         "qrc:/image/reminput.png");
            createRemoteKeyBtnRow(false, idRow01, btnWidth1, rowpHeight,rowpHeight,"POWER", "",              "qrc:/image/rempower.png");
            createRemoteKeyBtnTxt(10, idRow01, btnWidth1, rowHeight, rowpHeight, "APA", "APA",             "");
            createRemoteKeyBtnTxt(10, idRow01, btnWidth1, rowHeight, rowpHeight, "ECO_MODE", "Eco mode",   "");

            // reset, return ...
            colWidth = ((idRemoteCtrll.width - borderWidth*2 - 40) / 3);
            btnWidth2 = ((colWidth - 10) / 2)
            btnWidth3 = ((colWidth - 20) / 3)

            idRectCenter.width = colWidth
            idRectCenter.height = (rowHeight*3 + 20)

            createRemoteKeyBtnCol(true, idRow11, btnWidth2, rowHeight, rowHeight, "RESET", "Reset", "qrc:/image/remreset.png");
            createRemoteKeyBtnCol(true, idRow11, btnWidth2, rowHeight, rowHeight, "RETURN", "Return", "qrc:/image/remreturn.png");
            createRemoteKeyBtnTxt(10, idRow12, btnWidth2, rowHeight, rowHeight, "ASPECT", "Aspect", "");
            createRemoteKeyBtnTxt(10, idRow12, btnWidth2, rowHeight, rowHeight, "KEYSTONE", "Keystone", "");
            createRemoteKeyBtnTxt(10, idRow13, btnWidth2, rowHeight, rowHeight, "PATTERN", "Pattern", "");
            createRemoteKeyBtnCol(true, idRow13, btnWidth2, rowHeight, rowHeight, "FREEZE", "Freeze", "qrc:/image/remfreeze.png");

            // control up/down/left/right ...
            var rowHi1 = rowHeight - 10
            var rowHi2 = rowHeight + 20
            createRemoteKeyBtnRow(false, idRow21, btnWidth3, rowHi1, rowHi1, "ARROW_UP", "", "qrc:/image/remctrlup.png");
            createRemoteKeyBtnRow(false, idRow22, btnWidth3, rowHi2, rowHi2, "ARROW_LEFT", "", "qrc:/image/remctrlleft.png");
            createRemoteKeyBtnTxt(Math.min(btnWidth3, rowHi2), idRow22, btnWidth3, rowHi2, rowHi2, "ENTER", "Enter", "");
            createRemoteKeyBtnRow(false, idRow22, btnWidth3, rowHi2, rowHi2, "ARROW_RIGHT", "", "qrc:/image/remctrlright.png");
            createRemoteKeyBtnRow(false, idRow23, btnWidth3, rowHi1, rowHi1, "ARROW_DOWN", "", "qrc:/image/remctrldown.png");

            idRow21.width = btnWidth3
            idRow21.parent.width = colWidth
            idRow23.width = btnWidth3
            idRow23.parent.width = colWidth

            // volumn, zoom, mute ...
            createRemoteKeyBtnRow(true, idRow31, btnWidth2, rowHeight, rowHeight, "VOLUME-", " -", "qrc:/image/remvolume.png");
            createRemoteKeyBtnRow(true, idRow31, btnWidth2, rowHeight, rowHeight, "VOLUME+", " +", "qrc:/image/remvolume.png");
            createRemoteKeyBtnRow(true, idRow32, btnWidth2, rowHeight, rowHeight, "D_ZOOM-", " -", "qrc:/image/remzoom.png");
            createRemoteKeyBtnRow(true, idRow32, btnWidth2, rowHeight, rowHeight, "D_ZOOM+", " +", "qrc:/image/remzoom.png");
            createRemoteKeyBtnCol(true, idRow33, btnWidth2, rowHeight, rowHeight, "PIC_MUTING", "Pic Mute", "qrc:/image/rempicmute.png");
            createRemoteKeyBtnCol(true, idRow33, btnWidth2, rowHeight, rowHeight, "AUDIO_MUTING", "Audio Mute", "qrc:/image/remsoundmute.png");
        }
    }
}
