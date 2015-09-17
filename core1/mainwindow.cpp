#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_tcpServer=new QTcpServer(this);
    m_tcpServer->listen(QHostAddress::Any,31300);
    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(slotSend()));
}

void MainWindow::slotSend()
{
    m_tcpSocket=m_tcpServer->nextPendingConnection();
    connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(slotReadMessage()));
}

void MainWindow::slotReadMessage()
{
    int decide=0;//默认账号不存在
    QDataStream in(m_tcpSocket);
    QString name,passwd;
    bool signorlog,judge=true;
    in>>signorlog>>name>>passwd;
    QString sname="\""+name+"\"",scode="\""+passwd+"\"";
    //首先检查用户名是否已存在于数据库不存在返回true并且保存数据已存在若相同返回true不相同返回false
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.sqlite");
    if(!db.open())
    {
        qDebug()<<"open sql fail!";
        QMessageBox::warning(this,tr("提示"),tr("无法打开数据库"),QMessageBox::Ok);
        decide=4;
    }
    else
    {
        QSqlQuery query;
        QString sql="create table if not exists client(id varchar(50) primary key,password varchar(50),score integer,state bool)";
        query.exec(sql);
        sql="select * from client";
        query.exec(sql);
        while(query.next())//实现查询
        {
            if(query.value(0).toString()==name&&signorlog==false)
            {
                judge=false;
                if(query.value(1).toString()==passwd)
                {
                    if(query.value(3).toBool()==true)
                        decide=2;//账号密码匹配，但是有用户已经登录
                    else
                    {
                        decide=1;//账号密码匹配，但是没有用户登录
                        sql=QString("update client set state=1 where id=%1;").arg(sname);
                        query.exec(sql);
                    }
                }
                else
                    decide=3;//账号密码不匹配
            }
        }
        if(judge&&signorlog==true)
        {
            sql=QString("insert into client(id,password,score,state) values(%1,%2,0,1)").arg(sname).arg(scode);
            query.exec(sql);
        }
        db.close();
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out<<decide;
    m_tcpSocket->write(block);
}

MainWindow::~MainWindow()
{
    delete ui;
}
