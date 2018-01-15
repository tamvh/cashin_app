import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle {
    id: itemcontainer

    property string btnId;
    property string btnIcon;
    property string displayName;
    property string btnWidth;
    property string btnHeight;
    property string parentHeight;
    property real itmOpacity: 1;

    width: btnWidth
    height: btnHeight
    radius: btnHeight
    color: "transparent"
    anchors.verticalCenter: parent.verticalCenter

    signal clicked(string btnId);

    Image {
        id: idicon
        height: btnHeight
        width: btnWidth
        fillMode: Image.Pad
        verticalAlignment: Qt.AlignVCenter
        source: btnIcon
        opacity:itmOpacity

        MouseArea {
            anchors.fill: parent
            onPressed: {
                itmOpacity = 0.5
            }

            onReleased: {
                itmOpacity = 1
            }

            onClicked: itemcontainer.clicked(itemcontainer.btnId)
        }
    }
}
