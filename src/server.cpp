#include "socket.h"
#include "common.h"
#include <thread>
#include <chrono>
#include <algorithm>

// a global variable to keep the connections to the server
// is needed in signal handling
std::vector<int> connections;
// the main target function
std::string get_prime_factors(int num)
{
    std::string result;
    for (int i = 2; i <= num; ++i)
    {
	while (num % i == 0)
	{
	    num /= i;
	    char temp[128];
	    sprintf(temp, "%d", i);
	    result.append(temp).append(" ");
	}
    }
    return result;
}
// connection serving thread function
void serve(int client_sock)
{
    auto t_id = std::this_thread::get_id();
    std::cout << " A thread #" << t_id << " has been created\n";
    std::cout << "Ready to handle the messages\n";    

    add_connection(client_sock);
    // wait in indefinite loop for data
    while(1)
    {
        char send_buf[DFLT_BUFFER_SIZE];
        char recv_buffer[DFLT_BUFFER_SIZE];
	// erase previous records
	memset(recv_buffer, 0, strlen(recv_buffer));
	// wait for data from the client socket
	auto res = recv(client_sock, recv_buffer, sizeof(recv_buffer), 0);
	std::string response;
	int num = 0;
	// check the result
        if(res > 0)
        {
            if(std::isdigit(recv_buffer[0]))
	    {
		num = atoi(recv_buffer);
		response = get_prime_factors(num);
	    }
	    else
	    {
		response = "Invalid input";
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
	    if (get_error() == TCP_CONN_CLOSED)
	    {
		// winsock generates an error when connection is closed
		std::cout << "Connection with the client has been closed\n";
	    }
	    else
	    {
		std::cout << "Couldn't read data from the client: "
			    << get_error() << "\n";
	    }
	    break;
	}
        // if response is ready send it
	if(sizeof(send_buf))
	{
	    std::cout << "The prime factors for number " << num << " are: " << send_buf << std::endl;
	    send(client_sock, send_buf, sizeof(send_buf), 0);
        }
    }
    std::cout << "The thread #" << t_id << " is to be finished. Wish the best.\n";
    remove_connection(client_sock);
    close_socket(client_sock);
}

void TCPServer::custom_connect(const cfg_t &cfg)
{
    int ret_val = do_connect(cfg);
    // check the result of binding
    if (ret_val < 0)
    {
        char temp[DFLT_BUFFER_SIZE];
        sprintf(temp, "Unable to wake up the server (%s:%d)\n", 
		cfg.host.c_str(), cfg.port);
        throw std::string(temp);
    }
    else 
    {
        // get the appropriate host name
	std::string ip = get_ip(cfg);
	char hostname[512];
	gethostname(hostname, 512);
	// inform the user how to connect to the newly started server
	std::cout << "The server socket has been bound to the address ("
		<< ip << ":" << cfg.port << ")\n"
		<< " use '</path/to>/online_factor client "
		<< hostname << " "
		<< cfg.port << "' to connect \n"
		<< " press CTRL+C to stop\n"
		<< "Listening...\n";
    }
    // start listening on the socket bound previously
    ret_val = listen(sock, 10);
    if (ret_val < 0)
    {
        throw std::string("Unable to start listening");
    }
    // wait infinite loop for connections and data handle
    while (1)
    {
	std::cout << "Ready for new connections\n";
	// wait for connection from a client
	auto clientSock = accept(sock, NULL, NULL);
	
        if (clientSock < 0)
        {
            throw std::string("Unable to accept connection");
        }
	// start a thread to serve a new client when connected
	std::thread handler(serve, clientSock);
	// no need to wait it
	handler.detach();
    }
}

int TCPServer::do_connect(const cfg_t& cfg)
{
    auto serverInfo = init_srv_info(cfg);
    // bind the unbound socket to the certain port
    int result = bind(sock, (struct sockaddr*)& serverInfo, sizeof(serverInfo));
    
    return result;
}

void add_connection(int sock)
{
    connections.push_back(sock);
}
void remove_connection(int sock)
{
    auto elem = std::find(connections.begin(), connections.end(), sock);
    connections.erase(elem);
}

void handle_term_signal(int)
{
    for (auto sock : connections)
    {
	::close_socket(sock);
    }
    connections.clear();
    std::cout << std::endl << "The bar is closed. "
		"Have a good day!" << std::endl;
    // and don't forget to make a real exit, of course
    exit(0);
}
