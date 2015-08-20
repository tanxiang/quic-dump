#include "instance.hh"
#include "chacha20.hh"

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
void instance<protocol>::tcp_sendto(std::string& data){
}
template <typename protocol>
void instance<protocol>::udp_sendto(std::string& data){
}
template <typename protocol>
void instance<protocol>::client_send(std::string& data){
}

};

template class ssev::instance<ssev::ssocket5<ssev::chacha20>>;
