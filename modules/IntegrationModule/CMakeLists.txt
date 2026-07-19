set(INTEGRATION_TARGET IntegrationModule)


set(MODULE_PUBLIC_FACADES
    facade/IIntegrationService.h
)


set(MODULE_PRIVATE_HEADERS)


add_library(${INTEGRATION_TARGET} STATIC
    ${MODULE_PUBLIC_FACADES}
)

target_link_libraries(${INTEGRATION_TARGET} PRIVATE
    Qt6::Core
    Qt6::Network
)

target_include_directories(${INTEGRATION_TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/facade
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private
)
