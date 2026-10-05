import QtQuick
import QtQuick.Controls

TextField {
    id: root

    topPadding: 5
    bottomPadding: 5
    leftPadding: 5
    rightPadding: 5
    width: contentWidth + leftPadding + rightPadding

    // Центрируем текст по горизонтали и вертикали
    horizontalAlignment: TextInput.AlignHCenter
    verticalAlignment: TextInput.AlignVCenter

    font.family: Theme.fFamily
    font.pixelSize: Theme.fSizeNormal
    font.bold: Theme.fBoldNormal
}
