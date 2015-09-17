#include <QtGui/QApplication>
#include "checkguest.h"
#include "tcpserver.h"
#include "chatarrange.h"
#include "gamearrange.h"
#include "serversql.h"
#include <QTextCodec>
#include <qDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    serversql::initdatabase();
    checkguest c;
    TCPserver t;
    chatarrange ca;
    gamearrange g;
    ca.show();
    return a.exec();
}
