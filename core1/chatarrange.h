#ifndef CHATARRANGE_H
#define CHATARRANGE_H

#include <QWidget>
#include "chatwidget.h"

namespace Ui {
    class chatarrange;
}

class chatarrange : public QWidget
{
    Q_OBJECT

public:
    explicit chatarrange(QWidget *parent = 0);
    ~chatarrange();

private slots:
    void on_pushButton_clicked();

private:
    Ui::chatarrange *ui;
    chatWidget* w[30];
};

#endif // CHATARRANGE_H
