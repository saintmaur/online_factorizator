all:
	g++ ./src/*.cpp -std=c++11 -pthread -o ./bin/online_factor
test:
	g++ -isystem ${GTEST_ROOT}/include -std=c++11 -pthread \
	./src/socket.cpp ./src/server.cpp ./src/client.cpp \
	./unittest/gtest_common.cpp ./unittest/gtest_main.cpp \
	${GTEST_ROOT}/libgtest.a -o ./bin/online_factor_test