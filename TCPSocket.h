#ifndef TCP_DESC_PART
#define TCP_DESC_PART 1
#include "common.h"
#include <time.h>

#include <sys/types.h>
#include <thread>
#include <memory>
#include <stdio.h>
#include <vector>

#ifndef __linux__
#include <winsock2.h>
#endif


void* fork_requesting(void* arg);
void* fork_serving(void* arg);

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
	std::vector<std::unique_ptr<std::thread>> handlers;
	std::string sid;
	SOCKET TCPSock;
	std::string msg;
public:
    TCPSocket();
    virtual ~TCPSocket();

	bool init();
    virtual void cust_connect(const cfg_t &cfg){}
    virtual void cust_send(){}
	void set_msg(const char* msg_);
	void close();
};

class TCPClient : public TCPSocket
{
public:
    TCPClient(){}
    ~TCPClient(){}
    void cust_connect(const cfg_t &cfg);
    void cust_send();
};

class TCPServer : public TCPSocket
{
public:
    TCPServer(){}
	~TCPServer(){};
    void cust_connect(const cfg_t &cfg);
    void cust_send(){}
};
#endif
