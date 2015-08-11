#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "server.hh"

namespace ssev{

template<typename instance_type>
void server<instance_type>::io_accept(ev::io &watcher, int revents){
#ifdef DEBUG_FD_LIVETIME
	output<<"io_accept:"<<revents<<'\n';
#endif
	static uint32_t count;
	if (ev::ERROR & revents) {
		perror("got invalid event");
		return;
	}
	::sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(watcher.fd, (struct sockaddr *)&client_addr, &client_len);
	if(client_fd > 0){
		while(io_workers[count%io_workers.size()].pending())
			++count;
		io_workers[count++%io_workers.size()].setup(client_fd, client_addr);
	}
}

template<typename instance_type>
void server<instance_type>::signal_cb(ev::sig &signal, int revents){

}

static inline int bind_helper(int& fd,unsigned short port = 0){
	fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(fd < 0)
		return fd;
	::sockaddr_in addr;
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

template<typename instance_type>
server<instance_type>::server(unsigned short port ,std::string ip){
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

template<typename instance_type>
server<instance_type>::~server(){
	close(bind_fd);
}

};

#include "instance.hh"
#include "chacha20.hh"
template class ssev::server<ssev::instance<ssev::ssocket5<ssev::chacha20>>>;
