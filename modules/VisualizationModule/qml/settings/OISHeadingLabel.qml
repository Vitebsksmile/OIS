import QtQuick
import QtQuick.Controls

Label {
    id: root
    font.family: Theme.fFamily
    font.pixelSize: Theme.fSizeHeading
    font.bold: Theme.fBoldHeading

    // Центрируем текст по горизонтали и вертикали
    horizontalAlignment: TextInput.AlignHCenter
    verticalAlignment: TextInput.AlignVCenter
}

