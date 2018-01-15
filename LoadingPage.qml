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
    id: loadingPage
    closePolicy: Popup.NoAutoClose
    modal: true
    focus: true
    x: (parent.width - width)/2
    y: (parent.height - height)/2
    topPadding: 40
    rightPadding: 40
    leftPadding: 40
    bottomPadding: 40
    contentItem: ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        width: 500
        RowLayout {
            spacing: 20
            QtControl14.BusyIndicator {
                id: progressScan
                running: image.status === Image.Loading
                height: 100
                width: 100
            }
            Text {
                id: txtText
                text: "Hệ thống đang xử lý..."
                font.pixelSize: 26
                color: "#4A4A4A"
            }
        }
    }
}
