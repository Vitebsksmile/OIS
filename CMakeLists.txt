#   Устанавливает минимально необходимую версию CMake
# (3.25 нужна для многих фич Qt 6)
cmake_minimum_required(VERSION 3.27.7)


#   Создает переменную ORG_NAME (используется для отображаемого названия приложения)
set(DOMAIN_ORG_NAME "vstu.by")


#   Создает переменную ORG_NAME (используется для отображаемого названия приложения)
set(ORG_NAME "VSTU IS-23")


#   Создает переменную APP_NAME (используется для отображаемого названия приложения)
set(APP_NAME "OIS by SHCHERBAKOV")


#   Объявляет имя проекта (MyApp), его версию и используемый язык (C++)
project(OISApp VERSION 0.0.1 LANGUAGES CXX)


#   Устанавливает стандарт C++
set(CMAKE_CXX_STANDARD 17)


#   Запрещает компиляцию, если компилятор не поддерживает выбранный стандарт C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#   ?????
set(CMAKE_CXX_EXTENSIONS OFF)


#target_compile_definitions(${PROJECT_NAME} PRIVATE
#    APP_DOMAIN_ORG_NAME_STR="${DOMAIN_ORG_NAME}"
#    APP_ORG_NAME_STR="${ORG_NAME}"
#    AAPP_NAME_STR="${APP_NAME}"
#    APP_VERSION="${PROJECT_VERSION}"
#)


#   Добавляем определения (флаги) компиляции для передачи препроцессорных
#   макросов (#define) в C++ код прямо из CMake
add_definitions(
    -DAPP_DOMAIN_ORG_NAME_STR="${DOMAIN_ORG_NAME}"
    -DAPP_ORG_NAME_STR="${ORG_NAME}"
    -DAPP_NAME_STR="${APP_NAME}"
    -DAPP_VERSION="${PROJECT_VERSION}"
)


#   Ищет базовый пакет Qt 6.8 Core (необходим для работы любых программ на Qt)
find_package(Qt6 6.8 REQUIRED COMPONENTS
    Core            #   необходим для работы любых программ на Qt
    Qml             #   нужен для работы логики языка QML
    Quick           #   нужен для визуальных элементов интерфейса
    QuickControls2  #   расширяет возможности Quick
    QuickDialogs2   #   расширяет возможности Quick
    Gui             #   предоставляет классы для интеграции с оконной системой,
    #   обработки событий, интеграции с OpenGL и OpenGL ES, 2D-графики, базовой обработки изображений,
    #   шрифтов и текста. Эти классы используются внутри технологий пользовательского интерфейса Qt,
    #   но также могут использоваться напрямую, например, для написания приложений с использованием
    #   низкоуровневых графических API OpenGL ES.
    Multimedia
    Sql             #   модуль базы данных
)


#   Ищет модуль OpenCV (модуль компьютерного зрения)
find_package(OpenCV REQUIRED)


#   Применяет стандартные настройки Qt для проекта
#   (автоматизирует пути сборки и размещения файлов)
#   https://doc.qt.io/qt-6/qt-standard-project-setup.html
qt_standard_project_setup(REQUIRES 6.8) #    REQUIRES 6.8


#   Подключает модули логики приложения (вызываются их собственные CMakeLists.txt)
#add_subdirectory(modules/DatabaseModule)
add_subdirectory(modules/ImageProcessingModule)
#add_subdirectory(modules/IntegrationModule)
add_subdirectory(modules/VisualizationModule)


#   Подключает папку с основным исполняемым файлом приложения (точкой входа)
add_subdirectory(app)


#   Tests
#   Подключает встроенный модуль CMake для тестирования
#include(CTest)
#   Разрешает запуск тестов
#enable_testing()
#   Подключает папку с исходным кодом тестов
#add_subdirectory(tests)


#   Создает "виртуальную" цель, чтобы файлы ниже отображались в дереве проекта в IDE
add_custom_target(non-compiled-files SOURCES
    .gitignore      #   файл исключений Git
    .gitattributes  #   Защита от изменений. заставит Git воспринимать перечисленные файлы как бинарный файл
    #README.md       #   описание проекта
)
