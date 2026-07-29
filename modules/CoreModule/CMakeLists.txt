set(CORE_TARGET CoreModule)

set(MODULE_PUBLIC_HEADERS
    include/CameraTypes.h
    include/IFrameBuffer.h
    include/Frame.h
)

#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается" прямо в исполняемый файл при сборке
add_library(${CORE_TARGET} STATIC
    ${MODULE_PUBLIC_HEADERS}
    ${MODULE_PRIVATE_HEADERS}

)

target_include_directories(${CORE_TARGET} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${CORE_TARGET} PRIVATE Qt6::Core)

add_library(OIS::Core ALIAS ${CORE_TARGET})
