#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
#include <sys/un.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TCP_CONN_CLOSED 0
typedef unsigned int SOCKLEN_TYPE;
#else
#include <cctype>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define TCP_CONN_CLOSED WSAECONNRESET
typedef int SOCKLEN_TYPE;
#endif