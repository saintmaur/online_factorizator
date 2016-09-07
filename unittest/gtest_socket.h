#include "gtest/gtest.h"
#include "../socket.h"

class TCPServerTest : public ::testing::Test
{
    public:
	TCPServerTest();
	TCPServer server;
};