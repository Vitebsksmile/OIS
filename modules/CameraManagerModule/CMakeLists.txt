set(CAMERA_MANAGER_TARGET CameraManagerModule)

set(MODULE_PUBLIC_FACADES
    facade/ICameraManagerService.h
    facade/cameramanagerservice.h
)

set(MODULE_PRIVATE_INTERFACE
    interface/ICamera.h
    interface/IFrameObserver.h
    interface/IGrabStrategy.h
    interface/ICameraDriver.h
    interface/ICameraFactory.h
)

set(MODULE_PRIVATE_HEADERS)

set(MODULE_DRIVERS_HEADERS
    drivers/baslerdriver.h
)

set(MODULE_CONFIG_HEADERS
    config/cameraconfig.h
)

set(MODULE_SOURCE_FILES
    src/cameramanagerservice.cpp
    src/baslerdriver.cpp
    src/cameraconfig.cpp)

#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается" прямо в исполняемый файл при сборке
add_library(${CAMERA_MANAGER_TARGET} STATIC
<<<<<<< Updated upstream
    ${MODULE_PUBLIC_FACADES}
    ${MODULE_PRIVATE_INTERFACE}
    ${MODULE_PRIVATE_HEADERS}
    ${MODULE_DRIVERS_HEADERS}
    ${MODULE_CONFIG_HEADERS}
    ${MODULE_SOURCE_FILES}
    interface/ICameraFactoryRegistry.h
=======
  ${MODULE_PUBLIC_FACADES}
  ${MODULE_PRIVATE_HEADERS}
  ${MODULE_DRIVERS_HEADERS}
  ${MODULE_CONFIG_HEADERS}
  ${MODULE_SOURCE_FILES}

  private/ICamera.h
  private/IFrameObserver.h
>>>>>>> Stashed changes
)

# Подключаем заголовочные файлы pylon SDK
target_include_directories(${CAMERA_MANAGER_TARGET} PRIVATE
<<<<<<< Updated upstream
    "D:/Program Files/Basler/pylon/Development/include")

target_link_libraries(${CAMERA_MANAGER_TARGET} PRIVATE
    "${CMAKE_SOURCE_DIR}/modules/CameraManagerModule/lib/libPylonC.a")

target_include_directories(${CAMERA_MANAGER_TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/facade
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${CAMERA_MANAGER_TARGET} PRIVATE
    Qt6::Quick
    OIS::Core)
=======
  "D:/Program Files/Basler/pylon/Development/include"
)

target_link_libraries(${CAMERA_MANAGER_TARGET} PRIVATE
  "${CMAKE_SOURCE_DIR}/modules/CameraManagerModule/lib/libPylonC.a"
)

target_include_directories(${CAMERA_MANAGER_TARGET}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/facade
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${CAMERA_MANAGER_TARGET} PRIVATE
  Qt6::Quick

  CoreModule
)
>>>>>>> Stashed changes
