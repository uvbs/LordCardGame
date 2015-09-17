#ifndef CHECKGUEST_H
#define CHECKGUEST_H

#include <QWidget>
#include <QTcpSocket>
#include <QtNetwork>
#include <QMessageBox>
#include <Qtsql>

class checkguest : public QWidget
{
    Q_OBJECT
public:
    explicit checkguest(QWidget *parent = 0);

private slots:
    void slotReadMessage();
    void slotSend();

private:
    QTcpSocket* m_tcpSocket;
    QTcpServer* m_tcpServer;

};

#endif // CHECKGUEST_H
