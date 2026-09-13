pragma Singleton
import QtQuick

QtObject {
    id: root

    //  --- Данные для панели предупреждения ---
    property bool hasAlarm: true
    property string alarmMessage: "⚠️ СЕРИЙНЫЙ БРАК! Компонент R12 [Смещение] на 3-х платах подряд!"

    //  --- Данные статуса оператора ---
    property string currentStatus: "РАБОТА"
    property string operatorName: "Иванов И.И."

    //Метрики эффективности (KPI) ---
    property string fpyValue: "98.4 %"
    property string fpyThreshold: "Порог нормы: >97.5 %"

    //  --- Счетчики смены ---
    property int countTotal: 1245
    property int countPass: 1225
    property int countFail: 20

    //  Данные по последнему браку ---
    property string lastDefectPosition: "R12"
    property string lastDefectType: "Смещение компонента X/Y"
    property string lastDefectPhoto: "[ ФОТО С КАМЕРЫ ]"    //  Здесь может быть путь к картинке

    //  --- пример функции для обновления данных из C++ бэкенда ---
    function updateCounters(total, pass, fail) {
        countTotal = total;
        countPass = pass;
        countFail = fail;
    }
}
