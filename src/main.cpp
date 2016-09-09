#include "socket.h"
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <cctype>
#include <signal.h>

const char* MSG_DELIM = "\n================================\n";

void print_usage(char * cmd)
{
    std::string temp_str("Please try one of the following variants:");
    temp_str.append(MSG_DELIM).append(cmd).append(" server <port>\n")
	.append(cmd).append(" client <host> <port>")
	.append(MSG_DELIM);
    throw temp_str;
}

int parse_port(char* port_str)
{
    int port = 0;
    if(sizeof(port_str) && std::isdigit(port_str[0]))
    {
	port = atoi(port_str);
	if (port < 1000 || port > 65000)
	{
	    port = DFLT_PORT;
	}
	return port;
    }
    else 
    {
	throw std::string("The port is not set");
    }
}

int main(int argc, char* argv[])
{
    signal(SIGINT, handle_term_signal);
    
    cfg_t cfg;
    try
    {
	if(argc > 1)
	{
	    if(strcmp(argv[1], "server") == 0)
	    {
		cfg.appl_mode = appl_mode_t::mode_server;
		if(argc > 2)
		{
		    cfg.port = parse_port(argv[2]);
		}

	    }
	    else if(strcmp(argv[1], "client") == 0)
	    {
		cfg.appl_mode = appl_mode_t::mode_client;
		cfg.host = argv[2];
		if(argc > 3)
		{
		    cfg.port = parse_port(argv[3]);
		}
	    }
	    else
	    {
		throw std::string("Unknown mode is set");
	    }
	}
	else
	{
	    print_usage(argv[0]);
	}

        switch(cfg.appl_mode){
        case mode_client:
        {
            TCPClient client;
            client.custom_connect(cfg);
         }
            break;
        case mode_server:
        {
            TCPServer server;
            server.custom_connect(cfg);
        }
            break;
        }
    }
    catch(std::string &str)
    {
	std::cout << str << std::endl;
    }

    return 1;
}

