#include "serversql.h"
QSqlDatabase* serversql::db;
serversql::serversql()
{
}

void serversql::initdatabase()
{
   db=&QSqlDatabase::addDatabase("QSQLITE");
   db->setDatabaseName("server.sqlite");
    if(!db->open())
    {
       qDebug()<<"open sql fail!";
       QMessageBox::warning(NULL,QObject::tr("提示"),QObject::tr("无法打开数据库"),QMessageBox::Ok);
    }
}
