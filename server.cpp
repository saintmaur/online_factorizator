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

void serve(int arg)
{
    auto t_id = std::this_thread::get_id();
    std::cout << " A thread #" << t_id << " has been created\n";
    std::cout << "Ready to handle the messages\n";    
    
    int clientSock = arg;

    while(1)
    {
        char send_buf[DFLT_BUFFER_SIZE];
	
	char recvBuffer[DFLT_BUFFER_SIZE];
        memset(recvBuffer, 0, strlen(recvBuffer));
	auto res = recv(clientSock, recvBuffer, sizeof(recvBuffer), 0);
	std::string response = "";
	
	std::cout << "Received data: " << recvBuffer << std::endl;
		
        if(res > 0)
        {
            if(std::isdigit(recvBuffer[0]))
	    {
		int num = atoi(recvBuffer);
		for (int i = 2; i <= num; ++i) {
		    while (num % i == 0)
		    {
			num /= i;
			char temp[128];
			sprintf(temp, "%d", i);
			response.append(temp).append(" ");
		    }
		}
	    }
	    else
	    {
		response = "";
	    }
	    strcpy(send_buf, response.c_str());
        }
	else if (res == 0)
	{
	    std::cout << "Connection with the client has been closed\n";
	    break;
	} 
	else
	{
            std::cout << "Couldn't read data from the client: " 
		    << strerror(errno) << "\n";
            break;
        }
        // if response is prepared send it
	if(sizeof(send_buf))
	{
	    std::cout << "the prime factors found: " << send_buf << std::endl;
            send(clientSock, send_buf, sizeof(send_buf), 0);
        }
    }
    std::cout << "The thread #" << t_id << " is to be finished. Wish the best.\n";
    close_socket(clientSock);
}

void TCPServer::cust_connect(const cfg_t &cfg)
{
    sockaddr_in serverInfo;

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(cfg.port);
    // bind the unbound socket to the certain port
    int retVal = bind(sock, (struct sockaddr*)& serverInfo, sizeof(serverInfo));
    if (retVal < 0)
    {
        char temp[DFLT_BUFFER_SIZE];
        sprintf(temp, "Unable to wake up the server (%s:%d)\n", 
		cfg.host.c_str(), cfg.port);
        throw std::string(temp);
    }
    else 
    {
        std::string ip = get_ip(cfg);
	std::cout << "The server socket has been binded to an address ("
		<< ip << ":" << cfg.port << ")\n"
		<< " use '</path/to>/online_factor -m 2"
		<< " -h "<< ip
		<< " -p " << cfg.port << "' to connect \n"
		<< " press CTRL+C to stop\n"
		<< "Listening...\n";
    }
    // start listening on the socket binded to the port
    retVal = listen(sock, 10);
    if (retVal < 0)
    {
        throw std::string("Unable to start listening");
    }
    // wait for connection
    while (1)
    {
	std::cout << "Ready for new connections\n";
	auto clientSock = accept(sock, NULL, NULL);
	
        if (clientSock < 0)
        {
            throw std::string("Unable to accept connection");
        }
	// after receiving a connect start a thread to serve it
	std::thread handler(serve, clientSock);
	// no need to wait it
	handler.detach();
    }
}