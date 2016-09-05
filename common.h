#ifndef COMMON_PART
#define COMMON_PART 1
#include <string.h>

const int DFLT_CLNT_COUNT   = 2;
const int DFLT_PORT         = 1111;
const int DFLT_BUFFER_SIZE  = 1024;

enum appl_mode_t{
    mode_server = 1,
    mode_client = 2
};

struct cfg_t {
    int port;
    int clnt_count;
    char host[100];
    appl_mode_t appl_mode;
    cfg_t() : port(DFLT_PORT), clnt_count(DFLT_CLNT_COUNT),
        appl_mode(mode_server)
    {
        strcpy_s(host, "localost");
    }
};

//void fix_params(int argc, char* argv[], cfg_t &cfg){
//    const struct option longopts[] =
//        {
//            {"port",        optional_argument,	0, 'p'},
//            {"mode",        optional_argument,	0, 'm'},
//            {"clnt_cnt",	optional_argument,	0, 'c'},
//            {"server_host", optional_argument,	0, 'h'},
//            {0,0,0,0},
//        };
//    int opt = 0;
//    char * temp = 0;
//    int option_index;
//    while( (opt = getopt_long(argc,argv,"p:m:c:h::",&longopts,&option_index)) != -1){
//        temp = optarg;
//        switch(opt){
//        case 'p':
//            int port = atoi(temp);
//            if (port < 1000 || port > 65000){
//                cfg.port = DFLT_PORT;
//            } else {
//                cfg.port = port;
//            }
//            break;
//        case 'm':
//            if (temp){
//                appl_mode_t mode = static_cast<appl_mode_t>(atoi(temp));
//                if (mode != mode_server && mode != mode_client) {
//                    cfg.appl_mode = mode_server;
//                } else{
//                    cfg.appl_mode = mode;
//                }
//            }
//            break;
//        case 'c':
//            if (temp){
//                cfg.clnt_count = atoi(temp);
//                if (cfg.clnt_count < 0) {
//                    cfg.clnt_count = DFLT_CLNT_COUNT;
//                }
//            }
//            break;
//        case 'h':
//            if (temp){
//                cfg.host = temp;
//                if (sizeof(cfg.host) == 0) {
//                    cfg.host = DFLT_HOST;
//                }
//            }
//            break;
//        default:
//            break;
//        }
//    }
//}
#endif
