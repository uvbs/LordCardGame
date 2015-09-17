#ifndef CARDS_H
#define CARDS_H

#include <QMap>
#include <QList>
#include <QVector>
#include <QSet>
#include <QTime>

enum CardPoint
{
    Card_Begin,
    Card_3,
    Card_4,
    Card_5,
    Card_6,
    Card_7,
    Card_8,
    Card_9,
    Card_10,
    Card_J,
    Card_Q,
    Card_K,
    Card_A,
    Card_2,
    Card_SJ,
    Card_BJ,
    Card_End
};
const int PointCount=15;

enum CardSuit
{
    Suit_Begin,
    Diamond,//方块
    Club,//梅花
    Heart,//红桃
    Spade,//黑桃
    Suit_End
};

enum HandType
{
    Hand_Unknown,//不知道是什么牌型
    Hand_Pass,//过
    Hand_Single,//单牌
    Hand_Pair,//双牌
    Hand_Seq_Single,//单顺
    Hand_Seq_Pair,//双顺
    Hand_Seq_Triple,//三顺
    Hand_Plane_n_Single,//飞机带n单(包括三带一）
    Hand_Plane_n_Pair,//飞机带n对（包括三带二）
    Hand_Bomb,//炸弹
    Hand_Bomb_Single,//炸弹带一个
    Hand_Bomb_Pair,//炸弹带一对
    Hand_Bomb_Two_Single,//炸弹带两单
    Hand_Bomb_Jokers,//王炸
};

class Card
{
public:
    Card(CardPoint point1=Card_Begin,CardSuit suit1=Suit_Begin)
    {
        point=point1;
        suit=suit1;
    }
    Card(uint i,uint j)
    {
        if(i==0)
        point=Card_Begin;
        if(i==1)
        point=Card_3;
        if(i==2)
        point=Card_4;
        if(i==3)
        point=Card_5;
        if(i==4)
        point=Card_6;
        if(i==5)
        point=Card_7;
        if(i==6)
        point=Card_8;
        if(i==7)
        point=Card_9;
        if(i==8)
        point=Card_10;
        if(i==9)
        point=Card_J;
        if(i==10)
        point=Card_Q;
        if(i==11)
        point=Card_K;
        if(i==12)
        point=Card_A;
        if(i==13)
        point=Card_2;
        if(i==14)
        point=Card_SJ;
        if(i==15)
        point=Card_BJ;
        if(i==16)
        point=Card_End;
        if(j==0)
            suit=Suit_Begin;
        if(j==1)
            suit=Diamond;
        if(j==2)
            suit=Club;
        if(j==3)
            suit=Heart;
        if(j==4)
            suit=Spade;
        if(j==5)
            suit=Suit_End;
    }

    CardPoint point;
    CardSuit suit;
};

inline bool operator==(const Card& left,const Card& right)
{
    return (left.point==right.point&&left.suit==right.suit);
}

inline bool operator<(const Card& left,const Card& right)
{
   if(left.point==right.point)
   {
       return left.suit<right.suit;
   }
   else
   {
       return left.point<right.point;
   }
}

inline uint qHash(const Card& card)
{
    return card.point*100+card.suit;
}

typedef QVector<Card> CardList;//定义一个类CardList,其本质是卡片的链表

class Cards
{
public:
    Cards();
    ~Cards();
    enum SortType
    {
        Nosort,
        Asc,
        Desc
    };
    void Add(const Card& card);
    void Add(const Cards& cards);
    void Add(const QVector<Cards>& cardsArray);//这里应该是储存卡片组（Cards)的容器,不是卡片容器

    void Remove(const Card& card);
    void Remove(const Cards& cards);
    void Remove(const QVector<Cards>& cardsArray);

    bool Contains(const Card& card)const;
    bool Contains(const Cards& cards)const;

    Cards& operator<<(const Cards& cards);
    Cards& operator<<(const Card& card);
    Cards& operator<<(const QVector<Cards>& cardsArray);

    CardList ToCardList(SortType=Asc)const;//将卡组放入卡片容器中,形成有序卡组
    int Count();
    int PointCount(CardPoint point);
    CardPoint MinPoint();
    CardPoint MaxPoint();

    Card TakeRandomCard();

    bool IsEmpty() const;
    void Clear();
public:
    QSet<Card> m_cards;//无序的卡组
};

#endif // CARDS_H
