set(CAMERA_MANAGER_TARGET CameraManagerModule)

set(MODULE_PUBLIC_FACADES
    facade/ICameraManagerService.h
    facade/cameramanagerservice.h
)

set(MODULE_CAMERA_HEADERS
    camera/AbstractCamera.h
    camera/videocaptureworker.h
)

set(MODULE_FACTORY
    factory/FactoryCamera.h
)

set(MODULE_INTERFACE
    interface/ICamera.h
    interface/ICameraStrategy.h
)

set(MODULE_DRIVERS_HEADERS
    drivers/ICameraDriver.h
    drivers/baslerdriver.h
)

set(MODULE_CONFIG_HEADERS
    config/cameraconfig.h
)

set(MODULE_SOURCE_FILES
    facade/cameramanagerservice.cpp
    drivers/baslerdriver.cpp
    config/cameraconfig.cpp
    camera/videocaptureworker.cpp
)

#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается" прямо в исполняемый файл при сборке
add_library(${CAMERA_MANAGER_TARGET} STATIC
    ${MODULE_PUBLIC_FACADES}
    ${MODULE_CAMERA_HEADERS}
    ${MODULE_FACTORY}
    ${MODULE_DRIVERS_HEADERS}
    ${MODULE_CONFIG_HEADERS}
    ${MODULE_SOURCE_FILES}
)

target_include_directories(${CAMERA_MANAGER_TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/facade
        #"D:/Program Files/Basler/pylon 7/Development/include"
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/camera
        ${CMAKE_CURRENT_SOURCE_DIR}/interface
        ${CMAKE_CURRENT_SOURCE_DIR}/config
        ${CMAKE_CURRENT_SOURCE_DIR}/private
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/lib

)

target_include_directories(${CAMERA_MANAGER_TARGET}
    PUBLIC
        "D:/Program Files/Basler/pylon 7/Development/include"
)

# target_link_directories(${CAMERA_MANAGER_TARGET} PRIVATE
#     ${CMAKE_SOURCE_DIR}/modules/CameraManagerModule/lib
# )

target_link_libraries(${CAMERA_MANAGER_TARGET}
    PRIVATE
        Qt6::Quick
        ${OpenCV_LIBS}
        CoreModule

        #PylonC_v8_0
        ${CMAKE_SOURCE_DIR}/modules/CameraManagerModule/lib/libPylonC_v8_0.a
)
