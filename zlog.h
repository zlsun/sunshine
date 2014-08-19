#ifndef ZLOG_H
#define ZLOG_H

#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <cstdio>
#include <cstring>
#include <cctype>

namespace zlsun {

class Logger {
private:
    bool space;
    std::ostream& out;

public:

    Logger(std::ostream& out = std::cout): out(out), space(false) {}

    ~Logger() {
        out << std::endl;
    }

    template <typename T, size_t N>
    Logger& operator , (const T (&array)[N]) {
        if (space) out << " ";
        else space = true;
        if (N == 0) {
            out << "[]";
            return *this;
        }
        out << "[" << array[0];
        for (int i = 1; i < N; ++i) {
            out << ", " << array[i];
        }
        out << "]";
        return *this;
    }

    template <size_t N>
    Logger& operator , (const char (&string)[N]) {
        if (space) out << " ";
        else space = true;
        out << string;
        return *this;
    }

    template <typename T>
    Logger& operator , (const T& t) {
        if (space) out << " ";
        else space = true;
        out << t;
        return *this;
    }

    template <typename T>
    Logger& operator , (const std::initializer_list<T>& ils) {
        if (space) out << " ";
        else space = true;
        out << ils;
        return *this;
    }

};

class FormatLogger {
private:
    std::ostream& out;
public:
    FormatLogger(const char* format, std::ostream& out = std::cout): out(out) {
        int size = std::strlen(format);
        buffer = new char[size];
        std::strcpy(buffer, format);
    }
    ~FormatLogger() {
        out << buffer << std::endl;
        delete[] buffer;
    }
    template <typename T>
    FormatLogger& operator , (const T& t) {
        int size = std::strlen(buffer);
        char* new_buffer = new char[size * 2];
        std::memset(new_buffer, 0, size * 2);
        generate(new_buffer, buffer, t);
        std::swap(new_buffer, buffer);
        delete[] new_buffer;
        return *this;
    }
private:
    bool valid(char c) {
        static const char list[] = "dcfl";
        for (int i = 0; i < std::strlen(list); ++i)
            if (c == list[i])
                return true;
        return false;
    }
    template <typename T>
    void generate(char* new_buffer, char* format, const T& t) {
        bool first = true;
        while (char c = *format++) {
            if (first && c == '%' && format && *format != '%') {
                char* fmt = new char[100];
                std::memset(fmt, 0, 100);
                char* pfmt = fmt;
                *pfmt++ = c;
                while (format && !valid(*format)) {
                    *pfmt++ = *format++;
                }
                char* buf;
                if (format) {
                    *pfmt++ = *format++;
                    buf = new char[100];
                    std::memset(buf, 0, 100);
                    std::sprintf(buf, fmt, t);
                    delete[] fmt;
                } else {
                    buf = fmt;
                }
                std::strcat(new_buffer, buf);
                new_buffer += std::strlen(buf);
                delete[] buf;
                first = false;
            } else {
                *new_buffer++ = c;
            }
        }
    }
private:
    char* buffer;
};

template <typename SequenceT>
std::ostream& print_sequence(std::ostream& out, const SequenceT& seq) {
    if (seq.size() == 0) return out << "[]";
    auto it = seq.begin(), end = seq.end();
    out << "[" << *it;
    while (++it != end)
        out << ", " << *it;
    out << "]";
    return out;
}

template <typename MapT>
std::ostream& print_map(std::ostream& out, const MapT& map) {
    if (map.size() == 0) return out << "{}";
    auto it = map.begin();
    out << "{" << it->first << ": " << it->second;
    while (++it != map.end())
        out << ", " << it->first << ": " << it->second;
    out << "}";
    return out;
}

}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vec) {
    return zlsun::print_sequence(out, vec);
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::initializer_list<T>& ils) {
    return zlsun::print_sequence(out, ils);
}

template <typename K, typename V>
std::ostream& operator << (std::ostream& out, const std::map<K, V>& map) {
    return zlsun::print_map(out, map);
}

#ifdef NDEBUG
# define zlog           //
# define zlogo(out)     //
# define zlogf(format)  //
#else
# define zlog           zlsun::Logger(),
# define zlogo(out)     (zlsun::Logger(out)),
# define zlogf(args...) zlsun::FormatLogger(args),
#endif

#endif // ZLOG_H
