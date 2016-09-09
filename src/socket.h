#ifndef TCP_DESC_PART
#define TCP_DESC_PART 1

#include <sys/types.h>
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

#define TCP_CONN_CLOSED 0
typedef unsigned int SOCKLEN_TYPE;
#else
#include <cctype>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#include <winsock2.h>

#define TCP_CONN_CLOSED WSAECONNRESET

typedef int SOCKLEN_TYPE;
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
    std::string host	    = "localhost";
    int port		    = DFLT_PORT;
    appl_mode_t appl_mode   = mode_server;
    cfg_t(std::string host_ = "localhost"
	    , int port_ = DFLT_PORT
	    , appl_mode_t mode_ = mode_server)
	: host(host_), port(port_), appl_mode(mode_){}	
};

int get_error();
std::string get_ip(const cfg_t& cfg);
void close_socket(int sock);
void serve(int arg);
std::string get_prime_factors(int num);

void add_connection(int sock);
void remove_connection(int sock);
// basic class
class TCPSocket
{
protected:
    int sock;
    std::string msg;
public:
    TCPSocket();
    virtual ~TCPSocket();

    bool init();
    virtual void custom_connect(const cfg_t &){}
    virtual void custom_send(){}
    virtual int do_connect(const cfg_t &cfg){};
    std::string receive_data();
    void set_msg(const char* msg_);
    int check_connection();
    void close_initial_connect();
    int get_socket();
    sockaddr_in init_srv_info(const cfg_t &cfg);
};

class TCPClient : public TCPSocket
{
public:
    void custom_connect(const cfg_t &cfg);
    void custom_send();
    int do_connect(const cfg_t &cfg);
};

void handle_term_signal(int signal);

class TCPServer : public TCPSocket
{
public:
    TCPServer(){};
    ~TCPServer(){};

    void custom_connect(const cfg_t &cfg);
    void custom_send(){};
    int do_connect(const cfg_t &cfg);
};
#endif
