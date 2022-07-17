#ifndef CCLOX_UTILS_H
#define CCLOX_UTILS_H

#include <string_view>
#include <string>
#include <memory>

namespace cclox {
    template<typename ... Args>
    std::string string_format(std::string_view format, Args ... args) {
        int size = 1 + snprintf(nullptr, 0, format.data(), args ...);  // Extra space for \0
        if (size <= 0)
            return "";
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.data(), args ...);
        return std::string{buf.get(), buf.get() + size - 1};
    }
}

#endif //CCLOX_UTILS_H
