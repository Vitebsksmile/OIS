#   Создает переменную APP_TARGET со значением "OISApp"
#   (имя цели в CMake для компиляции)
set(APP_TARGET OISApp)


#   Создает переменную APP_NAME (используется для отображаемого названия приложения)
set(APP_NAME "OIS by SHCHERBAKOV")


#   Создает исполняемый файл проекта, используя main.cpp как точку входа
qt_add_executable(${APP_TARGET}
    main.cpp
    application.h application.cpp
)


#   Добавляет определения (флаги) компиляции для передачи препроцессорных
#   макросов (#define) в C++ код прямо из CMake
target_compile_definitions(${APP_TARGET}

    PRIVATE

        #   Включает возможность QML-отладки только если проект собран в режиме Debug
        #   В коде появится #define QT_QML_DEBUG,
        #   будет возможным использовать Hot Reload или QML Inspector.
        #   В финальной версии (Release) этого макроса не будет
        $<$<CONFIG:Debug>:QT_QML_DEBUG>

        #   Передает переменную APP_NAME как строку в main.cpp
        APP_NAME_STR="${APP_NAME}"
)


#   Ищет установленный пакет Qt 6.5 и выше с обязательными модулями Core Qml и т.д.
#find_package(Qt6 6.5 REQUIRED COMPONENTS Core Qml Quick QuickControls2 Gui)    #  for static


#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${APP_TARGET} PRIVATE

    #   Стандартные библиотеки Qt
    Qt6::Qml
    Qt6::Quick
    #Qt6::QuickLayouts                   #   for STATIC
    Qt6::QuickControls2
    #Qt6::QuickControls2Basic            #   for STATIC
    #Qt6::QuickControls2WindowsStyleImpl #   for STATIC
    Qt6::Gui
    Qt6::Sql

    #   Сторонние библиотеки и плагины
    #${OpenCV_LIBS}

    #   Библиотеки проекта для доступа к .h файлам и C++ логике
    DatabaseModule
    ImageProcessingModule
    #IntegrationModule
    VisualizationModule


    #   --- для static build --- {-->>

    #   Стандартные библиотеки Qt for STATIC
    #Qt6::qquicklayoutsplugin
    #Qt6::qtquickcontrols2plugin
    #Qt6::qtquickcontrols2basicstyleplugin


    #   Для гарантии регистрации QML-типов при static build
    VisualizationModuleplugin
    #ImageProcessingModuleLibplugin

    #   --<<}

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
