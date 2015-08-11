#include "server.hh"
#include "instance.hh"
#include "chacha20.hh"

using namespace ssev;

int main(int argc,char*argv[]){
	server<instance<ssocket5<chacha20> > > S{443,"127.0.0.1"};
	S.run();
	return 0;
}
