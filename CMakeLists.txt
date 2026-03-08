#   Устанавливает минимально необходимую версию CMake
# (3.25 нужна для многих фич Qt 6)
cmake_minimum_required(VERSION 3.19)


#   Объявляет имя проекта (MyApp), его версию и используемый язык (C++)
project(OIS VERSION 0.1 LANGUAGES CXX)


#   Устанавливает стандарт C++
set(CMAKE_CXX_STANDARD 17)


#   Запрещает компиляцию, если компилятор не поддерживает выбранный стандарт C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#   ?????
set(CMAKE_CXX_EXTENSIONS OFF)


#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)


#   Ищет базовый пакет Qt 6.5 Core (необходим для работы любых программ на Qt)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core)


#   Применяет стандартные настройки Qt для проекта
#   (автоматизирует пути сборки и размещения файлов)
qt_standard_project_setup()


#   Ищет модуль Qt Qml (нужен для работы логики языка QML)
find_package(Qt6 6.5 REQUIRED COMPONENTS Qml)


#   Ищет модуль Qt Quick (нужен для визуальных элементов интерфейса)
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)


#   Ищет модуль Qt QuickControls (расширяет возможности Quick)
find_package(Qt6 6.5 REQUIRED COMPONENTS QuickControls2)

#   Ищет модуль Qt QuickControls (расширяет возможности Quick)
find_package(Qt6 6.5 REQUIRED COMPONENTS QuickDialogs2)


#   Ищет модуль Qt Gui (предоставляет классы для интеграции с оконной системой,
#   обработки событий, интеграции с OpenGL и OpenGL ES, 2D-графики, базовой обработки изображений,
#   шрифтов и текста. Эти классы используются внутри технологий пользовательского интерфейса Qt,
#   но также могут использоваться напрямую, например, для написания приложений с использованием
#   низкоуровневых графических API OpenGL ES.)
find_package(Qt6 6.5 REQUIRED COMPONENTS Gui)


#   Ищет модуль OpenCV (модуль компьютерного зрения)
#find_package(OpenCV REQUIRED)


#   Устанавливает политику QTP0001 виртуального расположения qml-файлов
qt_policy(SET QTP0001 NEW)


#   Подключает модули логики приложения (вызываются их собственные CMakeLists.txt)
#add_subdirectory(modules/DatabaseModule)
#add_subdirectory(modules/ImageProcessingModule)
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
    #README.md       #   описание проекта
)
