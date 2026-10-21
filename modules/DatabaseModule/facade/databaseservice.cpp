#include "databaseservice.h"
#include <QStandardPaths>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHostInfo>
#include <QSysInfo>
#include <QNetworkInterface>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <systemenvironment.h>
#include "sqlquerybuilder.h"
#include "userrepository.h"
#include "sessionrepository.h"

DatabaseService::DatabaseService(QObject *parent)
    : IDatabaseService(parent)
{
    //  Creat database if needded
    QString dbName = "ois_inspection.db";
    if (!this->initDatabase(dbName)) {
        qCritical() << "CRITICAL! DatabaseService: Failed to start the database!";
    }

    m_sysEnvironment = new SystemEnvironment(this);

    //  Создаем QueryBuilder, который берет на себя рутину SQL-запросов.
    m_queryBuilder = new SqlQueryBuilder(this);

    //  Передаем ему ссылку на QueryBuilder (Внедрение зависимости / DI)
    m_userRepo = new UserRepository(m_queryBuilder, this);
    m_sessionRepo = new SessionRepository(m_sysEnvironment, m_queryBuilder, this);

}

DatabaseService::~DatabaseService()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (db.isOpen()) { db.close(); }
}

QAbstractTableModel* DatabaseService::abstractTableModel(const QString &tableName)
{
    if (m_relationalModelsMap.contains(tableName)) {
        return m_relationalModelsMap.value(tableName);
    }

    QSqlDatabase db = QSqlDatabase::database();
    DbRelationalTableModel *relationalTableModel = new DbRelationalTableModel(this, db);
    relationalTableModel->setTable(tableName);
    this->autoPopulateRelations(relationalTableModel, "name");

    if (!relationalTableModel->select()) {
        qCritical() << "WARNING! DatabaseService: The model is not being populated with data from the table";
    }
    m_relationalModelsMap.insert(tableName, relationalTableModel);
    if (!m_relationalModelsMap.contains(tableName)) {
        qCritical() << "WARNING! DatabaseService: The model is not being added to the model map";
    }
    return relationalTableModel;
}

const Core::AuthResult DatabaseService::authenticate(const QString &username,
                                                     const QString &password,
                                                     const QString &message)
{
    Core::AuthResult result;
    QString tableName = "operators";

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    QString sql = QString("SELECT * FROM %1 WHERE username = %2 LIMIT 1")
                      .arg(tableName,
                           ":username");

    if (!query.prepare(sql)) {
        //QString errorDetails = this->handleDatabaseError(query.lastError(), message);
        //result.error = errorDetails;
        qWarning()
            << "DatabaseService: prepare INSERT failed:"
            << result.error;
        result.success = false;

        return result;
    }

    query.bindValue(":username", username);

    if (!query.exec()) {
        //QString errorDetails = this->handleDatabaseError(query.lastError(), message);
        result.error = query.lastError().text();
        qWarning()
            << "DatabaseService: INSERT failed:"
            << result.error;
        result.success = false;

        return result;
    }

    // Читаем результаты
    while (query.next()) {
        QString storedHash = query.value("password_hash").toString();
        QString salt = query.value("salt").toString();

        // 2. Хэшируем введенный пароль с солью из БД (используем SHA-256)
        QByteArray inputData = (password + salt).toUtf8();
        QByteArray calculatedHash = QCryptographicHash::hash(inputData, QCryptographicHash::Sha256).toHex();

        if (!(storedHash == calculatedHash)) {
            result.success = false;
            result.error = "Incorrect password";
            qWarning()
                << "DatabaseService: Log in to the app:"
                << result.error;

            return result;
        }

        result.operatorId = query.value("operator_id").toInt();
        result.username = query.value("username").toString();
        result.fullName = query.value("full_name").toString();
        result.success = true;
    }

    return result;
}

const Core::DbOperationResult DatabaseService::creatUser(const QString &username,
                                                         const QString &password,
                                                         const QString &fullName,
                                                         const QString &jobTitle,
                                                         const QString &message)
{
    return m_userRepo->createUser(username, password, fullName, jobTitle, message);

    // if (password.isEmpty()) {
    //     QString userMessage = QString("Не удалось выполнить операцию: %1.\n\n").arg(message);
    //     Core::DbOperationResult result;
    //     result.error = "Password is empty!";
    //     return result;
    // }
    // // Генерация случайной соли (например, 16 символов)
    // QString salt = QString::number(QRandomGenerator::global()->generate64(), 36);
    // // Создание хэша
    // QByteArray hashPassword = QCryptographicHash::hash((password + salt).toUtf8(),
    //                                                    QCryptographicHash::Sha256).toHex();

    // Core::DbRecord record;
    // record.insert("username", username);
    // record.insert("full_name", fullName);
    // record.insert("password_hash", hashPassword);
    // record.insert("salt", salt);

    // Core::DbOperationResult result = this->insertRecord("operators", record, message);

    // return result;
}

const Core::DbOperationResult DatabaseService::creatCamera(const QString &address,
                                                           const QString &message)
{
    Core::DbRecord record;

    Core::DbOperationResult result = insertRecord("cameras", record, message);

    return result;
}

const Core::SessionContext DatabaseService::creatSession(Core::AuthResult authResult)
{
    return m_sessionRepo->creatSession(authResult);
}

const Core::SessionContext DatabaseService::finishSession(Core::SessionContext &context)
{
    qDebug() << "DatabaseService: context.sessionId =" << context.sessionId << "\n"
             << "context.active =" << context.active;
    return m_sessionRepo->finishSession(context);
}

const QStringList DatabaseService::availableTables() const
{
    QStringList tables{};
    QSqlDatabase db = QSqlDatabase::database();

    for (const QString &tableName : db.tables(QSql::Tables)) {
        if (tableName.startsWith("sqlite_", Qt::CaseInsensitive)) continue;
        if (tableName == "session_cameras") continue;

        tables.append(tableName);
    }
    return tables;
}

bool DatabaseService::initDatabase(const QString &dbName)
{
    QSqlDatabase db = QSqlDatabase::database();
    db = QSqlDatabase::addDatabase("QSQLITE");

    //  Construct the full path to the database file
    QString dbPath = dir().absoluteFilePath(dbName);
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qCritical() << "CRITICAL! DatabaseService: Error opening database"
                    << db.lastError().text();
        return false;
    }

    //  Enable Foreign Keys support
    QSqlQuery query(db);
    if (!query.exec("PRAGMA foreign_keys = ON;")) {
        qCritical() << "CRITICAL! DatabaseService: Failed to enable Foreign Key support: "
                    << query.lastError().text();
    }

    QString nameTable = "operators";
    this->creatTables();

    return true;
}

//  Creat the tables if it does not already exist
bool DatabaseService::creatTables()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

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
                              "full_name TEXT NOT NULL UNIQUE, "
                              //"password TEXT NOT NULL, "
                              "password_hash TEXT NOT NULL, "
                              "salt TEXT NOT NULL"
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
                            "session_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "operator_id TEXT NOT NULL, "
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
                           //"serial_number TEXT NOT NULL UNIQUE, "
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
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    //  Check for and add an default administrator if the table is empty
    query.exec("SELECT COUNT(*) FROM operators");
    if (query.next() && query.value(0).toInt() == 0) {
        query.prepare("INSERT INTO operators (username, full_name, password_hash, salt)"
                      "VALUES (:user, :name, :hash, :salt)");

        QString defoltUsername = "admin";
        QString defoltFullName = "default_full_name";
        QString defoltPassword = "admin";
        // Генерация случайной соли (например, 16 символов)
        QString salt = QString::number(QRandomGenerator::global()->generate64(), 36);

        // Создание хэша
        QByteArray hashPassword = QCryptographicHash::hash((defoltPassword + salt).toUtf8(),
                                                           QCryptographicHash::Sha256).toHex();

        query.bindValue(":user", defoltUsername);
        query.bindValue(":name", defoltFullName);
        query.bindValue(":hash", hashPassword);
        query.bindValue(":salt", salt);

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
            QString macAddress = m_sysEnvironment->currentMacAddress();

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

const Core::DbOperationResult DatabaseService::insertRecord(const QString &tableName,
                                                            const Core::DbRecord &record,
                                                            const QString &message)
{
    Core::DbOperationResult result;

    if (tableName.isEmpty() || record.isEmpty()) {
        result.error = "Invalid database operation";
        result.success = false;
        return result;
    }

    //  проверить существование такой таблицы
    //  проверить FK, если требуется

    //  Реализовать метод keys() в Core::DbRecord!!!!!!!
    //  чтобы избавиться от следующей строки: QVariantMap << QVariantMap
    const QVariantMap &values = record.values();
    QStringList fields = values.keys();
    QStringList placeholders;

    for (const QString &field : fields) { placeholders << ":" + field; }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    const QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                            .arg(tableName,
                                 fields.join(", "),
                                 placeholders.join(", "));

    if (!query.prepare(sql)) {
        //QString errorDetails = this->handleDatabaseError(query.lastError(), message);
        //result.error = errorDetails;
        qWarning()
            << "DatabaseService: prepare INSERT failed:"
            << result.error;
        result.success = false;
        //emit dbExecutionError(error);

        return result;
    }

    for (const QString &field : fields) {
        query.bindValue(":" + field, values.value(field));
    }

    if (!query.exec()) {
        //QString errorDetails = this->handleDatabaseError(query.lastError(), message);
        result.error = query.lastError().text();
        qWarning()
            << "DatabaseService: INSERT failed:"
            << result.error;
        result.success = false;
        //emit dbExecutionError(error);

        return result;
    }

    //  emit apdateModel(tableName);
    result.success = true;
    result.insertedId = query.lastInsertId().toInt();
    return result;
}

void DatabaseService::autoPopulateRelations(QSqlRelationalTableModel *relationalTableModel, const QString &defaultDisplayField)
{
    QString displayField;
    displayField.replace(displayField, defaultDisplayField);

    QString tableName = relationalTableModel->tableName();
    QSqlRecord record = relationalTableModel->record();

    QSqlDatabase db = QSqlDatabase::database();
    //  Check whether the table contains foreign keys
    QSqlQuery query(QString("PRAGMA foreign_key_list(%1);").arg(tableName), db);
    //  Map foreign keys to the records required for display
    while (query.next()) {
        QString foreignTable = query.value("table").toString();
        if (foreignTable == "sessions") {
            displayField.replace(displayField, "started_at");
        }
        if (foreignTable == "boards") {
            displayField.replace(displayField, "serial_number");
        }
        if (foreignTable == "defects") {
            displayField.replace(displayField, "component_designator");
        }

        QString foreignColumn = query.value("to").toString();
        int localColumnIndex = record.indexOf(query.value("from").toString());

        if (localColumnIndex != -1) {
            relationalTableModel->setRelation(localColumnIndex, QSqlRelation(foreignTable, foreignColumn, displayField));
        }
    }
}



int DatabaseService::currentComputerId(const QString &macAddress)
{
    int computerId = -1;

    QSqlDatabase db;
    QSqlQuery query(db);

    QString sql = "SELECT computer_id FROM computers WHERE mac_address = :mac_address LIMIT 1";

    if (!query.prepare(sql)) {
        qCritical()
            << "DatabaseService: prepare SELECT failed:"
            << query.lastError().text();
    }

    query.bindValue(":mac_address", macAddress);

    if (!query.exec()) {
        qCritical()
            << "DatabaseService: prepare SELECT failed:"
            << query.lastError().text();
        return computerId;
    }

    if (!query.next()) {
        qCritical()
            << "DatabaseService: SELECT failed:"
            << query.lastError().text();
        return computerId;
    }

    computerId = query.value(0).toInt();
    return computerId;
}

const QString DatabaseService::currentMacAddress() const
{
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
    return macAddress;
}

QDir DatabaseService::dir()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return dir;
}

//  Factory method
QSharedPointer<IDatabaseService> createDatabaseService(QObject *parent)
{
    return QSharedPointer<IDatabaseService>(new DatabaseService(parent));
}
