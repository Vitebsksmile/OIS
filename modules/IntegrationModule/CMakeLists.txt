add_library(IntegrationModule STATIC
#    integrationmanager.cpp # add
#    integrationmanager.h # add
#    jsonutils.cpp # add
#    jsonutils.h # add
#delete
)

target_link_libraries(IntegrationModule PRIVATE
    Qt6::Core
    Qt6::Network
)

target_include_directories(IntegrationModule PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
