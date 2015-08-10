#include "server.hh"
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

namespace ssev{


void server::io_accept(ev::io &watcher, int revents){

}

void server::signal_cb(ev::sig &signal, int revents){

}

static inline int bind_helper(int& fd,unsigned short port = 0){
	fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(fd < 0)
		return fd;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	int flags=1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags));
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
	}

	int qlen = 5;                            // Value to be chosen by application
	setsockopt(fd, SOL_TCP, TCP_FASTOPEN, &qlen, sizeof(qlen));

	socklen_t len = sizeof(addr);
	if (getsockname(fd, (struct sockaddr *)&addr, &len) == -1)
		perror("getsockname");
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
	return ntohs(addr.sin_port);
}


server::server(unsigned short port ,std::string ip){
	output<<"Server:"<<std::endl
		<<"EV loop:"<<(loop.is_default()? "default loop" : "dynamic loop")
		<<"\nEV backend:"<<(loop.backend()==ev::EPOLL?"epoll":"poll")
		<<std::endl;
	sio.set<&server::signal_cb>();
	sio.start(SIGINT);
	
	bind_helper(bind_fd,port);
	
	if(listen(bind_fd, 5)<0){
		perror("listen");
	}
	io.set<server, &server::io_accept>(this);
	io.start(bind_fd, ev::READ);
}

server::~server(){
	close(bind_fd);
}

};
