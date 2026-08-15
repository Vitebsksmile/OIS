# Собираем разделяемую версию
set(BUILD_SHARED_LIBS ON CACHE INTERNAL "Build using shared libraries ON/OFF")

message(BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS})


#   Создает исполняемый файл проекта, используя main.cpp как точку входа
add_executable(${PROJECT_NAME}
    main.cpp
    application.h application.cpp
)


#   Добавляем определения (флаги) компиляции для передачи препроцессорных
#   макросов (#define) в C++ код прямо из CMake
target_compile_definitions(${PROJECT_NAME}

    PRIVATE

        #   Включает возможность QML-отладки только если проект собран в режиме Debug
        #   В коде появится #define QT_QML_DEBUG,
        #   будет возможным использовать Hot Reload или QML Inspector.
        #   В финальной версии (Release) этого макроса не будет
        $<$<CONFIG:Debug>:QT_QML_DEBUG>
)


#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${PROJECT_NAME} PRIVATE

    #   Библиотеки проекта для доступа к .h файлам и C++ логике
    DatabaseModule
    ImageProcessingModule
    IntegrationModule
    VisualizationModule
    CameraManagerModule

    ${OpenCV_LIBS}

#   Для гарантии регистрации QML-типов при static build
    VisualizationModuleplugin

    #${CMAKE_SOURCE_DIR}/modules/CameraManagerModule/lib/libPylonC_v8_0.a
)
