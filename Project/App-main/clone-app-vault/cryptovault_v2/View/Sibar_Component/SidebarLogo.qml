import QtQuick 2.15
import QtQuick.Layouts 1.15

RowLayout {
    property color primaryBlue

    Layout.bottomMargin: 30
    spacing: 12
    Rectangle {
        width: 32; height: 32; radius: 8
        color: primaryBlue
    }
    Text { text: "Drew"; font.bold: true; font.pixelSize: 20; color: "#f8fafc" }
}
