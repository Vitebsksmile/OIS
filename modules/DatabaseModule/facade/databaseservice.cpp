#include "databaseservice.h"
#include <QStandardPaths>
#include <QSqlError>
#include <QSqlQuery>
#include <QHostInfo>
#include <QSysInfo>
#include <QNetworkInterface>
#include <QFile>
#include <QTextStream>

DatabaseService::DatabaseService(QObject *parent)
    : IDatabaseService(parent)
    , m_itemModel(nullptr)
{
    //  Creat database if needded
    QString dbName = "ois_inspection.db";
    if (!this->initDatabase(dbName)) {
        qCritical() << "CRITICAL! DatabaseService: Failed to start the database!";
    }
}

DatabaseService::~DatabaseService()
{
    if (m_db.isOpen()) m_db.close();
}

IDbModel* DatabaseService::itemModel() const
{
    return m_itemModel;
}

IDbModel* DatabaseService::model(const QString &name) const
{
    return m_modelsMap[name];
}

bool DatabaseService::logNewDefect(int boardId,
                                   int typeId,
                                   const QString &designator,
                                   double x,
                                   double y)
{
    DbModel *defectModel = m_modelsMap.value("defects");
    if (!defectModel) {
        return false;
        qCritical()
            << "WARNING!!! DatabaseService: defectModel ="
            << defectModel;
    }

    auto sqlModel = qobject_cast<QSqlRelationalTableModel*>(defectModel->dbModel());
    if (!sqlModel) {
        return false;
        qCritical()
            << "WARNING!!! DatabaseService: sqlModel ="
            << sqlModel;
    }

    QVariant trueTypeId;
    // Защитная проверка: существует ли вообще такой typeId в справочнике ГОСТ?
    // Это предотвратит падение базы, если алгоритм обработки изображений пришлет некорректный ID дефекта.
    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT 1 FROM defect_types WHERE defect_type_id = :id");
    checkQuery.bindValue(":id", typeId);
    if (!checkQuery.exec() || !checkQuery.next()) {
        qCritical()
            << "WARNING!!! DatabaseService: Попытка записать дефект с несуществующим defect_type_id:"
            << typeId;
        return false;
    } else {
        trueTypeId = checkQuery.boundValue(":id");
        qDebug()
            << "DatabaseService: trueTypeId ="
            << trueTypeId;
    }

    int newRow = sqlModel->rowCount();
    if (!sqlModel->insertRow(newRow)) {
        return false;
        qCritical()
            << "WARNING!!! DatabaseService: sqlModel->insertRow(newRow) ="
            << sqlModel->insertRow(newRow);
    }

    // 🌟 ИСПРАВЛЕНИЕ 2: Для реляционных полей (колонка 2) в Qt
    // безопаснее и правильнее использовать перегрузку setData, передавая QVariant(typeId).
    // Это заставит внутренний драйвер Qt SQL правильно сопоставить числовой ключ с внешней таблицей.
    sqlModel->setData(sqlModel->index(newRow, 1), QVariant(boardId));
    // defect_type_id (REFERENCES defect_types)
    sqlModel->setData(sqlModel->index(newRow, 2), trueTypeId);
    sqlModel->setData(sqlModel->index(newRow, 3), designator);
    sqlModel->setData(sqlModel->index(newRow, 4), x);
    sqlModel->setData(sqlModel->index(newRow, 5), y);
    sqlModel->setData(sqlModel->index(newRow, 6), "pending");

    if (!sqlModel->submitAll()) {
        // qCritical()
        //     << "WARNING!!! DatabaseService: sqlModel->submitAll() ="
        //     << sqlModel->submitAll()
        //     << "WARNING!!! DatabaseService: Ошибка при физической записи дефекта в SQLite:"
        //     << sqlModel->lastError().text();

        // Откатываем незаписанную строку в кэше, чтобы модель не оставалась в испорченном состоянии
        sqlModel->revertAll();
        return false;
    }

    emit defectAdded();
    return true;
}

const QStringList DatabaseService::availableTables() const
{
    QStringList tables{};

    for (const QString &tableName : m_db.tables(QSql::Tables)) {
        if (tableName.startsWith("sqlite_", Qt::CaseInsensitive)) continue;
        if (tableName == "session_cameras") continue;

        tables.append(tableName);
    }
    return tables;
}

bool DatabaseService::initDatabase(const QString &dbName)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    //  Construct the full path to the database file
    QString dbPath = dir().absoluteFilePath(dbName);
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qCritical()
            << "CRITICAL! DatabaseService: Error opening database"
            << m_db.lastError().text();
        return false;
    }

    //  Enable Foreign Keys support
    QSqlQuery query(m_db);
    if (!query.exec("PRAGMA foreign_keys = ON;")) {
        qCritical()
            << "CRITICAL! DatabaseService: Failed to enable Foreign Key support: "
            << query.lastError().text();
    }

    QString nameTable = "operators";
    this->creatTables();
    this->creatModel(nameTable);
    this->populateModelsMap();

    return true;
}

//  Creat the tables if it does not already exist
bool DatabaseService::creatTables()
{
    QSqlQuery query(m_db);

    //  Computers table
    QString createComputers = "CREATE TABLE IF NOT EXISTS computers ("
                              "computer_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "computer_name TEXT NOT NULL UNIQUE, "
                              "os_info TEXT, "
                              "mac_address TEXT NOT NULL UNIQUE"
                              ");";

    if (!query.exec(createComputers))
    {
        qCritical()
            << "CRITICAL! DatabaseService: Error creating Computers table"
            << query.lastError().text();
        return false;
    }

    //  Operators table
    QString createOperators = "CREATE TABLE IF NOT EXISTS operators ("
                              "operator_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "username TEXT NOT NULL UNIQUE, "
                              "full_name TEXT NOT NULL, "
                              "password TEXT NOT NULL"
                              ");";

    if (!query.exec(createOperators))
    {
        qCritical()
            << "CRITICAL! DatabaseService: Error creating Operators table"
            << query.lastError().text();
        return false;
    }

    //  Operators table
    QString createCameras = "CREATE TABLE IF NOT EXISTS cameras ("
                            "camera_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "name TEXT NOT NULL UNIQUE, "
                            "type TEXT CHECK(type IN ('USB', 'IP')), "
                            "usb_device_index INTEGER, "
                            "ip_rtsp_url TEXT, "
                            "resolution_w INTEGER DEFAULT 1920, "
                            "resolution_h INTEGER DEFAULT 1080, "
                            "is_available INTEGER CHECK(is_available IN (0, 1)), "
                            "last_used_at DATETIME"
                            ");";

    if (!query.exec(createCameras))
    {
        qCritical()
            << "CRITICAL! DatabaseService: Error creating Cameras table"
            << query.lastError().text();
        return false;
    }

    //  Sessions tables
    QString createSession = "CREATE TABLE IF NOT EXISTS sessions ("
                            "sessions_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "operator_id INTEGER NOT NULL, "
                            "computer_id TEXT NOT NULL, "
                            "started_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
                            "finished_at DATETIME, "
                            "FOREIGN KEY (operator_id) REFERENCES operators(operator_id), "
                            "FOREIGN KEY (computer_id) REFERENCES computers(computer_id)"
                            ");";

    if (!query.exec(createSession))
    {
        qCritical()
            << "CRITICAL! DatabaseService: Error creating Sessions table"
            << query.lastError().text();
        return false;
    }

    //  SessionCameras tables
    QString createSessionCameras = "CREATE TABLE IF NOT EXISTS session_cameras ("
                                   "session_id INTEGER, "
                                   "camera_id INTEGER, "
                                   "attached_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
                                   "FOREIGN KEY (session_id) REFERENCES sessions(session_id) ON DELETE CASCADE, "
                                   "FOREIGN KEY (camera_id) REFERENCES cameras(camera_id) ON DELETE CASCADE"
                                   ");";

    if (!query.exec(createSessionCameras))
    {
        qCritical()
            << "CRITICAL! DatabaseService: Error creating SessionCameras table"
            << query.lastError().text();
        return false;
    }

    //  DefectTypes tables
    QString createDefectTypes = "CREATE TABLE IF NOT EXISTS defect_types ("
                                "defect_type_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "gost_code TEXT NOT NULL UNIQUE, "
                                "name TEXT NOT NULL, "
                                "description TEXT"
                                ");";

    if (!query.exec(createDefectTypes))
    {
        qCritical()
            << "CRITICAL! DatabaseService: Error creating DefectTypes table"
            << query.lastError().text();
        return false;
    }

    //  Boards tables
    QString createBoards = "CREATE TABLE IF NOT EXISTS boards ("
                           "board_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "session_id INTEGER NOT NULL, "
                           "serial_number TEXT NOT NULL UNIQUE, "
                           "scanned_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
                           "status TEXT CHECK(status IN ('PASS', 'FAIL', 'REWORK')) DEFAULT 'PASS', "
                           "FOREIGN KEY (session_id) REFERENCES sessions(session_id)"
                           ");";

    if (!query.exec(createBoards))
    {
        qCritical()
            << "CRITICAL! DatabaseService: Error creating Boards table"
            << query.lastError().text();
        return false;
    }

    //  Defects tables
    QString createDefects = "CREATE TABLE IF NOT EXISTS defects ("
                            "defect_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "board_id INTEGER NOT NULL, "
                            "defect_type_id INTEGER NOT NULL, "
                            "component_designator TEXT NOT NULL, "
                            "coordinate_x REAL NOT NULL, "
                            "coordinate_y REAL NOT NULL, "
                            "verification_status TEXT CHECK(verification_status IN ('pending', 'confirmed', 'false_alarm')) DEFAULT 'pending', "
                            "FOREIGN KEY (board_id) REFERENCES boards(board_id) ON DELETE CASCADE, "
                            "FOREIGN KEY (defect_type_id) REFERENCES defect_types(defect_type_id)"
                            ");";

    if (!query.exec(createDefects))
    {
        qCritical()
            << "CRITICAL! DatabaseService: Error creating Defects table"
            << query.lastError().text();
        return false;
    }

    //  Table of images and reference samples
    QString createImages = "CREATE TABLE IF NOT EXISTS images ("
                           "image_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "board_id INTEGER NOT NULL, "
                           "defect_id INTEGER, "
                           "camera_id INTEGER NOT NULL, "
                           "image_type TEXT CHECK(image_type IN ('golden_reference', 'defect_crop', 'full_board')), "
                           "file_path TEXT NOT NULL, "
                           "coordinate_y REAL NOT NULL, "
                           "FOREIGN KEY (board_id) REFERENCES boards(board_id) ON DELETE CASCADE, "
                           "FOREIGN KEY (defect_id) REFERENCES defects(defect_id) ON DELETE CASCADE, "
                           "FOREIGN KEY (camera_id) REFERENCES cameras(camera_id) ON DELETE SET NULL"
                           ");";

    if (!query.exec(createImages))
    {
        qCritical()
            << "CRITICAL! DatabaseService: Error creating Images table"
            << query.lastError().text();
        return false;
    }

    insertDefaultDataIfNeeded();

    return true;
}

bool DatabaseService::insertDefaultDataIfNeeded()
{
    QSqlQuery query(m_db);

    //  Check for and add an default administrator if the table is empty
    query.exec("SELECT COUNT(*) FROM operators");
    if (query.next() && query.value(0).toInt() == 0) {
        query.prepare("INSERT INTO operators (username, full_name, password)"
                      "VALUES (:user, :name, :password)");

        query.bindValue(":user", "admin");
        query.bindValue(":name", "default_full_name");
        query.bindValue(":password", "admin");

        if (query.exec()) {
            qInfo() << "DatabaseService: Add default operators 'admin'";
        } else {
            qCritical()
                << "CRITICAL! DatabaseService: ERROR inserting into the operators table."
                << query.lastError().text();
        }
    }

    //  Check for and add a defects directory according to GOST if the directory is empty
    query.exec("SELECT COUNT(*) FROM defect_types");
    if (query.next() && query.value(0).toInt() == 0) {
        query.prepare("INSERT INTO defect_types (gost_code, name, description)"
                      "VALUES (:code, :name, :desc)");

        query.bindValue(":code", "ГОСТ-Р-5481-КЗ");
        query.bindValue(":name", "Короткое замыкание (Перемычка)");
        query.bindValue(":desc", "Посадочное место на плате пусто, компонент потерян или не установлен.");

        if (query.exec()) {
            qInfo() << "DatabaseService: The baseline defect reference, compiled on GOST has been successfully populated.";
        } else {
            qCritical()
                << "CRITICAL! DatabaseService: ERROR inserting into the defect_types table."
                << query.lastError().text();
        }
    }

    //  Check for the computer and add its details
    //  Получение сетевого имени ПК
    QString computerName = QHostInfo::localHostName();

    //  Готовим запрос с плейсхолдером
    query.prepare("SELECT computer_name FROM computers WHERE computer_name = :computerName LIMIT 1");

    //  Привязываем переменную
    query.bindValue(":computerName", computerName);

    if (query.exec()) {
        if (!query.next()) {
            //  Запрос успешный, но такого компьютера нет (или таблица пуста)
            //  Получение данных об операционной системе
            QString osPrettyName = QSysInfo::prettyProductName();

            //  Получение физического (MAC) адреса активной сетевой карты
            QString macAddress = "Not found";
            const auto interfaces = QNetworkInterface::allInterfaces();
            for (const QNetworkInterface &interface : interfaces) {
                // Отсекаем петлевой интерфейс (localhost) и неактивные карты
                if (interface.flags().testFlag(QNetworkInterface::IsLoopBack) ||
                    !interface.flags().testFlag(QNetworkInterface::IsUp)) {
                    continue;
                }

                QString hardwareAddress = interface.hardwareAddress();
                if (!hardwareAddress.isEmpty()) {
                    macAddress = hardwareAddress;
                    break; // Берем первый попавшийся активный физический адрес
                }
            }

            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO computers (computer_name, os_info, mac_address)"
                                "VALUES (:name, :os_info, :mac)");

            insertQuery.bindValue(":name", computerName);
            insertQuery.bindValue(":os_info", osPrettyName);
            insertQuery.bindValue(":mac", macAddress);

            if (insertQuery.exec()) {
                qInfo() << "DatabaseService: Computer informations successfully added.";
            } else {
                qCritical()
                << "CRITICAL! DatabaseService: ERROR inserting into the computers table."
                << insertQuery.lastError().text();
            }
        }
    } else {
        qCritical()
            << "CRITICAL! DatabaseService: ERROR inserting into the computers table."
            << query.lastError().text();
    }

    return true;
}

bool DatabaseService::creatModel(const QString &nameTable)
{
    m_itemModel = new DbModel(this, m_db);
    m_itemModel->setTable(nameTable);   //  Указываем, какую таблицу читать
    m_itemModel->refreshData();         //  Делаем первый выбор данных (select)

    return true;
}

bool DatabaseService::populateModelsMap()
{
    QStringList tables = m_db.tables(QSql::Tables);
    m_modelsMap.clear();

    for (const QString &tableName : tables) {
        if (tableName.startsWith("sqlite_", Qt::CaseInsensitive)) continue;
        if (tableName == "session_cameras") continue;

        DbModel *model = new DbModel(this, m_db);
        model->setParent(this);
        model->setTable(tableName);

        m_modelsMap.insert(tableName, model);
    }
    return true;
}

QDir DatabaseService::dir()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return dir;
}

bool DatabaseService::logNewComputer()
{
    // 1. Получение сетевого имени ПК
    QString computerName = QHostInfo::localHostName();
    qDebug() << computerName;

    // 2. Получение данных об операционной системе
    QString osPrettyName = QSysInfo::prettyProductName(); // Полное красивое имя ОС
    qDebug() << osPrettyName;

    // 3. Получение физического (MAC) адреса активной сетевой карты
    QString macAddress = "Not found";
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : interfaces) {
        // Отсекаем петлевой интерфейс (localhost) и неактивные карты
        if (interface.flags().testFlag(QNetworkInterface::IsLoopBack) ||
            !interface.flags().testFlag(QNetworkInterface::IsUp)) {
            continue;
        }

        QString hardwareAddress = interface.hardwareAddress();
        if (!hardwareAddress.isEmpty()) {
            macAddress = hardwareAddress;
            qDebug() << macAddress;
            break; // Берем первый попавшийся активный физический адрес
        }
    }

    return true;
}

//  Factory method
QSharedPointer<IDatabaseService> createDatabaseService(QObject *parent)
{
    return QSharedPointer<IDatabaseService>(new DatabaseService(parent));
}
