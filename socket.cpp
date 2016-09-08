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

std::string get_ip(const cfg_t& cfg)
{
    hostent* host = gethostbyname(cfg.host.c_str());
    if (host == NULL) {
	throw std::string("gethostbyname has failed");
    }
    char ip[100];

	char hostname[512];
	gethostname(hostname, 1023);

	std::cout << hostname << std::endl
			<< host->h_name << std::endl;
	
    struct in_addr **addr_list;

    addr_list = (in_addr **) host->h_addr_list;

    for (int i = 0; addr_list[i] != NULL; i++) {
	//Return the first address;
	strcpy(ip, inet_ntoa(*addr_list[i]));
    }
    
    return std::string(ip);
}

TCPSocket::TCPSocket(){
	
	if (!init())
	{
		char temp[DFLT_BUFFER_SIZE];
		sprintf(temp, "Unable to initialize server socket");
		throw temp;
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
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

std::string TCPSocket::receive_data()
{
    char response[DFLT_BUFFER_SIZE];
    int res = recv(sock, response, sizeof(response), 0);

    if (res < 0)
    {
        throw std::string("Unable to receive a message");
    }
    
    return std::string(response);
}

void TCPSocket::close_socket()
{
#ifdef __linux__
	close(sock);
#else
	closesocket(sock);
	WSACleanup();
#endif
}

int get_error()
{
#ifdef __linux__
    return errno;
#else
    return WSAGetLastError();
#endif
}

void close_socket(int sock)
{
#ifdef __linux__
	close(sock);
#else
	closesocket(sock);
#endif
}

void TCPSocket::set_msg(const char* msg_)
{
    msg.assign(msg_);
}

bool TCPSocket::init()
{
#ifndef __linux__
    WSADATA wsaData;
	// initialize the winsock library 
	// - pass the version and the placeholder for additional data
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) 
    {
		printf("WSAStartup failed with error: %d\n", res);
		return false;
    }
    return true;
#else
    return true;
#endif
}

void TCPSocket::check_connection()
{
    int error_code;
    SOCKLEN_TYPE error_code_size = sizeof (error_code);
    getsockopt(sock, SOL_SOCKET, SO_ERROR, (char *)&error_code, &error_code_size);
    if(error_code)
    {
	close_socket();
	throw std::string("Connection is closed");
    }
}

int TCPSocket::get_socket()
{
    return sock;
}