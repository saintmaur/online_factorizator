#include "getopt.h"
#include "socket.h"
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <ctype.h>

const char* MSG_DELIM = "================================";

void print_usage(char * cmd)
{
    std::string temp_str("Please try one of the following variants:\n");
    temp_str.append(cmd).append(" server <port>\n")
	.append(cmd).append(" client <host> <port>");
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
    cfg_t cfg;
    try
    {
	if(argc > 1)
	{
	    if(strcmp(argv[1], "server") == 0)
	    {
		appl_mode_t mode = appl_mode_t::mode_server;
		if(argc > 2)
		{
		    cfg.port = parse_port(argv[2]);
		}

	    }
	    else if(strcmp(argv[1], "client") == 0)
	    {
		appl_mode_t mode = appl_mode_t::mode_client;
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
    
//    //fix_params(argc, argv, cfg);
//    const struct option longopts[] =
//        {
//            {"port",        optional_argument,	0, 'p'},
//            {"mode",        optional_argument,	0, 'm'},
//            {"server_host", optional_argument,	0, 'h'},
//            {0,0,0,0},
//        };
//    int opt = 0;
//    char * temp = 0;
//    int option_index;
//    while( (opt = getopt_long(argc,argv,"p:m:c:h::",longopts,&option_index)) != -1)
//    {
//        temp = optarg;
//        switch(opt)
//	{
//        case 'p':
//        {
//            int port = atoi(temp);
//            if (port < 1000 || port > 65000){
//                cfg.port = DFLT_PORT;
//            } else {
//                cfg.port = port;
//            }
//        }
//            break;
//        case 'm':
//        {
//            if (temp){
//                //1 - server, 2 - client
//                appl_mode_t mode = static_cast<appl_mode_t>(atoi(temp));
//                if (mode != mode_server && mode != mode_client) {
//                    cfg.appl_mode = mode_server;
//                } else{
//                    cfg.appl_mode = mode;
//                }
//            }
//        }
//            break;
//        case 'c':
//        {
//            cfg.clnt_count = atoi(temp);
//            if (cfg.clnt_count){
//                if (cfg.clnt_count < 0) {
//                    cfg.clnt_count = DFLT_CLNT_COUNT;
//                }
//            }
//        }
//            break;
//        case 'h':
//        {
//            if(temp){
//                cfg.host = temp;
//            } else {
//                cfg.host = "localhost";
//            }
//        }
//            break;
//        default:
//            break;
//        }
//    }
        switch(cfg.appl_mode){
        case mode_client:
        {
            TCPClient client;
            client.cust_connect(cfg);
         }
            break;
        case mode_server:
        {
            TCPServer server;
            server.cust_connect(cfg);
        }
            break;
        }
    }
    catch(std::string &str)
    {
	std::cout << "Got an error: " << str << std::endl;
    }

    return 1;
}

