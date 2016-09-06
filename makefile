all:
	g++ getopt.cpp socket.cpp server.cpp client.cpp main.cpp -std=c++11 -pthread -o online_factor