#include "gtest/gtest.h"
#include "gtest_socket.h"
#include "../socket.h"

TEST(Dummy, DummyNumbers)
{
    ASSERT_EQ(2*2,2+2);
}

TEST_F(TCPServerTest, TCPServerCanListen)
{
    ASSERT_GT(server.get_socket(), 0);
}

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    
    int res = RUN_ALL_TESTS();
	    
    return res;
}