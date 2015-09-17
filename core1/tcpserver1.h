#ifndef TCPSERVER1_H
#define TCPSERVER1_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "clienttcpsocket1.h"

class TCPserver1 : public QTcpServer
{
    Q_OBJECT
public:
    explicit TCPserver1(QObject *parent = 0);
    TCPserver1(QObject *parent = 0,int port = 0);
    QList<clientTCPsocket1*>clientTCPsocketList;
    virtual void incomingConnection(int descriptor);

signals:
  void updateS(QString,int);

public slots:
  void updateC(QString msg, int length);
  void slotD(int descriptor);

};

#endif // TCPSERVER1_H
