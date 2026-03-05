#pragma once
#include <QtCore/qglobal.h>

/*Это проверка макроса, который обычно определяется в файле проекта (CMakeLists.txt или .pro). Если макрос определен, значит, компилятор прямо сейчас собирает файлы внутри вашей библиотеки.*/
#if defined(VISUALIZATIONMODULE_LIBRARY)

/*Когда вы собираете библиотеку, макрос VISUALIZATIONMODULE_EXPORT превращается в "инструкцию" для компилятора выставить этот класс напоказ, чтобы другие программы могли его увидеть внутри .dll.*/
#  define VISUALIZATIONMODULE_EXPORT Q_DECL_EXPORT
#else

/*Когда вы подключаете эту библиотеку в другом проекте (например, в основном приложении), макрос VISUALIZATIONMODULE_LIBRARY там не определен. Тогда макрос превращается в инструкцию "импортировать это из внешней библиотеки".*/
#  define VISUALIZATIONMODULE_EXPORT Q_DECL_IMPORT
#endif
