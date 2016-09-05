#ifndef COMMON_PART
#define COMMON_PART 1
#include <string>

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
#endif
