#include "socket.h"
#include "common.h"

std::string get_ip(const cfg_t& cfg)
{
    hostent* host = gethostbyname(cfg.host.c_str());
    if (host == NULL) {
		auto err = get_error();
	throw std::string("gethostbyname has failed");
    }
    char ip[100];

    struct in_addr **addr_list;
    // a way to get the correct IP is hard
    addr_list = (in_addr **) host->h_addr_list;
    for (int i = 0; addr_list[i] != NULL; i++)
    {
	//Return the first address;
	strcpy(ip, inet_ntoa(*addr_list[i]));
    }
    return std::string(ip);
}

TCPSocket::TCPSocket(){
    // winsock initial procedure is something special
    if (!init())
    {
	throw std::string("Unable to initialize a socket");
    }
    // try to create a socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
    {
        throw std::string("Unable to create socket");

    }
}

TCPSocket::~TCPSocket()
{
    // close own socket
    close_initial_connect();
}

std::string TCPSocket::receive_data()
{
    char response[DFLT_BUFFER_SIZE];
    // recieve data from socket
    int res = recv(sock, response, sizeof(response), 0);
    // check the result
    if (res < 0)
    {
        throw std::string("Unable to receive a message");
    }
    return std::string(response);
}

void TCPSocket::close_initial_connect()
{
// platform independent socket closing
#ifdef __linux__
	close(sock);
#else
	closesocket(sock);
	WSACleanup();
#endif
}

int get_error()
{
// platform independent error retrieving
#ifdef __linux__
    return errno;
#else
    return WSAGetLastError();
#endif
}

void close_socket(int sock)
{
// platform independent socket closing
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
	// - pass the version and the placeholder for additional data if any
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) 
    {
	std::cout << "WSAStartup failed with error: " << res << std::endl;
	return false;
    }
    return true;
#else
    return true;
#endif
}

int TCPSocket::check_connection()
{
    int error_code;
    SOCKLEN_TYPE error_code_size = sizeof (error_code);
    getsockopt(sock, SOL_SOCKET, SO_ERROR, (char *)&error_code, &error_code_size);
    return error_code;
}

int TCPSocket::get_socket()
{
    return sock;
}

sockaddr_in TCPSocket::init_srv_info(const cfg_t& cfg)
{
    // a structure to pass necessary params to socket handle functions
    sockaddr_in serverInfo;
    // set necessary params
    serverInfo.sin_family = AF_INET;
    if(cfg.appl_mode == appl_mode_t::mode_client)
    {
	// get the IP address
	std::string ip = get_ip(cfg);
	serverInfo.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    else
    {
	serverInfo.sin_addr.s_addr = INADDR_ANY;
    }
    serverInfo.sin_port = htons(cfg.port);
    
    return serverInfo;
}
    