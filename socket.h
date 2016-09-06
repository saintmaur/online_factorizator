#ifndef TCP_DESC_PART
#define TCP_DESC_PART 1

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

const int DFLT_CLNT_COUNT   = 2;
const int DFLT_PORT         = 1111;
const int DFLT_BUFFER_SIZE  = 1024;

enum appl_mode_t{
    mode_server = 1,
    mode_client = 2
};

struct cfg_t 
{
    int port		    = DFLT_PORT;
    int clnt_count	    = DFLT_CLNT_COUNT;
    std::string host	    = "localhost";
    appl_mode_t appl_mode   = mode_server;
};

int get_error();
std::string get_ip(const cfg_t& cfg);
void close_socket(int sock);
void serve(int arg);

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
    std::string receive_data();
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

class TCPException : public std::exception
{
    
};
#endif
