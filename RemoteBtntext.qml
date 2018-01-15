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
    property int rRound: 1

    width: btnWidth
    height: btnHeight
    radius: rRound
    color: "transparent"
    border.color: "white"
    border.width: 2
    opacity: 1
    anchors.margins: 5
    anchors.verticalCenter: parent.verticalCenter

    signal clicked(string btnId);

    Rectangle {
        anchors.fill: parent
        anchors.margins: 1
        radius: rRound
        color: "white"
        opacity: itmOpacity

    }

    Label {
        id: idlabel
        width: btnWidth
        height: btnHeight
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        text: displayName
        font.pointSize: 20
        color: "white"
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
