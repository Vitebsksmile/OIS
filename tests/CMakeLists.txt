set(TEST_SOURCES
    test_main.cpp
)


add_executable(unit_tests ${TEST_SOURCES})


target_compile_definitions(unit_tests PRIVATE
    APP_DOMAIN_ORG_NAME_STR="${DOMAIN_ORG_NAME}"
    APP_ORG_NAME_STR="${ORG_NAME}"
    APP_NAME_STR="${APP_NAME}"
    APP_VERSION="${PROJECT_VERSION}"
)


target_link_libraries(unit_tests PRIVATE 
    Qt6::Core 
    Qt6::Test
    Qt6::Sql
    ${OpenCV_LIBS}
    
    # Линкуем ваши модули напрямую как статические библиотеки
    DatabaseModule
    ImageProcessingModule
    VisualizationModule
    CameraManagerModule
    CoreModule

    VisualizationModuleplugin
)


add_test(NAME AllTests COMMAND unit_tests)