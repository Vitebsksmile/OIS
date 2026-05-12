#   Создает переменную APP_TARGET со значением "OISApp"
#   (имя цели в CMake для компиляции)
#set(APP_TARGET OISApp PARENT_SCOPE) # Для главного (верхнего) CMake
#set(APP_TARGET OISApp)

#set_target_properties(app_bin PROPERTIES OUTPUT_NAME ${PROJECT_NAME})


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


#   Ищет установленный пакет Qt 6.5 и выше с обязательными модулями Core Qml и т.д.
#find_package(Qt6 6.5 REQUIRED COMPONENTS Core Qml Quick QuickControls2 Gui)    #  for static


#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${PROJECT_NAME} PRIVATE

    #   Стандартные библиотеки Qt
    #Qt6::Qml
    #Qt6::Quick
    #Qt6::QuickLayouts                   #   for STATIC
    #Qt6::QuickControls2
    #Qt6::QuickControls2Basic            #   for STATIC
    #Qt6::QuickControls2WindowsStyleImpl #   for STATIC
    #Qt6::Gui
    #Qt6::Multimedia
    #Qt6::Sql

    #   Сторонние библиотеки и плагины
    #${OpenCV_LIBS}

    #   Библиотеки проекта для доступа к .h файлам и C++ логике
    #DatabaseModule
    ImageProcessingModule
    #IntegrationModule
    VisualizationModule


    #   --- для static build --- {-->>

    #   Стандартные библиотеки Qt for STATIC
    #Qt6::qquicklayoutsplugin
    #Qt6::qtquickcontrols2plugin
    #Qt6::qtquickcontrols2basicstyleplugin


    #   Для гарантии регистрации QML-типов при static build
    VisualizationModuleplugin
    #ImageProcessingModuleLibplugin

    #   --<<}

)


#include(${PROJECT_SOURCE_DIR}/app/fixup_script.cmake.in)


# 1. Определяем папку для сборки дистрибутива
set(DEPLOY_DIR "${CMAKE_BINARY_DIR}/release_app")

# 2. Добавляем команду после сборки основного приложения
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${DEPLOY_DIR}/"

    # 1. Копируем плагин платформы (без него Qt-приложение не запустится)
    COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_DIR}/platforms"
    COMMAND ${CMAKE_COMMAND} -E copy "D:/msys64/mingw64/share/qt6/plugins/platforms/qwindows.dll" "${DEPLOY_DIR}/platforms/"

    # 2. Копируем необходимые папки QML модулей
    # Можно скопировать всё (будет весить много), либо только нужные:
    #COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_DIR}/qml"

    # Копируем конкретно QtQuick и Controls (самые важные)
    #COMMAND ${CMAKE_COMMAND} -E copy_directory "${MSYS_QML_DIR}/QtQuick" "${DEPLOY_DIR}/qml/QtQuick"
    #COMMAND ${CMAKE_COMMAND} -E copy_directory "${MSYS_QML_DIR}/Qt" "${DEPLOY_DIR}/qml/Qt"

    # Если используете Controls:
    #COMMAND ${CMAKE_COMMAND} -E copy_directory "${MSYS_QML_DIR}/QtQuick/Controls" "${DEPLOY_DIR}/qml/QtQuick/Controls"

    # 3. Запускаем fixup_script для сбора OpenCV и системных DLL (libstdc++, libgcc и т.д.)
    COMMAND ${CMAKE_COMMAND}
        "-DEXE_PATH=${DEPLOY_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        "-DSEARCH_DIRS=D:/msys64/mingw64/bin"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/fixup_script.cmake"
    VERBATIM
)
