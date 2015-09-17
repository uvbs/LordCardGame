#ifndef CLIENTTCPSOCKET_H
#define CLIENTTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "tcpserver.h"

class clientTCPsocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit clientTCPsocket(QObject *parent = 0);

signals:
    void updateClients(QString,int);
    void disconnected(int);
    void decidedata(QString,int,bool);
    void exitroom(QString,int);

public slots:
    void dataReceived();
    void slotDisconnected();
};

#endif // CLIENTTCPSOCKET_H
