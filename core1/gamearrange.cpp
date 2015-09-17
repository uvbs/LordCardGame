#include "gamearrange.h"

gamearrange::gamearrange()
{
    int port=31331;
    for(int i=0;i<30;i++,port++)
        server[i]=new TCPserver2(port);
}
