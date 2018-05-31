#include <QtGui>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QFile>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include "csql_db.h"

DatabaseManager::DatabaseManager()
{
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

bool DatabaseManager::closeDB()
{
    if (db.isOpen())
    {
        db.close();
    }
}

#if 0
bool DatabaseManager::openDB()
{
    // Find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");

#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
#else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName("my.db");
#endif

    // Open databasee
    return db.open();
}
#endif

QSqlError DatabaseManager::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();
}

bool DatabaseManager::deleteDB()
{
    // Close database
    db.close();

#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
#else

    // Remove created database binary file
    return QFile::remove("my.db");
#endif
}

int DatabaseManager::insertRecord(int mindex, QString recordnumber, QString timestamp)
{
    int newId = -1;
    bool ret = false;

    if (db.isOpen())
    {
        //http://www.sqlite.org/autoinc.html
        // NULL = is the keyword for the autoincrement to generate next value

        QSqlQuery query;
        ret = query.exec(QString("insert into record values(NULL,%1,'%2','%3')")
                         .arg(mindex).arg(recordnumber).arg(timestamp));

        // Get database given autoincrement value
        if (ret)
        {
            // http://www.sqlite.org/c3ref/last_insert_rowid.html
            newId = query.lastInsertId().toInt();
        }

    }
    return newId;
}

bool DatabaseManager::createRecordTable()
{
    // Create table "record"
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec("create table record "
                         "(id integer primary key, "
                         "mindex integer, "
                         "recordnumber varchar(20), "
                         "timestamp varchar(30)) ");

    }
    return ret;
}

#if 1
bool DatabaseManager::openDB()
{
    // Find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");

#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
#else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName("my.db");
#endif

    // Open databasee
    return db.open();
}
#endif

bool DatabaseManager::getRecord(int id, RecordData *&record)
{
    bool ret = false;

    QSqlQuery query(QString("select * from record where id = %1").arg(id));
    if (query.next())
    {
        record->id           = query.value(0).toInt();
        record->mindex       = query.value(1).toInt();
        record->recordnumber = query.value(2).toString();
        record->timestamp    = query.value(3).toString();
        ret = true;
    }

    return ret;
}

bool DatabaseManager::deleteRecord(int id)
{
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec(QString("delete from record where id=%1").arg(id));
    }
    return ret;
}

#if 0
QString MyWidget::searchPersonName(int personId)
{
    QString name;
    QSqlTableModel *model = new QSqlTableModel;
    // Set used table
    model->setTable("record");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // Set where clause
    model->setFilter(QString("id=%1").arg(personId));
    model->select();

    //The index of the record we are interested in
    int index = 0;

    // Read result
    QSqlRecord record = model->record(index);
    if (!record.isEmpty())
        name = record.value(2).toString();
    return name;
}
#endif

