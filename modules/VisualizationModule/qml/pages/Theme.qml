pragma Singleton
import QtQuick

QtObject {
    id: root
    //  =====================================
    //  Colors palette
    //  =====================================
    property color cBg: "transparent"   //  "#F0F0F0"
    property color cPanel: "#FFFFFF"
    property color cBorder: "#D1D5DB"
    property color cText: "#1F2937"
    property color cTextMuted: "#6B7280"
    property color cTextDanger: "#991B1B"
    property color cPrimary: "#2563EB"
    property color cSuccess: "#10B981"
    property color cDanger: "#FEE2E2"
    property color cBorderDanger: "#EF4444"
    property color cWarning: "#F59E0B"

    //  =====================================
    //  Font and text display settings
    //  =====================================

    //  Font name for the intire application (Segoe UI, Arial, Roboto)
    property string fFamily: "Segor UI"

    //  Settings for main headings
    property int fSizeTitle: 22
    property bool fBoldTitle: true

    //  Settings for subheadings
    property int fSizeHeading: 14
    property bool fBoldHeading: true

    // Settings for large numbers (KPI)
    property int fSizeKpi: 32
    property bool fBoldKpi: true

    // Settings for KPI descriptions and labels
    property int fSizeDesc: 12
    property bool fBoldDesc: true
    property real fLetterSpacingDesc: 0.5 // Межбуквенный интервал (letter-spacing: 0.5px)

    // Settings for plain text on panels
    property int fSizeNormal: 14
    property bool fBoldNormal: false

    //  =====================================
    //  Downloading function from 'config.json'
    //  =====================================
    function loadConfig() {
        var request = new XMLHttpRequest();
        request.open("GET", "qrc:/qt/qml/config.json", false);

        request.onreadystatechange = function() {
            if (request.readyState === XMLHttpRequest.DONE) {
                if (request.status === 200 || request.status === 0) {
                    try {
                        var config = JSON.parse(request.responseText);

                        //  Парсим цвета
                        var colors = config.theme;
                        root.cBg = colors.cBg
                        root.cPanel = colors.cPanel;
                        root.cBorder = colors.cBorder;
                        root.cText = colors.cText;
                        root.cTextMuted = colors.cTextMuted;
                        root.cTextDanger = colors.cTextDanger;
                        root.cPrimary = colors.cPrimary;
                        root.cSuccess = colors.cSuccess;
                        root.cDanger = colors.cDanger;
                        root.cBorderDanger = colors.cBorderDanger;
                        root.cWarning = colors.cWarning;

                        //  Парсим настройки отображения текста
                        if (config.typography) {
                            var t = config.typography;
                            root.fFamily = t.fFamily || root.fFamily;

                            root.fSizeTitle = t.fSizeTitle || root.fFamily;
                            root.fBoldTitle = t.fBoldTitle !== undefined ? t.fBoldTitle : root.fBoldTitle;

                            root.fSizeHeading = t.fSizeHeading || root.fSizeHeading;
                            root.fBoldHeading = t.fBoldHeading || root.fBoldHeading;

                            root.fSizeKpi = t.fSizeKpi || root.fSizeKpi;
                            root.fBoldKpi = t.fBoldKpi !== undefined ? t.fBoldKpi : root.fBoldKpi;

                            root.fSizeDesc = t.fSizeDesc || root.fSizeDesc;
                            root.fBoldDesc = t.fBoldDesc !== undefined ? t.fBoldDesc: root.fBoldDesc;
                            root.fLetterSpacingDesc = t.fLetterSpacingDesc || root.fLetterSpacingDesc;

                            root.fSizeNormal = t.fSizeNormal || root.fSizeNormal;
                            root.fBoldNormal = t.fBoldNormal !== undefined ? t.fBoldNormal : root.fBoldNormal;
                        }

                        console.log("Конфигурация цветов и шрифтов загружена.");
                    } catch (e) {
                        console.warn("Ошибка парсинга JSON: ", e);
                    }
                }
            }
        };
        request.send();
    }
}
