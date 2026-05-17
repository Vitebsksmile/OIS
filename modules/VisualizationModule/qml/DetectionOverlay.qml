import QtQuick

Canvas {
    property var detections: []

    onPaint: {
        var ctx = getContext("2d")
        ctx.strokeStyle = "red"
        ctx.lineWidth = 2

        for (var i = 0; i < detections.length; i++) {
            var d = detections[i]
            ctx.strokeRect(d.x, d.y, d.w, d.h)
        }
    }
}
