import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQml.Models 2.1
import MainController 1.0

Page {
    property int itemHeight: 60

    Connections {
        target: mainController
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: borderWidth
        height: parent.height
        width: parent.width
        border.width: borderWidth
        border.color: idWindows.color
        color: idWindows.color

        Flickable {
            anchors.fill: parent
            contentHeight: column.height

            Column {
                id: column
                width: parent.width
                spacing: 20

                RowLayout {
                    width: parent.width
                    height: 50
                    spacing: 20

                    Button {
                        height: parent.height
                        text: "Thoát"
                        onClicked: mainController.appQuit()
                    }

                    Label {
                        Layout.fillWidth: true
                        height: parent.height
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignHCenter
                        font.pointSize: 18
                        text: "Build date: " + mainController.appBuildDate() + "   "
                    }
                }

                Rectangle {
                    anchors { left: parent.left; right: parent.right }
                    width: parent.width
                    height: itemHeight
                    color: idWindows.color

                    Row {
                        Label {
                            anchors.verticalCenter: parent.verticalCenter
                            width: 160
                            text: "MAC ADDRESS: "
                            font.pointSize: 18
                        }

                        TextField {
                            id: txtMacAddress
                            width: 250
                            font.pointSize: 18
                            text: mainController.getMacAddress()
                            onEditingFinished: {
                                mainController.setMacAddress(text)
                            }
                        }
                    }
                }
            }
        }
    }
}
