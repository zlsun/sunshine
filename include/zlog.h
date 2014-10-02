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
#include <boost/format.hpp>

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
    boost::format formatter;

public:
    FormatLogger(const char* format, std::ostream& out = std::cout):
        out(out), formatter(format) {}

    ~FormatLogger() {
        out << formatter << std::endl;
    }

    template <typename T>
    FormatLogger& operator , (const T& t) {
        formatter % t;
        return *this;
    }

};

template <typename SequenceT>
std::ostream& printSequence(std::ostream& out, const SequenceT& seq) {
    if (seq.size() == 0) {
        return out << "[]";
    }
    auto it = seq.begin(), end = seq.end();
    out << "[" << *it;
    while (++it != end)
        out << ", " << *it;
    out << "]";
    return out;
}

template <typename MapT>
std::ostream& printMap(std::ostream& out, const MapT& map) {
    if (map.size() == 0) {
        return out << "{}";
    }
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
    return zlsun::printSequence(out, vec);
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::initializer_list<T>& ils) {
    return zlsun::printSequence(out, ils);
}

template <typename K, typename V>
std::ostream& operator << (std::ostream& out, const std::map<K, V>& map) {
    return zlsun::printMap(out, map);
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
