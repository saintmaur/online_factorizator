test:
	g++ -isystem ${GTEST_ROOT}/include -std=c++11 -pthread socket.cpp server.cpp \
	client.cpp unittest/gtest_socket.cpp unittest/gtest_main.cpp \
	${GTEST_ROOT}/libgtest.a \
	-o online_factor_test
all:
	g++ getopt.cpp socket.cpp server.cpp client.cpp main.cpp -std=c++11 -pthread -o online_factor