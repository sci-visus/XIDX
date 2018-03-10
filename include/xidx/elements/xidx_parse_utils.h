

#ifndef XIDX_PARSE_UTILS_H_
#define XIDX_PARSE_UTILS_H_

#include <string>

namespace xidx{
  
template<typename ... Args>
static std::string string_format(const std::string& format, Args ... args){
    size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args ...);
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size);
}

}
#endif
