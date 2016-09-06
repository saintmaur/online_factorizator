#include "getopt.h"
#include "TCPSocket.h"
#include <cstdlib>

const char* MSG_DELIM = "================================";

int main(int argc, char* argv[])
{
    cfg_t cfg;
//    //fix_params(argc, argv, cfg);
    const struct option longopts[] =
        {
            {"port",        optional_argument,	0, 'p'},
            {"mode",        optional_argument,	0, 'm'},
            {"clnt_cnt",	optional_argument,	0, 'c'},
            {"server_host", optional_argument,	0, 'h'},
            {0,0,0,0},
        };
    int opt = 0;
    char * temp = 0;
    int option_index;
    while( (opt = getopt_long(argc,argv,"p:m:c:h::",longopts,&option_index)) != -1){
        temp = optarg;
        switch(opt){
        case 'p':
        {
            int port = atoi(temp);
            if (port < 1000 || port > 65000){
                cfg.port = DFLT_PORT;
            } else {
                cfg.port = port;
            }
        }
            break;
        case 'm':
        {
            if (temp){
                //1 - server, 2 - client
                appl_mode_t mode = static_cast<appl_mode_t>(atoi(temp));
                if (mode != mode_server && mode != mode_client) {
                    cfg.appl_mode = mode_server;
                } else{
                    cfg.appl_mode = mode;
                }
            }
        }
            break;
        case 'c':
        {
            cfg.clnt_count = atoi(temp);
            if (cfg.clnt_count){
                if (cfg.clnt_count < 0) {
                    cfg.clnt_count = DFLT_CLNT_COUNT;
                }
            }
        }
            break;
        case 'h':
        {
            if(temp){
                cfg.host = temp;
            } else {
                cfg.host = "localhost";
            }
        }
            break;
        default:
            break;
        }
    }
    try{
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
    } catch(std::exception &e){
        printf("Caught an exception: %s", e.what());
		system("pause");
    }

    return 1;
}

