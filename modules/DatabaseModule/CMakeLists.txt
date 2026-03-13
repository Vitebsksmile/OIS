#   Сщздает переменную DATABASEMODULE_TARGET со значением "DatabaseModuleLib"
#   (имя цели в CMake для компиляции)
set(DATABASEMODULE_TARGET DatabaseModuleLib)


#   Создает список файлов исходного кода и заголовков
list(APPEND MODULE_SOURCE_FILES)


#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается прямо в исполняемый файл при сборке
qt_add_library(${DATABASEMODULE_TARGET} STATIC
    ${MODULE_SOURCE_FILES}
)


#   Указываем, что папка с этим модулем должна быть доступна для поиска заголовков (.h)
#   PUBLIC означает, что любой проект, подключивший эту библиотеку, тоже увидит её заголовки
target_include_directories(${DATABASEMODULE_TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${DATABASEMODULE_TARGET} PUBLIC
    Qt6::Sql
)


