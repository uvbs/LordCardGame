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
    int beginornot;//�Ƿ�ʼ
    int counter;//������
    int bet;//����
    int who1;//��ǰ��������������
    int now;//���ڸ�˭�Ƿ���/�е���
    int who;//˭�ǵ���
    int roomnumber;
    quint16 blocksize;
    Cards m_cards;
    Cards m_Cards[3];
    Cards m_extra;
};

#endif // TCPSERVER2_H
