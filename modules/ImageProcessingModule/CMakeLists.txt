#   Создает переменную IMAGE_PROCESSING_TARGET со значением "ImageProcessingModuleLib"
#   (имя цели в CMake для компиляции)
set(IMAGE_PROCESSING_TARGET ImageProcessingModuleLib)


#   Создает список файлов исходного кода и заголовков
list(APPEND MODULE_SOURCE_FILES
    #imageprocessor.h imageprocessor.cpp
    #filters.h filters.cpp
    #calibration.h calibration.cpp
)


#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается" прямо в исполняемый файл при сборке
qt_add_library(${IMAGE_PROCESSING_TARGET} STATIC
        ${MODULE_SOURCE_FILES}
)


list(APPEND MODULE_SOURCE_FILES_FOR_QML
    imageprocessor.h imageprocessor.cpp
)


#   Создает список qml-файлов
list(APPEND MODULE_QML_FILES )


#   Регистрирует QML-модуль для основной цели приложения
#   Превращает созданную библиотеку в полноценный QML-модуль
qt_add_qml_module(${IMAGE_PROCESSING_TARGET}

    #   Логическое имя для QML (import ImageProcessingModule 1.0)
    URI ImageProcessingModuleLib
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
target_include_directories(${IMAGE_PROCESSING_TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})


target_include_directories(${IMAGE_PROCESSING_TARGET} PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${IMAGE_PROCESSING_TARGET} PUBLIC
    Qt6::Quick
    ${OpenCV_LIBS}
)


# Установка QML-файлов (если нужны отдельно на диске)
#install(FILES Visualization.qml ImageView.qml ButtonSideBar.qml
#    DESTINATION ${CMAKE_INSTALL_DATADIR}/qml/VisualizationModule
#)
