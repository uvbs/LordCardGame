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
       QMessageBox::warning(NULL,QObject::tr("��ʾ"),QObject::tr("�޷������ݿ�"),QMessageBox::Ok);
    }
}
