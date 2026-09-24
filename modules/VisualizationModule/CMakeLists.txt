#   Создает переменную VISUALIZATION_TARGET со значением "VisualizationModule"
#   (имя цели в CMake для компиляции)
set(VISUALIZATION_TARGET VisualizationModule)

#   Добавляем публичные фасады (внешнее API)
set(MODULE_PUBLIC_FACADES
    facade/visualizationservice.h
)

set(MODULE_PRIVATE_CONTROLLERS
    controllers/filehandlerController.h
    controllers/videostreamcontroller.h
    controllers/dbmodelcontroller.h
    controllers/authController.h
)

set(models
    models/ListModel.h
)

#   Добавляем приватные заголовки (не экспортируются)
set(MODULE_PRIVATE_HEADERS
    private/filehandler.h
    private/videoProvider.h
    #private/opencvvideoitem.h
)

#   Добавляем файлы исходного кода
set(MODULE_SOURCE_FILES
    facade/visualizationservice.cpp
    controllers/authController.cpp
    controllers/filehandlerController.cpp
    controllers/videostreamcontroller.cpp
    controllers/dbmodelcontroller.cpp
    #private/opencvvideoitem.cpp
    private/filehandler.cpp
    private/videoProvider.cpp
)

set(PLAIN_QML_FILES
    qml/AppMenuBar.qml
    qml/AppSideBar.qml

    qml/pages/LoginRegistrationPage.qml
    qml/pages/DashboardPage.qml
    qml/pages/InspectionPage.qml
    qml/pages/StatisticsPage.qml
    qml/pages/SettingsPage.qml
    qml/pages/OperatorPage.qml
    qml/pages/UserListPage.qml
    qml/pages/UserListPage_2.qml
    qml/pages/UserManualPage.qml

    qml/settings/TitleText.qml
    qml/settings/HeadingText.qml
    qml/settings/KpiText.qml
    qml/settings/NormalText.qml
    qml/settings/MutedText.qml

    qml/model/InspectionStatisticsModel.qml
    qml/model/InspectionSummaryModel.qml
    qml/model/MetricsModel.qml
    qml/model/DetectionStatisticsModel.qml
    qml/model/StatisticsResultTableModel.qml

    qml/DetectionOverlay.qml
    qml/SideBarButton.qml
    qml/ImagePickerDialog.qml
    qml/ButtonBar.qml
    qml/ControlPanel.qml
    qml/AppToolBar.qml

    qml/view/ImageContainerView.qml
    qml/view/InformationView.qml
    qml/view/ImageView.qml
    qml/view/LifeView.qml
    qml/view/ProcessingView.qml

    qml/view/CameraSettingsView.qml
    qml/view/ImageProcessingSettingsView.qml
    qml/view/MLSettingsView.qml
    qml/view/DatabaseSettingsView.qml
    qml/view/ButtonSettingsView.qml
)

set(qml_model_singleton
    qml/model/OperatorModel.qml
)

set(QML_SINGLETONS
    qml/pages/Theme.qml
)

set(MODULE_IMAGE_FILES
    qrc/setCamera6.svg
    qrc/setVideocamera11.svg
)

set_source_files_properties(${QML_SINGLETONS} ${qml_model_singleton}
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
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
        ${PLAIN_QML_FILES}
        ${QML_SINGLETONS}
        ${qml_model_singleton}

    RESOURCES
        #qtquickcontrols2.conf
        ${MODULE_IMAGE_FILES}

    SOURCES
        ${MODULE_PUBLIC_FACADES}
        ${MODULE_PRIVATE_CONTROLLERS}
        ${models}
        ${MODULE_PRIVATE_INTERFACE}
        ${MODULE_PRIVATE_HEADERS}
        ${MODULE_SOURCE_FILES}

    DEPENDENCIES
        QtMultimedia
        QML_FILES config.json
)


# Указывает, что папка с этим модулем должна быть доступна для поиска заголовков (.h)
# PUBLIC означает, что любой проект, подключивший эту библиотеку, тоже увидит её заголовки
target_include_directories(${VISUALIZATION_TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/facade
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/controllers
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_SOURCE_DIR}/private
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${VISUALIZATION_TARGET} PUBLIC
    Qt6::Core
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::QuickDialogs2
    Qt6::Gui
    Qt6::Multimedia

    CoreModule
)


#   Установка QML-файлов (если нужны отдельно на диске)
#install(FILES Visualization.qml ImageView.qml ButtonSideBar.qml
#    DESTINATION ${CMAKE_INSTALL_DATADIR}/qml/VisualizationModule
#)
