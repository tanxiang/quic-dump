#pragma once
#include <ev++.h>
#include <unit.hh>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <unistd.h>

namespace ssev{

class loop_thread:public noncopyable{
	std::thread sub_thread;
protected:
	ev::dynamic_loop loop;
public:
	void emit(){
		sub_thread=std::thread{[this](){
			return loop.run(0);
		}};
		return sub_thread.detach();
	}
	~loop_thread() {
		//or send async event & wait sub_thread exit
		loop.break_loop();
	}

};

template<typename instance_type>
class io_worker:private loop_thread{
	ev::async loop_sync{loop};
	std::mutex acpt_fd_mutex;
	std::queue<int> acpt_fd_queue;
	void async_cb(ev::async &watcher, int revents){
#ifdef DEBUG_FD_LIVETIME
		output<<"group:"<<this<<"->async_cb()"<<'\n';
#endif
		{
			std::lock_guard<std::mutex> lock{acpt_fd_mutex};
			//set other val after consume or need std::memory_order_acquire
#ifdef DEBUG_FD_SETUP
			if(acpt_fd_queue.empty()){
				debug<<"iogroup setup callback get fds empty 0\n";
				return;
			}
#endif
			while(!acpt_fd_queue.empty()){
				new instance_type{acpt_fd_queue.front(),loop};
				acpt_fd_queue.pop();
			}
		}
	}
public:
	io_worker(){
		loop_sync.set<io_worker,&io_worker::async_cb>(this);
		loop_sync.start();
		emit();
	}

	void setup(int fd,const struct sockaddr_in &client_addr){
		{
			std::lock_guard<std::mutex> lock{acpt_fd_mutex};
#ifdef DEBUG_FD_SETUP
			output<<"iogroup setup cover unsetup fd count == "<<acpt_fd_queue.size()<<'\n';
#endif
			acpt_fd_queue.push(fd);
		}
		loop_sync.send();
	}
	

	bool pending(){
		return loop_sync.async_pending();
	}
};

template<typename instance_type>
class server:public noncopyable{
protected:
	ev::default_loop loop;
private:
	ev::io io{loop};
	ev::sig sio{loop};
	std::vector<io_worker<instance_type> > io_workers{static_cast<unsigned long>(sysconf(_SC_NPROCESSORS_ONLN))};
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
