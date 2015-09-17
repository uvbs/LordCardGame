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
    int decide=0;//Ĭ���˺Ų�����
    QDataStream in(m_tcpSocket);
    QString name,passwd;
    bool signorlog,judge=true;
    in>>signorlog>>name>>passwd;
    QString sname="\""+name+"\"",scode="\""+passwd+"\"";
    //���ȼ���û����Ƿ��Ѵ��������ݿⲻ���ڷ���true���ұ��������Ѵ�������ͬ����true����ͬ����false
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.sqlite");
    if(!db.open())
    {
        qDebug()<<"open sql fail!";
        QMessageBox::warning(this,tr("��ʾ"),tr("�޷������ݿ�"),QMessageBox::Ok);
        decide=4;
    }
    else
    {
        QSqlQuery query;
        QString sql="create table if not exists client(id varchar(50) primary key,password varchar(50),score integer,state bool)";
        query.exec(sql);
        sql="select * from client";
        query.exec(sql);
        while(query.next())//ʵ�ֲ�ѯ
        {
            if(query.value(0).toString()==name&&signorlog==false)
            {
                judge=false;
                if(query.value(1).toString()==passwd)
                {
                    if(query.value(3).toBool()==true)
                        decide=2;//�˺�����ƥ�䣬�������û��Ѿ���¼
                    else
                    {
                        decide=1;//�˺�����ƥ�䣬����û���û���¼
                        sql=QString("update client set state=1 where id=%1;").arg(sname);
                        query.exec(sql);
                    }
                }
                else
                    decide=3;//�˺����벻ƥ��
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
