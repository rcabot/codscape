#pragma once
#include <sstream>
#include <string>
#include <vector>

namespace util
{
    inline std::vector<std::string> split(const std::string& s, const char delim) {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, delim)) {
            result.push_back(item);
        }

        return result;
    }

    constexpr uint32_t hash(const char* data, const size_t size) noexcept {
        uint32_t hash = 5381;

        for (auto c = data; c < data + size; ++c)
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(*c);

        return hash;
    }

    inline void log_format()
    {
        std::cout << std::endl;
    }

	template<typename T, typename...Params>
	void log_format(T&& a, Params&&... args)
    {
        std::cout << a;
    	if(sizeof...(args)!=0)
    	{
            std::cout << " ";
    	}
        log_format(std::forward<Params>(args)...);
    }
}
