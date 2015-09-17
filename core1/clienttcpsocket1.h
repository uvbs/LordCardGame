#ifndef CLIENTTCPSOCKET1_H
#define CLIENTTCPSOCKET1_H

#include <QObject>
#include <QTcpSocket>

class clientTCPsocket1 : public QTcpSocket
{
    Q_OBJECT
public:
    explicit clientTCPsocket1(QObject *parent = 0);

signals:
  void updateC(QString,int);
  void disconnected(int);

public slots:
  void dataR();
  void slotD();

};

#endif // CLIENTTCPSOCKET1_H
