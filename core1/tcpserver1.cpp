#include "tcpserver1.h"

TCPserver1::TCPserver1(QObject *parent) :
    QTcpServer(parent)
{
}

TCPserver1::TCPserver1(QObject *parent,int port) :
    QTcpServer(parent)
{
    this->listen(QHostAddress::Any,port);
}

void TCPserver1::incomingConnection(int descriptor)
{
    clientTCPsocket1 *TCPsocket = new clientTCPsocket1(this);
    TCPsocket->setSocketDescriptor(descriptor);
    connect(TCPsocket,SIGNAL(updateC(QString,int)),this,SLOT(updateC(QString,int)));
    connect(TCPsocket,SIGNAL(disconnected(int)),this,SLOT(slotD(int)));
    this->clientTCPsocketList.append(TCPsocket);
}

void TCPserver1::updateC(QString msg, int length)
{
    emit updateS(msg,length);							  //
    for(int i=0;i<clientTCPsocketList.count();i++)
    {
        QTcpSocket *item = clientTCPsocketList.at(i);
        item->write(msg.toLocal8Bit(),2*length);
    }
}

void TCPserver1::slotD(int descriptor)
{
    for(int i=0;i<clientTCPsocketList.count();i++)
    {
        QTcpSocket *item = clientTCPsocketList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
            clientTCPsocketList.removeAt(i);
            return;
        }
    }
    return;
}
