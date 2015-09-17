#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include "tcpserver1.h"
#include <QScrollbar>

namespace Ui {
    class chatWidget;
}

class chatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit chatWidget(QWidget *parent = 0);
    chatWidget(int p,QWidget *parent = 0);
    ~chatWidget();

private:
    Ui::chatWidget *ui;
    int port;
    TCPserver1 *server;
public slots:
    void slotCreateS();
    void updateS(QString msg, int length);
};

#endif // CHATWIDGET_H
