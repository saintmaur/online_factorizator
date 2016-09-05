#include "TCPSocket.h"
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
#else
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

std::mutex client_mutex;
std::mutex server_mutex;

TCPSocket::TCPSocket(){
    TCPSock = socket(AF_INET, SOCK_STREAM, 6);
    if (TCPSock < 0)
    {
        printf("Unable to create socket\n");

    } else {
        char tmstmp[50];
        time_t ts = time(NULL);
        sprintf(tmstmp, "%f", (double)ts);
        sid = "Socket_";
        sid.append(tmstmp);
    }
}

TCPSocket::~TCPSocket(){
    close_socket();
}

void TCPSocket::close_socket()
{
#ifdef __linux__
	close(TCPSock);
#else
	closesocket(TCPSock);
	WSACleanup();
#endif
}

void TCPSocket::set_msg(const char* msg_)
{
	msg.assign(msg_);
}

void TCPClient::cust_connect(const cfg_t &cfg)
{
    sockaddr_in serverInfo;

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(cfg.port);

    int retVal = connect(TCPSock, (struct sockaddr*) &serverInfo, sizeof(serverInfo));
    if (retVal < 0)
    {
        char temp[50];
        sprintf(temp, "Unable to connect to server (%s:%d)\n", cfg.host.c_str(), cfg.port);
        TCPException e(temp);
        throw(e);
    } else
    {
        std::cout << "Connection has been established\n";
        std::cout << "Wating for input\n";
        while(1)
	{
            printf("> ");
            char seBuf[DFLT_BUFFER_SIZE];
            std::cin.getline(seBuf, sizeof(seBuf));
            printf("Sending a request to server\n");
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
        TCPException e("Unable to send the message to the server");
        throw(e);
    }

    char response[DFLT_BUFFER_SIZE];
    retVal = recv(TCPSock, response, sizeof(response), 0);

    if (retVal < 0)
    {
        TCPException e("Unable to receive a message from server");
        throw(e);
    }

    printf("Number factorization is: %s\n", response);
}

void serve(int arg){

    std::cout << "Ready to recieve a message\n";
    std::cout << " Thread #" << std::this_thread::get_id() << " is running\n";
    int clientSock = arg;

    fflush(stdin);

    char f_name[20];
    time_t ts = time(NULL);
    sprintf(f_name, "Socket_%f_file.txt", (double)ts);

    FILE * cli_file_h = fopen(f_name, "a");

    char sendBuffer[DFLT_BUFFER_SIZE];

    while(1)
    {
        char recvBuffer[DFLT_BUFFER_SIZE];
        memset(recvBuffer, 0, strlen(recvBuffer));
	auto res = recv(clientSock, recvBuffer, sizeof(recvBuffer), 0);
	std::string responce;
        if(res > 0)
        {
            if(std::isdigit(recvBuffer[0]))
	    {
		int num = atoi(recvBuffer);
		for (int i = 2; i <= num; ++i) {
		    while (num % i == 0) // check for divisibility
		    {
			num /= i;
			char temp[128];
			sprintf(temp, "%d", i);
			responce.append(temp).append(" ");
		    }
		}
	    }
        } else if (res == 0)
	{
	    
	} else
	{
            std::cout << "Couldn't read data from the client: " 
		    << strerror(errno) << "\n";

            break;
        }
        if(responce.size())
	{
            send(clientSock, responce.c_str(), responce.size(), 0);
	    
	    responce = "";
        }
    }
    fclose(cli_file_h);
}

bool TCPSocket::init()
{
#ifndef __linux__
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) 
    {
	printf("WSAStartup failed with error: %d\n", iResult);
	return false;
    }
    return true;
#else
    return true;
#endif
}

void TCPServer::cust_connect(const cfg_t &cfg)
{
    sockaddr_in serverInfo;
    if (!init())
    {
	char temp[DFLT_BUFFER_SIZE];
	sprintf(temp, "Unable to initialize server socket(%s:%d)\n", 
		cfg.host.c_str(), cfg.port);
	TCPException e(temp);
	throw(e);
    }

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(cfg.port);

    int retVal = bind(TCPSock, (struct sockaddr*)& serverInfo, sizeof(serverInfo));
    if (retVal < 0)
    {
        char temp[DFLT_BUFFER_SIZE];
        sprintf(temp, "Unable to wake up the server (%s:%d)\n", 
		cfg.host.c_str(), cfg.port);
        TCPException e(temp);
        throw(e);
    } else 
    {
        std::cout << "The server socket has been binded to an address ("
		<< cfg.host << ":" << cfg.port << ")\n"
		<< " use '</path/to>/online_factor -m 2"
		<< " -h "<< cfg.host 
		<< " -p " << cfg.port << "' to connect \n"
		<< " press CTRL+C to stop "
		<< "Listening...\n";
    }

    retVal = listen(TCPSock, 10);
    if (retVal < 0)
    {
        TCPException e("Unable to listen");
        throw(e);
    }
    //Ждем клиента
    while (1)
    {
	std::cout << "Ready to accept connections\n";
	auto clientSock = accept(TCPSock, NULL, NULL);

        if (clientSock < 0)
        {
            TCPException e("Unable to accept connection");
            throw(e);
        }
	std::cout << " Thread #" << std::this_thread::get_id() << " is running\n";
	std::thread handler(serve, clientSock);
	handler.detach();
    }
}

TCPException::TCPException(const char* msg) {
    strcpy(err, msg);
}

const char* TCPException::what() const throw() {
    char result[DFLT_BUFFER_SIZE];
    sprintf(result, "%s: %s\n\n", err, strerror(errno));
    return result;
}
