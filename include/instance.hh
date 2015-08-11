#pragma once
#include <ev++.h>
#include "unit.hh"

namespace ssev{

template <typename protocol>
class instance:public noncopyable{
	int client_fd;
	int fd=0;
	ev::io io_read,io_write,
		client_io_read,client_io_write;
	protocol session;
	
	void recv_event(ev::io &watcher, int revents);
	
	void client_recv_event(ev::io &watcher, int revents);
	
	void timer_event(ev::periodic &watcher, int revents);
public:
	int tcp_sendto();//fastopen
	int tcp_connect_send();
	int udp_sendto();
	int client_send();
	
	instance(int fd,const ev::dynamic_loop &l);
	~instance();
};

template <typename cryptor>
class ssocket5:public noncopyable{

};

};
