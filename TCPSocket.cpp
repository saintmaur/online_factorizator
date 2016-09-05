#include "TCPSocket.h"
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <cstdio>
#include <string>
#include <memory>
#include <mutex>
#include <stdlib.h>
#include <stdio.h>

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
        sprintf_s(tmstmp, "%f", (double)ts);
        sid = "Socket_";
        sid.append(tmstmp);
    }
}

TCPSocket::~TCPSocket(){
    close();
}

void TCPSocket::close()
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

void TCPClient::cust_connect(const cfg_t &cfg){
	sockaddr_in serverInfo;

    serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(cfg.port);

    int retVal = connect(TCPSock, (struct sockaddr*) &serverInfo, sizeof(serverInfo));
    if (retVal < 0)
    {
        char temp[50];
        sprintf_s(temp, "Unable to connect to server (%s:%d)\n", cfg.host, cfg.port);
        TCPException e(temp);
        throw(e);
    } else {
        printf("Connection has been established\n");
        printf("Wating for input\n");
        while(1){
            printf("> ");
            char seBuf[DFLT_BUFFER_SIZE];
            std::cin.getline(seBuf, sizeof(seBuf));
            printf("Sending a request to server\n");
			set_msg(seBuf);
            cust_send();
        }
    }
}

void TCPClient::cust_send(){

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

    printf("Number factorization is:\n%s\n", response);
}

void* fork_serving(void * arg){

    printf("Ready to recieve a message\n");

    int clientSock = *(reinterpret_cast<int*>(arg));
    fflush(stdin);
    char f_name[20];
    time_t ts = time(NULL);
    sprintf_s(f_name, "Socket_%f_file.txt", (double)ts);
    FILE * cli_file_h = fopen(f_name, "a");

    char sendBuffer[DFLT_BUFFER_SIZE];

    while(1){
        char recvBuffer[DFLT_BUFFER_SIZE];
        memset(recvBuffer, 0, strlen(recvBuffer));
		auto res = recv(clientSock, recvBuffer, sizeof(recvBuffer), 0);
        if(res > 0)
        {
            // check whether recvBuffer is a number
			// check whether recvBuffer is an integer (cut if not)
			// factorize
			// fill the sendBuffer with numbers sequence
            //sprintf(sendBuffer, "Data's been put to the file (%s)", f_name);
        } else {
            printf("Couldn't read data from the client: %s\n",
                   strerror(errno));

            break;
        }
        if(strlen(sendBuffer)){
			std::lock_guard<std::mutex> _m(server_mutex);
            send(clientSock, sendBuffer, sizeof(sendBuffer), 0);

            memset(sendBuffer, 0, strlen(sendBuffer));
        }
    }
    fclose(cli_file_h);
    return 0;
}

bool TCPSocket::init()
{
#ifndef __linux__
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}
	return true;
#endif
}

void TCPServer::cust_connect(const cfg_t &cfg){

    sockaddr_in serverInfo;

	if (!init())
	{
		char temp[DFLT_BUFFER_SIZE];
		sprintf(temp, "Unable to initialize server socket\n", cfg.host, cfg.port);
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
        sprintf(temp, "Unable to wake up the server (%s:%d)\n", cfg.host, cfg.port);
        TCPException e(temp);
        throw(e);
    } else {
        printf("The Server Socket has been binded to an address\n");

        printf("Listening...\n");
    }

    retVal = listen(TCPSock, 10);
    if (retVal < 0)
    {
        TCPException e("Unable to listen");
        throw(e);
    }
    //Ждем клиента
    while (1){
		auto clientSock = accept(TCPSock, NULL, NULL);

        if (clientSock < 0)
        {
            TCPException e("Unable to accept connection");
            throw(e);
        }
		
		std::thread handler(fork_serving, clientSock);
		handler.join();
    }
}

TCPException::TCPException(const char* msg) {
    strcpy_s(err, msg);
}

const char* TCPException::what() const throw() {
    char result[DFLT_BUFFER_SIZE];
	std::string temp;
	std::iostream strm(temp);
	strm << err << ": " << strerror(errno);
    sprintf_s(result, "%s: %s\n\n", err, strerror(errno));
    return result;
}
