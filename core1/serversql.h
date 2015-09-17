#ifndef SERVERSQL_H
#define SERVERSQL_H
#include <QMessageBox>
#include <QObject>
#include <QSqlDatabase>
#include <QtSql>
class serversql
{
public:
    serversql();
    static QSqlDatabase* db;
    static void initdatabase();
};

#endif // SERVERSQL_H
