#ifndef UTIL_H
#define UTIL_H
#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <locale>
#include <codecvt>
#include <memory>
#include <string>
#include <stdexcept>


namespace util
{
    template<typename ... Args>
    inline std::string string_format( const std::string& format, Args ... args )
    {
        int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
        if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
        auto size = static_cast<size_t>( size_s );
        auto buf = std::make_unique<char[]>( size );
        std::snprintf( buf.get(), size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
    }

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

    inline std::wstring s2ws(const std::string& str)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.from_bytes(str);
    }

    inline std::string ws2s(const std::wstring& wstr)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.to_bytes(wstr);
    }
}
#endif