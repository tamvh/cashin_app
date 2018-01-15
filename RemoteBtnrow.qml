import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle {
    property string btnId;
    property string btnIcon;
    property string displayName;
    property string btnWidth;
    property string btnHeight;
    property string parentHeight;
    property real itmOpacity: btnOpacity;

    width: btnWidth
    height: btnHeight
    radius: 10
    color: "transparent"
    border.color: "white"
    border.width: 1
    opacity: 1
    anchors.margins: 5
    anchors.verticalCenter: parent.verticalCenter

    signal clicked(string btnId);

    Component.onCompleted: {
        idicon.parent.width = idicon.width + idlabel.width
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 1
        radius: 10
        color: "white"
        opacity: itmOpacity
    }

    Row {
        width: btnWidth
        height: parentHeight
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        RowLayout {
            width: (idicon.width + idlabel.contentWidth + 10) // btnWidth
            height: btnHeight
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            spacing: 10

            Rectangle { // khoảng trắng bên trái
                opacity: 0
                color: "red"
                height: btnHeight
                width: (parent.parent.width - parent.width) / 2
            }

            Image {
                id: idicon
                height: btnHeight
                fillMode: Image.Pad
                verticalAlignment: Qt.AlignVCenter
                source: btnIcon
            }

            Label {
                id: idlabel
                width: contentWidth
                height: btnHeight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                text: displayName
                font.pointSize: 20
                color: "white"
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            itmOpacity = btnOpacity*2
        }

        onReleased: {
            itmOpacity = btnOpacity
        }

        onClicked: parent.clicked(parent.btnId)
    }
}
