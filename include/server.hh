#pragma once
#include <ev++.h>
#include <unit.hh>

namespace ssev{

class server:public noncopyable{
protected:
	ev::default_loop loop;
private:
	ev::io io{loop};
	ev::sig sio{loop};

	int bind_fd=0;
	void io_accept(ev::io &watcher, int revents);
	
	static void signal_cb(ev::sig &signal, int revents);

public:
	server(unsigned short port = 443,std::string ip="0.0.0.0");

	~server();

	void run(){
		return loop.run(0);
	}
};

};
