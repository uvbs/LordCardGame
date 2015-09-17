#include "tcpserver2.h"

TCPserver2::TCPserver2(QObject *parent) :
    QTcpServer(parent)
{
}

TCPserver2::TCPserver2(int port,QObject *parent) :
    QTcpServer(parent)
{
    counter=0;
    QTime time;
    time = QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    now=qrand()%3;
    beginornot=0;
    bet=1;
    who1=-1;
    who=-1;
    blocksize=0;
    this->listen(QHostAddress::Any,port);
    roomnumber=port-31330;
    connect(this,SIGNAL(newConnection()),this,SLOT(accept()));
}

void TCPserver2::accept()
{
    QTcpSocket* tcpsocket = this->nextPendingConnection();
    this->tcpsocketlist.append(tcpsocket);
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(read()));
    connect(tcpsocket,SIGNAL(disconnected()),this,SLOT(dis()));
}

void TCPserver2::read()
{
    QString m;
    QTcpSocket* m_tcpsocket=dynamic_cast<QTcpSocket*>(sender());
    if(m_tcpsocket==0)
        return;
    QDataStream in(m_tcpsocket);
    in.setVersion(QDataStream::Qt_4_0);//设置数据流版本
    if(blocksize==0)
    {
        if(m_tcpsocket->bytesAvailable()<(int)sizeof(quint16))
        {
            return;
        }
        in>>blocksize;
    }
    if(m_tcpsocket->bytesAvailable()<blocksize)
    {
        return;
    }
    in>>m;
    if(m=="name")
    {
        tcp.append(dynamic_cast<QTcpSocket*>(sender()));
        QString n;
        in>>n;
        QString* name=new QString(n);
        clientname.append(name);
        if(clientname.count()==3)
        {
            for(int i=0;i<3;i++)
            {
                for(int j=0;j<3;j++)
                {
                    if(*clientname.at(j)==*TCPserver::clientname.at(TCPserver::room[roomnumber-1][i]))
                    {
                        this->tcpsocketlist.append(tcp.at(j));
                    }
                }
            }
            tcpsocketlist.removeAt(2);
            tcpsocketlist.removeAt(1);
            tcpsocketlist.removeAt(0);
            tcp.clear();
            for(int i=0;i<3;i++)
            {
                *clientname.at(i)=*TCPserver::clientname.at(TCPserver::room[roomnumber-1][i]);
            }
            for(int i=0;i<3;i++)
            {
                QString o="allname";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<o;
                for(int j=0;j<3;j++)
                {
                    out<<*clientname.at(j);
                    QString sql;
                    QSqlQuery query;
                    sql="select * from client";
                    query.exec(sql);
                    while(query.next())//实现查询
                    {
                        if(query.value(0).toString()==*clientname.at(j))
                        {
                            QString score=query.value(2).toString();
                            out<<score;
                        }
                    }
                }
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(i)->write(block);
            }
        }
    }
    if(m=="ready")
    {
        beginornot++;
        if(beginornot==3)
        {
            m_cards.Clear();
            m_Cards[0].Clear();
            m_Cards[1].Clear();
            m_Cards[2].Clear();
            m_extra.Clear();
            for(int i=1;i<=13;i++)
            {
                for(int j=1;j<=4;j++)
                {
                    m_cards.Add(Card((CardPoint)i,(CardSuit)j));
                }
            }
            m_cards.Add(Card(Card_SJ,Suit_Begin));
            m_cards.Add(Card(Card_BJ,Suit_Begin));
            for(int i=0;i<17;i++)
                m_Cards[0].Add(m_cards.TakeRandomCard());
            for(int i=0;i<17;i++)
                m_Cards[1].Add(m_cards.TakeRandomCard());
            for(int i=0;i<17;i++)
                m_Cards[2].Add(m_cards.TakeRandomCard());
            for(int i=0;i<3;i++)
                m_extra.Add(m_cards.TakeRandomCard());
            for(int i=0;i<3;i++)
            {
                QString n="fapai";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<n;
                for(QSet<Card>::const_iterator it=m_Cards[i].m_cards.begin();it!=m_Cards[i].m_cards.end();it++)
                {
                    out<<qHash(*it);
                }
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(i)->write(block);
            }
        }
    }
    if(m=="ready1")
    {
        beginornot++;
        if(beginornot==3)
        {
            m_cards.Clear();
            m_Cards[0].Clear();
            m_Cards[1].Clear();
            m_Cards[2].Clear();
            m_extra.Clear();
            for(int i=1;i<=13;i++)
            {
                for(int j=1;j<=4;j++)
                {
                    m_cards.Add(Card((CardPoint)i,(CardSuit)j));
                }
            }
            m_cards.Add(Card(Card_SJ,Suit_Begin));
            m_cards.Add(Card(Card_BJ,Suit_Begin));
            for(int i=0;i<17;i++)
                m_Cards[0].Add(m_cards.TakeRandomCard());
            for(int i=0;i<17;i++)
                m_Cards[1].Add(m_cards.TakeRandomCard());
            for(int i=0;i<17;i++)
                m_Cards[2].Add(m_cards.TakeRandomCard());
            for(int i=0;i<3;i++)
                m_extra.Add(m_cards.TakeRandomCard());
            for(int i=0;i<3;i++)
            {
                QString n="fapai1";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<n;
                for(QSet<Card>::const_iterator it=m_Cards[i].m_cards.begin();it!=m_Cards[i].m_cards.end();it++)
                {
                    out<<qHash(*it);
                }
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(i)->write(block);
            }
        }
    }
    if(m=="fapaiover")
    {
        beginornot--;
        if(beginornot==0)
        {
            QString o="jiao";
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out<<(quint16)0;
            out<<o;
            out.device()->seek(0);
            out<<(quint16)(block.size()-sizeof(quint16));
            tcpsocketlist.at(now)->write(block);
            now=(now+1)%3;
            counter++;
        }
    }
    if(m=="jiao")
    {
        int decide;
        in>>decide;
        for(int i=0;i<3;i++)
        {
            QString o="whetherjiao";
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out<<(quint16)0;
            out<<o<<*clientname.at(((now+2)%3))<<decide;
            out.device()->seek(0);
            out<<(quint16)(block.size()-sizeof(quint16));
            tcpsocketlist.at(i)->write(block);
        }
        if(decide==0)
        {
            if(counter==3)
            {
                m_cards.Clear();
                m_Cards[0].Clear();
                m_Cards[1].Clear();
                m_Cards[2].Clear();
                m_extra.Clear();
                for(int i=1;i<=13;i++)
                {
                    for(int j=1;j<=4;j++)
                    {
                        m_cards.Add(Card((CardPoint)i,(CardSuit)j));
                    }
                }
                m_cards.Add(Card(Card_SJ,Suit_Begin));
                m_cards.Add(Card(Card_BJ,Suit_Begin));
                for(int i=0;i<17;i++)
                    m_Cards[0].Add(m_cards.TakeRandomCard());
                for(int i=0;i<17;i++)
                    m_Cards[1].Add(m_cards.TakeRandomCard());
                for(int i=0;i<17;i++)
                    m_Cards[2].Add(m_cards.TakeRandomCard());
                for(int i=0;i<3;i++)
                    m_extra.Add(m_cards.TakeRandomCard());
                for(int i=0;i<3;i++)
                {
                    QString n="chongxinfapai";
                    QByteArray block;
                    QDataStream out(&block,QIODevice::WriteOnly);
                    out<<(quint16)0;
                    out<<n;
                    for(QSet<Card>::const_iterator it=m_Cards[i].m_cards.begin();it!=m_Cards[i].m_cards.end();it++)
                    {
                        out<<qHash(*it);
                    }
                    out.device()->seek(0);
                    out<<(quint16)(block.size()-sizeof(quint16));
                    tcpsocketlist.at(i)->write(block);
                }
                counter=0;
                QTime time;
                time = QTime::currentTime();
                qsrand(time.msec()+time.second()*1000);
                now=qrand()%3;
                beginornot=3;
            }
            else
            {
                QString o="jiao";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<o;
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(now)->write(block);
                now=(now+1)%3;
                counter++;
            }
        }
        else if(decide==1)
        {
            who=(now+2)%3;
            counter=3-counter;
            if(counter!=0)
            {
                QString o="qiang";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<o;
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(now)->write(block);
                now=(now+1)%3;
                counter--;
            }
            else
            {
                for(int i=0;i<3;i++)
                {
                    QString o="dipai";
                    QByteArray block;
                    QDataStream out(&block,QIODevice::WriteOnly);
                    out<<(quint16)0;
                    out<<o<<*clientname.at(who);
                    for(QSet<Card>::const_iterator it=m_extra.m_cards.begin();it!=m_extra.m_cards.end();it++)
                    {
                        out<<qHash(*it);
                    }
                    m_Cards[who].Add(m_extra);
                    out.device()->seek(0);
                    out<<(quint16)(block.size()-sizeof(quint16));
                    tcpsocketlist.at(i)->write(block);
                }
                QString o="chupai";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<o;
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(who)->write(block);
            }
        }
    }
    if(m=="qiang")
    {
        int decide;
        in>>decide;
        for(int i=0;i<3;i++)
        {
            QString o="whetherqiang";
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out<<(quint16)0;
            out<<o<<*clientname.at((now+2)%3)<<decide;
            out.device()->seek(0);
            out<<(quint16)(block.size()-sizeof(quint16));
            tcpsocketlist.at(i)->write(block);
        }
        if(decide==0)
        {
            if(counter==0)
            {
                if(who1!=-1)
                {
                    QString o="qiang1";
                    QByteArray block;
                    QDataStream out(&block,QIODevice::WriteOnly);
                    out<<(quint16)0;
                    out<<o;
                    out.device()->seek(0);
                    out<<(quint16)(block.size()-sizeof(quint16));
                    tcpsocketlist.at(who)->write(block);
                }
                else
                {
                    for(int i=0;i<3;i++)
                    {
                        QString o="dipai";
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out<<(quint16)0;
                        out<<o<<*clientname.at(who);
                        for(QSet<Card>::const_iterator it=m_extra.m_cards.begin();it!=m_extra.m_cards.end();it++)
                        {
                            out<<qHash(*it);
                        }
                        m_Cards[who].Add(m_extra);
                        out.device()->seek(0);
                        out<<(quint16)(block.size()-sizeof(quint16));
                        tcpsocketlist.at(i)->write(block);
                    }
                    QString o="chupai";
                    QByteArray block;
                    QDataStream out(&block,QIODevice::WriteOnly);
                    out<<(quint16)0;
                    out<<o;
                    out.device()->seek(0);
                    out<<(quint16)(block.size()-sizeof(quint16));
                    tcpsocketlist.at(who)->write(block);
                }
            }
            else
            {
                QString o="qiang";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<o;
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(now)->write(block);
                now=(now+1)%3;
                counter--;
            }
        }
        else if(decide==1)
        {
            who1=(now+2)%3;
            if(counter!=0)
            {
                QString o="qiang";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<o;
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(now)->write(block);
                now=(now+1)%3;
                counter--;
            }
            else
            {
                QString o="qiang1";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<o;
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(who)->write(block);
            }
            bet*=2;
        }
    }
    if(m=="qiang1")
    {
        int decide;
        in>>decide;
        for(int i=0;i<3;i++)
        {
            QString o="whetherqiang";
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out<<(quint16)0;
            out<<o<<*clientname.at(who)<<decide;
            out.device()->seek(0);
            out<<(quint16)(block.size()-sizeof(quint16));
            tcpsocketlist.at(i)->write(block);
        }
        if(decide==0)
        {
            for(int i=0;i<3;i++)
            {
                QString o="dipai";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<o<<*clientname.at(who1);
                for(QSet<Card>::const_iterator it=m_extra.m_cards.begin();it!=m_extra.m_cards.end();it++)
                {
                    out<<qHash(*it);
                }
                m_Cards[who1].Add(m_extra);
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(i)->write(block);
            }
            who=who1;
            who1=-1;
            QString o="chupai";
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out<<(quint16)0;
            out<<o;
            out.device()->seek(0);
            out<<(quint16)(block.size()-sizeof(quint16));
            tcpsocketlist.at(who)->write(block);
        }
        else if(decide==1)
        {
            for(int i=0;i<3;i++)
            {
                QString o="dipai";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<o<<*clientname.at(who);
                for(QSet<Card>::const_iterator it=m_extra.m_cards.begin();it!=m_extra.m_cards.end();it++)
                {
                    out<<qHash(*it);
                }
                m_Cards[who].Add(m_extra);
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(i)->write(block);
            }
            bet*=2;
            QString o="chupai";
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out<<(quint16)0;
            out<<o;
            out.device()->seek(0);
            out<<(quint16)(block.size()-sizeof(quint16));
            tcpsocketlist.at(who)->write(block);
        }
    }
    if(m=="chupai")
    {
        QTcpSocket* m_tcpsocket=dynamic_cast<QTcpSocket*>(sender());
        for(int i=0;i<3;i++)
        {
            if(tcpsocketlist.at(i)==m_tcpsocket)
            {
                who1=i;
            }
        }
        int nu,w;
        in>>nu;
        uint* d=new uint[nu];
        for(int i=0;i<nu;i++)
        {
            in>>d[i];
        }
        in>>w;
        if(w==1)
            bet*=2;
        for(int i=0;i<3;i++)
        {
            QString o="chudepai";
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out<<(quint16)0;
            out<<o<<nu<<*clientname.at(who1);
            for(int j=0;j<nu;j++)
            {
                out<<d[j];
            }
            out.device()->seek(0);
            out<<(quint16)(block.size()-sizeof(quint16));
            tcpsocketlist.at(i)->write(block);
        }
        for(int i=0;i<nu;i++)
        {
            m_Cards[who1].Remove(Card(d[i]/100,d[i]%100));
        }
        if(m_Cards[who1].IsEmpty())
        {
            int score[3];
            int change[3];
            QString sc[3];
            QString ch[3];
            for(int j=0;j<3;j++)
            {
                QString sql;
                QSqlQuery query;
                sql="select * from client";
                query.exec(sql);
                while(query.next())
                {
                    if(query.value(0).toString()==*clientname.at(j))
                    {
                        score[j]=query.value(2).toInt();
                    }
                }
            }
            if(who1==who)
            {
                score[who]+=10*bet;
                score[(who+1)%3]-=5*bet;
                score[(who+2)%3]-=5*bet;
                change[who]=10*bet;
                change[(who+1)%3]=0-5*bet;
                change[(who+2)%3]=0-5*bet;
            }
            else
            {
                score[who]-=10*bet;
                score[(who+1)%3]+=5*bet;
                score[(who+2)%3]+=5*bet;
                change[who]=0-10*bet;
                change[(who+1)%3]=5*bet;
                change[(who+2)%3]=5*bet;
            }
            for(int j=0;j<3;j++)
            {
                QString sname="\""+*clientname[j]+"\"";
                QString sscore="\""+QString::number(score[j],10)+"\"";
                QString sql;
                QSqlQuery query;
                sql="select * from client";
                query.exec(sql);
                while(query.next())
                {
                    if(query.value(0).toString()==*clientname[j])
                    {
                        sql=QString("update client set score=%1 where id=%2;").arg(sscore).arg(sname);
                        query.exec(sql);
                    }
                }
            }
            for(int j=0;j<3;j++)
            {
                sc[j]=QString::number(score[j],10);
                ch[j]=QString::number(change[j],10);
            }
            for(int j=0;j<3;j++)
            {
                QString o="win";
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<(quint16)0;
                out<<o<<*clientname.at(who1);
                out<<sc[0]<<sc[1]<<sc[2];
                out<<ch[0]<<ch[1]<<ch[2];
                out<<m_Cards[(who1+1)%3].Count();
                for(QSet<Card>::const_iterator it=m_Cards[(who1+1)%3].m_cards.begin();it!=m_Cards[(who1+1)%3].m_cards.end();it++)
                {
                    out<<qHash(*it);
                }
                out<<m_Cards[(who1+2)%3].Count();
                for(QSet<Card>::const_iterator it=m_Cards[(who1+2)%3].m_cards.begin();it!=m_Cards[(who1+2)%3].m_cards.end();it++)
                {
                    out<<qHash(*it);
                }
                out.device()->seek(0);
                out<<(quint16)(block.size()-sizeof(quint16));
                tcpsocketlist.at(j)->write(block);
            }
            who1=-1;
            bet=1;
            who=-1;
            QTime time;
            time = QTime::currentTime();
            qsrand(time.msec()+time.second()*1000);
            now=qrand()%3;
        }
    }
    blocksize=0;
    read();
}

void TCPserver2::dis()
{
    if(tcpsocketlist.count()==3)
    {
        QTcpSocket* m_tcpsocket=dynamic_cast<QTcpSocket*>(sender());
        for(int i=0;i<tcpsocketlist.count();i++)
        {
           QTcpSocket* item = tcpsocketlist.at(i);
           if(item->socketDescriptor()==m_tcpsocket->socketDescriptor())
           {
               tcpsocketlist.removeAt(i);
               QString m=*clientname.at(i);
               clientname.removeAt(i);
               for(int i=0;i<3;i++)
               {
                   if(*TCPserver::clientname[TCPserver::room[roomnumber-1][i]]==m)
                   {
                       for(;i<3;i++)
                       {
                           if(i==2)
                           {
                               TCPserver::room[roomnumber-1][2]=-1;
                               break;
                           }
                           TCPserver::room[roomnumber-1][i]=TCPserver::room[roomnumber-1][i+1];
                       }
                   }
               }
               for(int i=0;i<2;i++)
               {
                   QString m="exit";
                   QByteArray block;
                   QDataStream out(&block,QIODevice::WriteOnly);
                   out<<(quint16)0;
                   out<<m;
                   out.device()->seek(0);
                   out<<(quint16)(block.size()-sizeof(quint16));
                   tcpsocketlist.at(i)->write(block);
               }
           }
       }
       counter=0;
       QTime time;
       time = QTime::currentTime();
       qsrand(time.msec()+time.second()*1000);
       now=qrand()%3;
       beginornot=0;
       bet=1;
       who1=-1;
       who=-1;
   }
   if(tcpsocketlist.count()<3)
   {
       QTcpSocket* m_tcpsocket=dynamic_cast<QTcpSocket*>(sender());
       for(int i=0;i<tcpsocketlist.count();i++)
       {
           QTcpSocket* item = tcpsocketlist.at(i);
           if(item->socketDescriptor()==m_tcpsocket->socketDescriptor())
           {
               tcpsocketlist.removeAt(i);
               clientname.removeAt(i);
           }
       }
   }
}
