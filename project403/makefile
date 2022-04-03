server:
	g++ -std=c++11 -g threadpool.cpp epoll.cpp server.cpp util.cpp http.cpp main.cpp -o server -lpthread && g++ server.cpp util.cpp client.cpp -o client
clean:
	rm -rf client server