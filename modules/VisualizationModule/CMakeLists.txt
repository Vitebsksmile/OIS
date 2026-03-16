#   Создает переменную VISUALIZATION_TARGET со значением "VisualizationModuleLib"
#   (имя цели в CMake для компиляции)
set(VISUALIZATION_TARGET VisualizationModuleLib)


#   Создает список файлов исходного кода и заголовков
list(APPEND MODULE_SOURCE_FILES
    #visualization.cpp
)


#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается" прямо в исполняемый файл при сборке
qt_add_library(${VISUALIZATION_TARGET} STATIC
    ${MODULE_SOURCE_FILES}
)


#   Создает список файлов исходного кода и заголовков интегрированных в QML
list(APPEND MODULE_SOURCE_FILES_FOR_QML
    filehandler.h filehandler.cpp
    mymodel.h mymodel.cpp
)


#   Создает список qml-файлов
list(APPEND MODULE_QML_FILES
    Visualization.qml
    ImageView.qml
    PreProcessedImageView.qml
    SideBar.qml
    SideBarButton.qml
)


#   Регистрирует QML-модуль для основной цели приложения
#   Превращает созданную библиотеку в полноценный QML-модуль
qt_add_qml_module(${VISUALIZATION_TARGET}

    #   Логическое имя для QML (import VisualizationModule 1.0)
    URI VisualizationModule
    VERSION 1.0

    #   Базовый путь в системе ресурсов (будет доступно как qrc:/qt/qml/...)
    RESOURCE_PREFIX /qt/qml/
    QML_FILES
        ${MODULE_QML_FILES}
    SOURCES
        ${MODULE_SOURCE_FILES_FOR_QML}
)


# Указывает, что папка с этим модулем должна быть доступна для поиска заголовков (.h)
# PUBLIC означает, что любой проект, подключивший эту библиотеку, тоже увидит её заголовки
target_include_directories(${VISUALIZATION_TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})


#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${VISUALIZATION_TARGET} PUBLIC
    Qt6::Core
    Qt6::Qml
    Qt6::Quick
    #Qt6::QuickLayouts                   #   for STATIC
    Qt6::QuickControls2
    #Qt6::QuickControls2Basic            #   for STATIC
    #Qt6::QuickControls2WindowsStyleImpl #   for STATIC
    Qt6::QuickDialogs2
    Qt6::Gui
)


# Установка QML-файлов (если нужны отдельно на диске)
#install(FILES Visualization.qml ImageView.qml ButtonSideBar.qml
#    DESTINATION ${CMAKE_INSTALL_DATADIR}/qml/VisualizationModule
#)
