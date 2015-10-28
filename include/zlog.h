#ifndef ZLOG_H
#define ZLOG_H

#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <utility>
#include <boost/format.hpp>

namespace zl {

template <typename SequenceT>
std::ostream& printSequence(std::ostream& out, const SequenceT& seq) {
    if (seq.size() == 0) {
        return out << "[]";
    }
    auto it = seq.begin(), end = seq.end();
    out << "[" << *it;
    while (++it != end) {
        out << ", " << *it;
    }
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
    while (++it != map.end()) {
        out << ", " << it->first << ": " << it->second;
    }
    out << "}";
    return out;
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vec) {
    return zl::printSequence(out, vec);
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::list<T>& vec) {
    return zl::printSequence(out, vec);
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::initializer_list<T>& ils) {
    return zl::printSequence(out, ils);
}

template <typename K, typename V>
std::ostream& operator << (std::ostream& out, const std::pair<K, V>& p) {
    return out << "<" << p.first << ", " << p.second << ">";
}

template <typename K, typename V>
std::ostream& operator << (std::ostream& out, const std::map<K, V>& map) {
    return zl::printMap(out, map);
}

class DummyLogger {
public:
    template <typename T>
    DummyLogger& operator , (T&& t) {
        return *this;
    }
};

class Logger {
private:
    std::ostream& out;
    bool space;

public:
    Logger(std::ostream& out = std::cout): out(out), space(false) {}

    ~Logger() {
        out << std::endl;
    }

    template <typename T>
    Logger& operator , (T&& t) {
        if (space) out << " ";
        else space = true;
        out << std::forward<T>(t);
        return *this;
    }

    // add support for array because it's ambiguous to overloading 'operator<<' for array
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

    // special case of const T (&array)[N], print the string directly
    template <size_t N>
    Logger& operator , (const char (&string)[N]) {
        if (space) out << " ";
        else space = true;
        out << string;
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
    FormatLogger& operator , (T&& t) {
        formatter % std::forward<T>(t);
        return *this;
    }

};

} // namespace zl


#ifdef NDEBUG
# define zlog           (zl::DummyLogger()),
# define zlogo(out)     (zl::DummyLogger()),
# define zlogf(format)  (zl::DummyLogger()),
#else
# define zlog           (zl::Logger()),
# define zlogo(out)     (zl::Logger(out)),
# define zlogf(args...) (zl::FormatLogger(args)),
#endif

#endif // ZLOG_H
