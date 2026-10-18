set(TEST_SOURCES
    test_main.cpp
    unit_tests.cpp
    functional_tests.cpp
)


add_executable(camera_manager_module_tests ${TEST_SOURCES}
    tst_VideoCaptureWorker.cpp)


target_compile_definitions(camera_manager_module_tests PRIVATE
    APP_DOMAIN_ORG_NAME_STR="${DOMAIN_ORG_NAME}"
    APP_ORG_NAME_STR="${ORG_NAME}"
    APP_NAME_STR="${APP_NAME}"
    APP_VERSION="${PROJECT_VERSION}"
)


target_link_libraries(camera_manager_module_tests PRIVATE
    Qt6::Core
    Qt6::Quick
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

target_include_directories(camera_manager_module_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/modules/CameraManagerModule/camera
)


add_test(NAME AllTests COMMAND camera_manager_module_tests)