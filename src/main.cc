#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

int main(int argc,char* argv[]){
	if (argc != 4){
		std::cerr << "Usage: client <host> <port> <url>\n";
		return 1;
	}
	try{
		boost::asio::io_service io_service;
		boost::asio::ip::tcp::resolver resolver(io_service);
		boost::asio::ip::tcp::resolver::query query(argv[1], argv[2]);
		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
		boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv1);
	}catch(std::exception& e){
		std::cerr << "Exception: " << e.what() << "\n";
		return 1;
	}
	return 0;
}
