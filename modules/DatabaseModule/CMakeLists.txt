add_library(DatabaseModule STATIC
    main.cpp
)

target_link_libraries(DatabaseModule PRIVATE
    Qt6::Core
    Qt6::Sql
)

target_include_directories(DatabaseModule PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
