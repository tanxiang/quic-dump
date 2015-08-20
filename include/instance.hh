#pragma once
#include <ev++.h>
#include "unit.hh"
#include <fcntl.h>
namespace ssev{

template <typename protocol>
class instance:public noncopyable{
	int client_fd;
	int fd=0;
	ev::io io_read,io_write,
		client_io_read,client_io_write;
	ev::periodic timer;
	protocol session;
	
	void recv_event(ev::io &watcher, int revents);
	
	void client_recv_event(ev::io &watcher, int revents);
	
	void send_event(ev::io &watcher, int revents);
	
	void client_send_event(ev::io &watcher, int revents);
	
	void timer_event(ev::periodic &watcher, int revents);

	void tcp_sendto(std::string&);//fastopen
	void udp_sendto(std::string&);
	void client_send(std::string&);
public:

	instance(int fd,const ev::dynamic_loop &l):client_fd(fd),
		io_read{l},io_write{l},
		client_io_read{l},client_io_write{l},
		timer{l},session{
				[this](std::string& data){
					return tcp_sendto(data);
				},
				[this](std::string& data){
					return udp_sendto(data);
				},
				[this](std::string& data){
					return client_send(data);
				}
		}
	{
		client_io_read.set<instance, &instance<protocol>::client_recv_event>(this);
		client_io_write.set<instance, &instance<protocol>::client_send_event>(this);
		fcntl(fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK); 
		client_io_read.start(client_fd, ev::READ);
		io_read.set<instance, &instance<protocol>::recv_event>(this);
		io_write.set<instance, &instance<protocol>::send_event>(this);
		timer.set<instance, &instance<protocol>::timer_event>(this);
	}

	~instance(){
		io_read.stop();
		io_write.stop();
		client_io_read.stop();
		client_io_write.stop();
		timer.stop();
	}
};

template <typename cryptor>
class ssocket5:public noncopyable{
	std::function<void(std::string&)> tcp_sendto;
	std::function<void(std::string&)> udp_sendto;
	std::function<void(std::string&)> client_send;
public:
	ssocket5(auto tcp_sendto,auto udp_sendto,auto client_send):
		tcp_sendto{tcp_sendto},udp_sendto{udp_sendto},client_send{client_send}{
	}
};

};
