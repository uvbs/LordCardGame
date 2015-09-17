#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QtNetwork>
#include <QMessageBox>
#include <Qtsql>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotReadMessage();
    void slotSend();

private:
    Ui::MainWindow *ui;
    QTcpSocket* m_tcpSocket;//套接字对象指针
    QTcpServer* m_tcpServer;
};

#endif // MAINWINDOW_H
