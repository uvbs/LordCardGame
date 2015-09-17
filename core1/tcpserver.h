#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork>
#include <QMessageBox>
#include <Qtsql>
#include "clienttcpsocket.h"
class clientTCPsocket;


class TCPserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit TCPserver(QObject *parent = 0);
    static QList<clientTCPsocket*>clientTCPsocketList;
    static QList<QString*>clientname;
    static int room[30][3];
    virtual void incomingConnection(int descriptor);
public slots:
    void senddata(QString,int,bool);
    void slotDisconnected(int descriptor);
    void updateClien(QString msg, int length);
    void exitr(QString,int);

private:

};

#endif // TCPSERVER_H
