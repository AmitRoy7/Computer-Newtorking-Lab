#ifndef IPADDRESS_H_
#define IPADDRESS_H_

#include "headers.h"

///************FINDING IP ADDRESS***************\\\

string ipAddress()
{
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { /// check it is IP4
            /// is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            if(strcmp((ifa->ifa_name),"lo")!=0)
                return (string)addressBuffer;
        }
    }    return 0;
}


#endif // IPADDRESS_H_
