#ifndef TCP_DESC_PART
#define TCP_DESC_PART 1
#include "common.h"

#include <sys/types.h>
#include <thread>
#include <memory>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>

#ifndef __linux__
#include <winsock2.h>
#endif


void serve(int arg);

class TCPException: public std::exception
{
public:
    TCPException(const char* msg);
    const char* what() const throw();
    char err[DFLT_BUFFER_SIZE];
};

class TCPSocket
{
protected:
    std::string sid;
    int TCPSock;
    std::string msg;
public:
    TCPSocket();
    virtual ~TCPSocket();

    bool init();
    virtual void cust_connect(const cfg_t &cfg){}
    virtual void cust_send(){}
    void set_msg(const char* msg_);
    void close_socket();
};

class TCPClient : public TCPSocket
{
public:
    TCPClient(){ std::cout << "Client has started\n";}
    ~TCPClient(){}
    void cust_connect(const cfg_t &cfg);
    void cust_send();
};

class TCPServer : public TCPSocket
{
public:
    TCPServer(){std::cout << "Server has started\n";}
    ~TCPServer(){};
    void cust_connect(const cfg_t &cfg);
    void cust_send(){}
};
#endif
