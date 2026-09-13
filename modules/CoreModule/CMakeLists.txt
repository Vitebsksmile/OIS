set(CORE_TARGET CoreModule)

set(INTERFACE_FACADES
    facade/IVisualizationService.h
    facade/IDatabaseService.h
    facade/IImageProcessingService.h
    facade/ICameraManagerService.h
)

set(interface_models
    models/IDbModel.h
)

set(MODULE_PUBLIC_HEADERS
    include/CameraTypes.h
    include/IFrameBuffer.h
    include/Frame.h
    include/Operators.h
)

#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается" прямо в исполняемый файл при сборке
add_library(${CORE_TARGET} STATIC
    ${INTERFACE_FACADES}
    ${interface_models}
    ${MODULE_PUBLIC_HEADERS}
    ${MODULE_PRIVATE_HEADERS}
)

target_include_directories(${CORE_TARGET} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/facade>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/models>
)

target_link_libraries(${CORE_TARGET} PRIVATE
    Qt6::Core
    Qt6::Gui
    ${OpenCV_LIBS}
)

add_library(OIS::Core ALIAS ${CORE_TARGET})
