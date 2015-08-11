#include "instance.hh"
#include "chacha20.hh"

namespace ssev{

template <typename protocol>
instance<protocol>::instance(int fd,const ev::dynamic_loop &l){

}
template <typename protocol>
instance<protocol>::~instance(){

}

};

template class ssev::instance<ssev::ssocket5<ssev::chacha20>>;
