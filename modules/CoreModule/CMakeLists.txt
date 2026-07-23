set(CORE_TARGET CoreModule)

set(MODULE_PUBLIC_HEADERS
  #private/ICameraStrategy.h
)

set(MODULE_PRIVATE_HEADERS
  #private/ICameraStrategy.h
)

set(MODULE_SOURCE_FILES
  #src/baslerdriver.cpp
  #src/cameraconfig.cpp
)

#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается" прямо в исполняемый файл при сборке
add_library(${CORE_TARGET} STATIC
  main.cpp
  ${MODULE_PUBLIC_HEADERS}
  ${MODULE_PRIVATE_HEADERS}
  ${MODULE_SOURCE_FILES}
  CameraTypes.h
  IFrameBuffer.h
  Frame.h
)

#target_include_directories(${CORE_TARGET}
#  PUBLIC
#    ${CMAKE_CURRENT_SOURCE_DIR}/facade
#  PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/private
#    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
#    ${CMAKE_CURRENT_SOURCE_DIR}/src
#)

target_link_libraries(${CORE_TARGET} PRIVATE
  Qt6::Core
)
