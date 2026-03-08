#   Создает переменную APP_TARGET со значением "OISApp"
#   (имя цели в CMake для компиляции)
set(APP_TARGET OISApp)


#   Создает переменную APP_NAME (используется для отображаемого названия приложения)
set(APP_NAME "OIS by SHCHERBAKOV")


#   Ищет установленный пакет Qt 6.5 и выше с обязательными модулями Qml и Quick
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Qml Quick QuickControls2 QuickDialogs2 Gui)


#   Создает исполняемый файл проекта, используя main.cpp как точку входа
qt_add_executable(${APP_TARGET}
    main.cpp
)


#   Устанавливает политику QTP0001 виртуального расположения qml-файлов
#qt_policy(SET QTP0001 NEW)

#   Регистрирует QML-модуль для основной цели приложения
#   Превращает созданную библиотеку в полноценный QML-модуль
qt_add_qml_module(${APP_TARGET}
    #   Уникальный логический адрес модуля (для 'import Primary')
    URI Primary
    #URI path

    #   Базовый путь в системе ресурсов (будет доступно как qrc:/qt/qml/...)
    RESOURCE_PREFIX /qt/qml/

    #   Версия модуля
    VERSION 1.0

    #   Список QML-файлов, которые будут вшиты в бинарник
    QML_FILES

)


#   Добавляет определения (флаги) компиляции для передачи препроцессорных
#   макросов (#define) в C++ код прямо из CMake
target_compile_definitions(${APP_TARGET}

    #   Включает возможность QML-отладки только если проект собран в режиме Debug
    #   В коде появится #define QT_QML_DEBUG,
    #   будет возможным использовать Hot Reload или QML Inspector.
    #   В финальной версии (Release) этого макроса не будет
    PRIVATE $<$<CONFIG:Debug>:QT_QML_DEBUG>
)


#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${APP_TARGET} PRIVATE

    #   Стандартные библиотеки Qt
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Gui

    #   Сторонние библиотеки и плагины
    ${OpenCV_LIBS}

    #   Личные библиотеки и плагины

    #   Для доступа к .h файлам и C++ логике
    #DatabaseModule
    #ImageProcessingModule
    #IntegrationModule
    VisualizationModuleLib

    #   Для гарантии регистрации QML-типов при static build
    VisualizationModuleLibplugin

)


#   Подключаем модуль для стандартных путей (обязательно перед install)
include(GNUInstallDirs)

#   Описывает правила установки (куда копировать файлы при команде 'install')
install(TARGETS ${APP_TARGET}
    #   Для Windows: сюда попадет ваш .exe файл
    #   Обычно это папка "bin" внутри директории установки
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"

    #   Сюда попадут .lib файлы (файлы импорта для DLL), если они есть
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"

    #   Сюда попадают .dll файлы, если они собраны как отдельные модули (SHARED)
    #   В Windows DLL считаются "рантаймом", поэтому они тоже летят в bin
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
