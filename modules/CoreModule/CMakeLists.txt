set(CORE_TARGET CoreModule)

set(INTERFACE_FACADES
    facade/IVisualizationService.h
    facade/IDatabaseService.h
    facade/IImageProcessingService.h
    facade/ICameraManagerService.h
)

set(interface_models
    models/IDbModel.h
    #models/IDbSqlRelationTableModel.h
)

set(MODULE_PUBLIC_HEADERS
    include/CameraTypes.h
    include/IFrameBuffer.h
    include/Frame.h
    include/Operators.h
)

set(PIBLIC_CORE_HEADERS
    core/dbrecord.h
)

set(MODULE_SOURCE_FILES
    core/dbrecord.cpp
)

#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается" прямо в исполняемый файл при сборке
add_library(${CORE_TARGET} STATIC
    ${INTERFACE_FACADES}
    ${interface_models}
    ${MODULE_PUBLIC_HEADERS}
    ${MODULE_PRIVATE_HEADERS}
    ${PIBLIC_CORE_HEADERS}
    ${MODULE_SOURCE_FILES}
)

target_include_directories(${CORE_TARGET} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/facade>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/models>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>
)

target_link_libraries(${CORE_TARGET} PRIVATE
    Qt6::Core
    Qt6::Gui
    ${OpenCV_LIBS}
)

add_library(OIS::Core ALIAS ${CORE_TARGET})
