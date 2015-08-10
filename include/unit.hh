#include <iostream>
#include <memory>

#define output std::cerr<<__LINE__<<':'<<__FUNCTION__<<"\t"

namespace ssev{

struct noncopyable{
	noncopyable()=default;
	noncopyable(const noncopyable&) = delete;//noncopyable
	noncopyable& operator=(const noncopyable&) = delete;//noncopyable
};

};
