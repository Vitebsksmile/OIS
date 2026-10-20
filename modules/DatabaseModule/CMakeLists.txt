#   Создает переменную DATABASEMODULE_TARGET со значением "DatabaseModuleLib"
#   (имя цели в CMake для компиляции)
set(DATABASEMODULE_TARGET DatabaseModule)

set(MODULE_PUBLIC_FACADES
    facade/databaseservice.h
)

set(core_headers
    core/IDetectionWriteRepository.h
    core/IDetectionModelFactory.h
)

set(builder_headers
    builders/sqlquerybuilder.h
)

set(repository_headers
    repository/userrepository.h
    repository/sessionrepository.h
)

set(models_headers
    models/dbrelationaltablemodel.h
)

set(private_headers
    private/systemenvironment.h
)

#   Создает список файлов исходного кода
list(APPEND MODULE_SOURCE_FILES
    facade/databaseservice.cpp
    builders/sqlquerybuilder.cpp
    repository/userrepository.cpp
    repository/sessionrepository.cpp
    models/dbrelationaltablemodel.cpp
    private/systemenvironment.cpp
)

#   Создает статическую библиотеку (STATIC), адаптированную под механизмы Qt
#   Статическая библиотека "вшивается прямо в исполняемый файл при сборке
qt_add_library(${DATABASEMODULE_TARGET} STATIC
    ${MODULE_PUBLIC_FACADES}
    ${core_headers}
    ${builder_headers}
    ${repository_headers}
    ${models_headers}
    ${private_headers}
    ${MODULE_SOURCE_FILES}
)

#   Указываем, что папка с этим модулем должна быть доступна для поиска заголовков (.h)
#   PUBLIC означает, что любой проект, подключивший эту библиотеку, тоже увидит её заголовки
target_include_directories(${DATABASEMODULE_TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/facade
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/builders
        ${CMAKE_CURRENT_SOURCE_DIR}/repository
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_SOURCE_DIR}/private
)


#   Линкует (связывает) приложение с необходимыми библиотеками и зависимостями
target_link_libraries(${DATABASEMODULE_TARGET} PRIVATE
    Qt6::Sql
    Qt6::Network
    CoreModule
)


