#ifndef TCPSERVER2_H
#define TCPSERVER2_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "tcpserver.h"
#include "cards.h"
#include <QTime>

class TCPserver2 : public QTcpServer
{
    Q_OBJECT
public:
    explicit TCPserver2(QObject *parent = 0);
    TCPserver2(int port = 0,QObject *parent = 0);
    QList<QTcpSocket*> tcpsocketlist;
    QList<QString*>clientname;
    QList<QTcpSocket*> tcp;

signals:

public slots:
    void accept();
    void read();
    void dis();
private:
    int beginornot;//是否开始
    int counter;//计数器
    int bet;//倍率
    int who1;//当前最新抢地主的人
    int now;//现在该谁是否抢/叫地主
    int who;//谁是地主
    int roomnumber;
    quint16 blocksize;
    Cards m_cards;
    Cards m_Cards[3];
    Cards m_extra;
};

#endif // TCPSERVER2_H
