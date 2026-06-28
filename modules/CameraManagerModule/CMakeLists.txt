set(CAMERA_MANAGER_TARGET CameraManagerModule)

set(MODULE_PUBLIC_FACADES)

set(MODULE_PRIVATE_HEADERS
  private/cameramanager.h
)

set(MODULE_SOURCE_FILES
  src/cameramanager.cpp
)


add_library(${CAMERA_MANAGER_TARGET} STATIC
  ${MODULE_PUBLIC_FACADES}
  ${MODULE_PRIVATE_HEADERS}
  ${MODULE_SOURCE_FILES}
)

# Подключаем заголовочные файлы pylon SDK
target_include_directories(${CAMERA_MANAGER_TARGET} PRIVATE "D:/Program Files/Basler/pylon/Development/include")
target_link_libraries(${CAMERA_MANAGER_TARGET} PRIVATE "${CMAKE_SOURCE_DIR}/lib/libPylonC.a")

target_include_directories(${CAMERA_MANAGER_TARGET}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/facade
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${CAMERA_MANAGER_TARGET} PRIVATE
  Qt6::Quick
)
