#   Создает переменную DATABASEMODULE_TARGET со значением "DatabaseModuleLib"
#   (имя цели в CMake для компиляции)
set(DATABASEMODULE_TARGET DatabaseModule)

set(MODULE_PUBLIC_FACADES
    facade/databaseservice.h
)

set(MODULE_PRIVATE_HEADERS

)

set(models_headers
    private/models/dbmodel.h
    private/models/dbrelationaltablemodel.h
)

#   Создает список файлов исходного кода и заголовков
list(APPEND MODULE_SOURCE_FILES
    facade/databaseservice.cpp
    private/models/dbmodel.cpp
    private/models/dbrelationaltablemodel.cpp
)

#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается прямо в исполняемый файл при сборке
qt_add_library(${DATABASEMODULE_TARGET} STATIC
    ${MODULE_PUBLIC_FACADES}
    ${MODULE_PRIVATE_HEADERS}
    ${models_headers}
    ${MODULE_SOURCE_FILES}
)

#   Указываем, что папка с этим модулем должна быть доступна для поиска заголовков (.h)
#   PUBLIC означает, что любой проект, подключивший эту библиотеку, тоже увидит её заголовки
target_include_directories(${DATABASEMODULE_TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/facade
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private
        ${CMAKE_CURRENT_SOURCE_DIR}/private/models
)


#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${DATABASEMODULE_TARGET} PRIVATE
    Qt6::Sql
    Qt6::Network
    CoreModule
)


