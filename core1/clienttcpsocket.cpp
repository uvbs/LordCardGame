#include "clienttcpsocket.h"

clientTCPsocket::clientTCPsocket(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

void clientTCPsocket::dataReceived()
{
    QDataStream in(this);
    QString m_name,n;
    int roomNo;
    bool decide;
    int inorout;
    in>>m_name>>roomNo>>decide>>inorout>>n;
    if(inorout==1&&roomNo==0&&!decide)
    {
        int nu=TCPserver::clientTCPsocketList.count()-1;
        emit updateClients(m_name,nu);//记录第一次信息
    }
    if(inorout==1&&roomNo!=0)
    {
        emit decidedata(m_name,roomNo,decide);
    }
    if(inorout==0&&roomNo!=0&&!decide)
    {
        emit decidedata(m_name,roomNo,decide);
    }
    if(inorout==0&&roomNo!=0&&decide)
    {
        emit exitroom(m_name,roomNo);
    }
}

void clientTCPsocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}
