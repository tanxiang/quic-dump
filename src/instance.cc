#include "instance.hh"
#include "chacha20.hh"
#include <fcntl.h>
namespace ssev{
template <typename protocol>
void instance<protocol>::timer_event(ev::periodic &watcher, int revents){

}

template <typename protocol>
void instance<protocol>::recv_event(ev::io &watcher, int revents){

}

template <typename protocol>
void instance<protocol>::send_event(ev::io &watcher, int revents){

}

template <typename protocol>
void instance<protocol>::client_recv_event(ev::io &watcher, int revents){

}

template <typename protocol>
void instance<protocol>::client_send_event(ev::io &watcher, int revents){

}

template <typename protocol>
instance<protocol>::instance(int fd,const ev::dynamic_loop &l):client_fd(fd),
		io_read(l),io_write(l),
		client_io_read(l),client_io_write(l),
		timer(l){
	//io_read.set<instance, &instance<protocol>::recv_event>(this);
	
	client_io_read.set<instance, &instance<protocol>::client_recv_event>(this);
	client_io_write.set<instance, &instance<protocol>::client_send_event>(this);
	fcntl(fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK); 
	client_io_read.start(client_fd, ev::READ);

	io_read.set<instance, &instance<protocol>::recv_event>(this);
	io_write.set<instance, &instance<protocol>::send_event>(this);

	timer.set<instance, &instance<protocol>::timer_event>(this);
}

template <typename protocol>
instance<protocol>::~instance(){

}

};

template class ssev::instance<ssev::ssocket5<ssev::chacha20>>;
