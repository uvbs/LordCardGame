#include "chatarrange.h"
#include "ui_chatarrange.h"

chatarrange::chatarrange(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatarrange)
{
    ui->setupUi(this);
    int port=31301;
    for(int i=0;i<30;i++,port++)
        w[i]=new chatWidget(port);
}

chatarrange::~chatarrange()
{
    delete ui;
}

void chatarrange::on_pushButton_clicked()
{
    int nu=ui->comboBox->currentIndex();
    w[nu]->show();
}
