#include "socket.h"
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <memory>
#include <mutex>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#ifdef __linux__
#include <sys/un.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#else
#include <cctype>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#endif

void TCPClient::cust_connect(const cfg_t &cfg)
{
    sockaddr_in serverInfo;

    std::string ip = get_ip(cfg);

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr(ip.c_str());
    serverInfo.sin_port = htons(cfg.port);

    int retVal = connect(TCPSock, (sockaddr *)&serverInfo, sizeof(serverInfo));
    if (retVal < 0)
    {
        char temp[128];
	sprintf(temp, "Unable to connect to server (%s:%d), errcode: %d\n", 
			cfg.host.c_str(), cfg.port, get_error());
        throw std::string(temp);
    }
    else
    {
        std::cout << "Connection has been established\n";
        std::cout << "Wating for input (enter a number)\n";
        while(1)
	{
	    std::cout << "> ";
            char seBuf[DFLT_BUFFER_SIZE];
            std::cin.getline(seBuf, sizeof(seBuf));
	    set_msg(seBuf);
            cust_send();
        }
    }
}

void TCPClient::cust_send()
{
    int retVal = send(TCPSock, msg.c_str(), strlen(msg.c_str()), 0);
    if (retVal < 0)
    {
        throw std::string("Unable to send the message to the server");
    }

    std::string response = receive_data();
    
    std::cout << "Prime factors are: " << response << std::endl;
}