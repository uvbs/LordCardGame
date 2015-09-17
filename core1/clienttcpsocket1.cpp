#include "clienttcpsocket1.h"

clientTCPsocket1::clientTCPsocket1(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataR()));
    connect(this,SIGNAL(disconnected(int)),this,SLOT(slotD()));
}

void clientTCPsocket1::dataR()
{
    while(this->bytesAvailable()>0)
    {
        QString msg = this->readAll();
        int length = msg.length();
        emit updateC(msg,length);
    }
}

void clientTCPsocket1::slotD()
{
    emit disconnected(this->socketDescriptor());
}

