#ifndef  _RECORD_DATA_H
#define  _RECORD_DATA_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>

class RecordData
{
public:
    int id;
    QString recordnumber;
    QString timestamp;
    int mindex;
};



class DatabaseManager : public QObject
{
public:
    DatabaseManager();
    ~DatabaseManager();
    bool openDB();
    bool closeDB();
    bool createRecordTable();
    bool getRecord(int id, RecordData *&record);
    int  insertRecord(int mindex, QString recordnumber, QString timestamp);
    bool deleteRecord(int id);

public:
    bool deleteDB();
    QSqlError lastError();

private:
    QSqlDatabase db;
};
#endif
