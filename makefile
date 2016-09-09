bin_name=online_factor
test_bin_name=online_factor_test
$(shell bash -c '[ ! -d ./bin ] && mkdir ./bin')

all:
	g++ ./src/*.cpp -std=c++11 -pthread -o ./bin/$(bin_name)
	make test
	./bin/$(test_bin_name)
test:
	g++ -isystem ${GTEST_ROOT}/include -std=c++11 -pthread \
	./src/socket.cpp ./src/server.cpp ./src/client.cpp \
	./unittest/gtest_common.cpp ./unittest/gtest_main.cpp \
	${GTEST_ROOT}/libgtest.a -o ./bin/$(test_bin_name)