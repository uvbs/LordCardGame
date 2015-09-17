#ifndef GAMEARRANGE_H
#define GAMEARRANGE_H

#include "tcpserver2.h"

class gamearrange
{
public:
    gamearrange();
private:
    TCPserver2* server[30];
};

#endif // GAMEARRANGE_H
