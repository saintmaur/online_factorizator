#include "gtest/gtest.h"
#include "../src/socket.h"
#include "gtest_common.h"

TEST_F(TCPSocketTest, TCPSocketCanCreateSocket)
{
    TCPSocket sock_obj;
    ASSERT_GT(sock_obj.get_socket(), 0);
}

TEST_F(TCPSocketTest, TCPSocketCanGetIP)
{
	TCPSocket socket_obj;
	cfg_t cfg = {"www.google.com"};
    auto ip = get_ip(cfg);
    ASSERT_NE(ip, std::string(""));
}

TEST_F(TCPSocketTest, TCPSocketCanCloseCustomSocket)
{
    cfg_t cfg;
    int sock;
    TCPServer server;
    server.do_connect(cfg);
    sock = server.get_socket();
    close_socket(sock);
    
    int result = server.check_connection();
    ASSERT_NE(result, 0) << "Nope, it's not closed ";
}

TEST_F(TCPSocketTest, TCPSocketServerCanBindSocket)
{
    cfg_t cfg;
    TCPServer server;
    server.do_connect(cfg);
    int result = server.check_connection();
    ASSERT_EQ(result, 0) << "Nope, it's not bound ";
}
//===================================================
TEST_F(DummyNumbersTest, DummyNumbersTestCanFactorize)
{
    auto result = get_prime_factors(2);
    ASSERT_EQ(result, "2 ");
    
    result = get_prime_factors(32);
    ASSERT_EQ(result, "2 2 2 2 2 ");
    
    result = get_prime_factors(39);
    ASSERT_EQ(result, "3 13 ");
}