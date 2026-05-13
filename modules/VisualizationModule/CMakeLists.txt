#   Создает переменную VISUALIZATION_TARGET со значением "VisualizationModule"
#   (имя цели в CMake для компиляции)
set(VISUALIZATION_TARGET VisualizationModule)


#   Добавляем публичные фасады (внешнее API)
set(MODULE_PUBLIC_FACADES
    facade/IVisualizationService.h
    facade/visualizationservice.h
)


#   Добавляем приватные заголовки (не экспортируются)
set(MODULE_PRIVATE_HEADERS
    private/filehandlermanager.h
    private/filehandler.h

    private/mymodel.h
)


#   Добавляем файлы исходного кода
set(MODULE_SOURCE_FILES
    src/filehandlermanager.cpp
    src/filehandler.cpp
    src/visualizationservice.cpp

    src/mymodel.cpp
)


set(MODULE_QML_FILES
    qml/AppMenuBar.qml
    qml/HomeView.qml
    qml/Launcher.qml
    qml/ImageProcessingView.qml
    qml/ImageView.qml
    qml/SideBar.qml
    qml/BlureWidget.qml
    qml/CustomWidget.qml
    qml/SideBarButton.qml
    qml/IconButton.qml
    qml/MaskOverlay.qml
    qml/ViewContainer.qml
    qml/InformationView.qml
    qml/FutterView.qml
    qml/ImageContainer.qml
    qml/ImagePickerDialog.qml
    qml/CameraView.qml
    qml/VideoPlayerView.qml
    qml/ModeTabBar.qml
    qml/CamcoderIpInput.qml
    qml/MainContentView.qml
    qml/ButtonBar.qml
)


set(MODULE_IMAGE_FILES
    qrc/setCamera6.svg
    qrc/setVideocamera11.svg
)


#   Регистрирует QML-модуль для основной цели приложения
#   Превращает созданную библиотеку в полноценный QML-модуль
qt_add_qml_module(${VISUALIZATION_TARGET}
    STATIC
    #   Логическое имя для QML (import VisualizationModule 1.0)
    URI VisualizationModule
    VERSION 1.0

    #   Базовый путь в системе ресурсов (будет доступно как qrc:/qt/qml/...)
    #RESOURCE_PREFIX /qt/qml/
    QML_FILES
        Visualization.qml
        ${MODULE_QML_FILES}

    RESOURCES
        #qtquickcontrols2.conf
        ${MODULE_IMAGE_FILES}

    SOURCES
        ${MODULE_PUBLIC_FACADES}
        ${MODULE_PRIVATE_HEADERS}
        ${MODULE_SOURCE_FILES}
)


# Указывает, что папка с этим модулем должна быть доступна для поиска заголовков (.h)
# PUBLIC означает, что любой проект, подключивший эту библиотеку, тоже увидит её заголовки
target_include_directories(${VISUALIZATION_TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/facade
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


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
    Qt6::Multimedia
)


#   Установка QML-файлов (если нужны отдельно на диске)
#install(FILES Visualization.qml ImageView.qml ButtonSideBar.qml
#    DESTINATION ${CMAKE_INSTALL_DATADIR}/qml/VisualizationModule
#)
