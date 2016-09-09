#include "socket.h"
#include "common.h"

void TCPClient::custom_connect(const cfg_t &cfg)
{
    int ret_val = do_connect(cfg);
    // check the result of connecting
    if (ret_val < 0)
    {
        char temp[128];
	sprintf(temp, "Unable to connect to server (%s:%d), errcode: %d\n", 
			cfg.host.c_str(), cfg.port, get_error());
        throw std::string(temp);
    }
    else
    {
        std::cout << "Connection has been established" << std::endl;
        std::cout << "Wating for input (enter a number)" << std::endl;
	// wait in the infinite loop for inputs
        while(1)
	{
	    std::cout << "> ";
	    std::string str;
		str.clear();
	    // check connection before next input waiting
	    if(check_connection())
	    {
		throw std::string("Sorry, connection is closed");
	    }
	    std::cin >> str;
	    if(str.size())
	    {
		set_msg(str.c_str());
		custom_send();
	    }
	    else
	    {
		std::cout << "Type a number" << std::endl;
	    }
        }
    }
}

void TCPClient::custom_send()
{
    // try to send input data
    int retVal = send(sock, msg.c_str(), strlen(msg.c_str()), 0);
    if (retVal < 0)
    {
        throw std::string("Unable to send the message to the server");
    }
    // try to recieve the server's response and output it
    std::string response = receive_data();
    std::cout << "Response: " << response << std::endl;
}

int TCPClient::do_connect(const cfg_t &cfg)
{
    sockaddr_in serverInfo = init_srv_info(cfg);
    // try to connect to the server by given params
    int result = connect(sock, (sockaddr *)&serverInfo, sizeof(serverInfo));
    
    return result;
}