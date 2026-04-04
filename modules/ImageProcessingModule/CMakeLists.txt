#   Создает переменную IMAGE_PROCESSING_TARGET со значением "ImageProcessingModuleLib"
#   (имя цели в CMake для компиляции)
set(IMAGE_PROCESSING_TARGET ImageProcessingModule)


#   Добавляем публичные фасады (внешнее API)
set(MODULE_PUBLIC_FACADES
    facade/IImageProcessingService.h
    facade/imageprocessingservice.h
)


#   Добавляем приватные заголовки (не экспортируются)
set(MODULE_PRIVATE_HEADERS
    private/workingroom.h
    private/imagepreprocessing.h
)


#   Добавляем файлы исходного кода
set(MODULE_SOURCE_FILES
    src/imageprocessingservice.cpp
    src/workingroom.cpp
    src/imagepreprocessing.cpp
    #filters.h filters.cpp
    #calibration.h calibration.cpp
)


#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается" прямо в исполняемый файл при сборке
qt_add_library(${IMAGE_PROCESSING_TARGET} STATIC
    ${MODULE_PUBLIC_FACADES}
    ${MODULE_PRIVATE_HEADERS}
    ${MODULE_SOURCE_FILES}
)


#   Include директории
target_include_directories(${IMAGE_PROCESSING_TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/facade
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${IMAGE_PROCESSING_TARGET} PUBLIC
    Qt6::Quick
    ${OpenCV_LIBS}
)
