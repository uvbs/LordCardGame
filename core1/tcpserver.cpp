#include "tcpserver.h"

int TCPserver::room[30][3];
QList<clientTCPsocket*> TCPserver::clientTCPsocketList;
QList<QString*> TCPserver::clientname;

TCPserver::TCPserver(QObject *parent) :
    QTcpServer(parent)
{
    this->listen(QHostAddress::Any,31361);
    for(int i=0;i<30;i++)
        for(int j=0;j<3;j++)
           room[i][j]=-1;
}

void TCPserver::incomingConnection(int descriptor)
{
    clientTCPsocket *TCPsocket = new clientTCPsocket(this);
    TCPsocket->setSocketDescriptor(descriptor);
    connect(TCPsocket,SIGNAL(updateClients(QString,int)),this,SLOT(updateClien(QString,int)));
    connect(TCPsocket,SIGNAL(decidedata(QString,int,bool)),this,SLOT(senddata(QString,int,bool)));
    connect(TCPsocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
    connect(TCPsocket,SIGNAL(exitroom(QString,int)),this,SLOT(exitr(QString,int)));
    clientTCPsocketList.append(TCPsocket);
}

void TCPserver::exitr(QString n, int nu)
{
    nu--;
    for(int i=0;i<3;i++)
    {
        if(*clientname.at(room[nu][i])==n)
        {
            for(;i<3;i++)
            {
                if(i==2)
                {
                    room[nu][2]=-1;
                    break;
                }
                room[nu][i]=room[nu][i+1];
            }
        }
    }
}

void TCPserver::updateClien(QString msg,int nu)
{
    clientname.insert(nu,new QString(msg));
    //改变用户状态
    QString sname="\""+msg+"\"";
    QSqlQuery query;
    QString sql="create table if not exists client(id varchar(50) primary key,password varchar(50),score integer,state bool)";
    query.exec(sql);
    sql="select * from client";
    query.exec(sql);
    while(query.next())//实现查询
    {
        if(query.value(0).toString()==msg)
        {
            sql=QString("update client set state=1 where id=%1;").arg(sname);
            query.exec(sql);
        }
    }
}

void TCPserver::senddata(QString name, int nu , bool decide)
{
    if(!decide)//查看
    {
        nu--;
        int i;//看请求用户在QList中为第几个
        for(i=0;i<clientname.count();i++)
        {
            if(*clientname.at(i)==name)
                break;
        }
        int number=0,j;
        for(int i=0;i<3;i++)
        {
            if(room[nu][i]!=-1)
                number++;
        }//看究竟有几个用户在房间中
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<number;
        for(j=0;j<number;j++)
        {
            out<<*clientname[room[nu][j]];
            //积分待定
            QString sql;
            QSqlQuery query;
            sql="select * from client";
            query.exec(sql);
            while(query.next())//实现查询
            {
                if(query.value(0).toString()==*clientname[room[nu][j]])
                {
                    QString score=query.value(2).toString();
                    out<<score;
                }
            }
        }
        for(;j<3;j++)
        {
            out<<"";
        }
        clientTCPsocketList.at(i)->write(block);
    }
    if(decide)//请求进入
    {
        nu--;
        int i;//看请求用户在QList中为第几个
        for(i=0;i<clientname.count();i++)
        {
            if(*clientname.at(i)==name)
                break;
        }
        int number=0,j;
        for(int i=0;i<3;i++)
        {
            if(room[nu][i]!=-1)
                number++;
        }//看究竟有几个用户在房间中
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<number;
        for(j=0;j<number;j++)
        {
            out<<*clientname[room[nu][j]];
            QString sql;
            QSqlQuery query;
            sql="select * from client";
            query.exec(sql);
            while(query.next())//实现查询
            {
                if(query.value(0).toString()==*clientname[room[nu][j]])
                {
                    QString score=query.value(2).toString();
                    out<<score;
                }
            }
        }
        for(;j<3;j++)
        {
            out<<"";
        }
        clientTCPsocketList.at(i)->write(block);
        if(number>=0&&number<3)
        {
            room[nu][number]=i;
        }
        if(number==2)
        {
            QByteArray block1;
            int y=4;
            QDataStream out1(&block1,QIODevice::WriteOnly);
            out1<<y;
            for(j=0;j<3;j++)
            {
                out1<<""<<"";
            }
            clientTCPsocketList.at(room[nu][0])->write(block1);
            clientTCPsocketList.at(room[nu][1])->write(block1);
        }
    }
}

void TCPserver::slotDisconnected(int descriptor)
{
    for(int i =0;i<clientTCPsocketList.count();i++)
    {
        QTcpSocket *item = clientTCPsocketList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
            //数据库改变在线的属性,id:*clientname[i]
            QString sname="\""+*clientname[i]+"\"";
            QString sql;
            QSqlQuery query;
            sql="select * from client";
            query.exec(sql);
            while(query.next())//实现查询
            {
                if(query.value(0).toString()==*clientname[i])
                {
                    sql=QString("update client set state=0 where id=%1;").arg(sname);
                    query.exec(sql);
                }
            }
            clientTCPsocketList.removeAt(i);
            clientname.removeAt(i);
            for(int k=0;k<30;k++)
            {
                int j;
                for(j=0;j<3;j++)
                {
                    if(room[k][j]==i)
                    {
                        room[k][j]=-1;
                        break;
                    }
                }
                for(;j<3;j++)
                {
                    if(j==2)
                    {
                        room[k][j]=-1;
                        break;
                    }
                    room[k][j]=room[k][j+1];
                }
            }
            for(int m=0;m<30;m++)
            {
                for(int n=0;n<3;n++)
                {
                    if(room[m][n]>i)
                        room[m][n]--;
                }
            }
        }
    }
    return;
}
