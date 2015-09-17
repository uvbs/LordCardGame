#include "chatwidget.h"
#include "ui_chatwidget.h"

chatWidget::chatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatWidget)
{
    ui->setupUi(this);
}

chatWidget::~chatWidget()
{
    delete ui;
}

chatWidget::chatWidget(int p,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatWidget)
{
    ui->setupUi(this);
    port=p;
    this->slotCreateS();
}

void chatWidget::slotCreateS()
{
    this->server = new TCPserver1(this,this->port);
    connect(this->server,SIGNAL(updateS(QString,int)),this,SLOT(updateS(QString,int)));
}

void chatWidget::updateS(QString msg, int length)
{
    ui->textBrowser->append(msg.left(length));
    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
}
