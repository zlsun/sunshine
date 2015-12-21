#ifndef ZLOG_H
#define ZLOG_H

#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <utility>
#include <boost/format.hpp>

#include "zcommon.h"

NS_ZL_BEGIN

template <typename SequenceT>
std::ostream& printSequence(std::ostream& out, const SequenceT& seq)
{
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
std::ostream& printMap(std::ostream& out, const MapT& map)
{
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
std::ostream& operator << (std::ostream& out, const std::vector<T>& vec)
{
    return zl::printSequence(out, vec);
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::list<T>& vec)
{
    return zl::printSequence(out, vec);
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::initializer_list<T>& ils)
{
    return zl::printSequence(out, ils);
}

template <typename K, typename V>
std::ostream& operator << (std::ostream& out, const std::pair<K, V>& p)
{
    return out << "<" << p.first << ", " << p.second << ">";
}

template <typename K, typename V>
std::ostream& operator << (std::ostream& out, const std::map<K, V>& map)
{
    return zl::printMap(out, map);
}

class DummyLogger
{
public:
    template <typename T>
    DummyLogger& operator , (T&& t)
    {
        return *this;
    }
};

class Logger
{
private:
    std::ostream& out;
    bool addNewline;
    bool addSpace;
    bool space = false;

public:
    Logger(
        std::ostream& out = std::cout,
        bool addNewline = true,
        bool addSpace = true
    )
        : out(out)
        , addNewline(addNewline)
        , addSpace(addSpace)
    {
    }

    ~Logger()
    {
        if (addNewline) out << std::endl;
    }

    template <typename T>
    Logger& operator , (T&& t)
    {
        printSpace();
        print(std::forward<T>(t));
        return *this;
    }

private:
    void printSpace()
    {
        if (addSpace && space) out << " ";
        else space = true;
    }

    template <typename T>
    void print(T&& t)
    {
        out << std::forward<T>(t);
    }

    // add support for array because it's ambiguous to overloading 'operator<<' for array
    template <typename T, size_t N>
    void print(const T (&array)[N])
    {
        if (N == 0) {
            out << "[]";
            return;
        }
        out << "[" << array[0];
        for (int i = 1; i < N; ++i) {
            out << ", " << array[i];
        }
        out << "]";
    }

    // special case of const T (&array)[N], print the string directly
    template <size_t N>
    void print(const char (&str)[N])
    {
        out << str;
    }
};

class FormatLogger
{
private:
    boost::format formatter;
    std::ostream& out;
    bool addNewline;

public:
    FormatLogger(
        const char* format,
        std::ostream& out = std::cout,
        bool addNewline = true
    )
        : formatter(format)
        , out(out)
        , addNewline(addNewline)
    {}

    ~FormatLogger()
    {
        out << formatter;
        if (addNewline) out << std::endl;
    }

    template <typename T>
    FormatLogger& operator , (T&& t)
    {
        formatter % std::forward<T>(t);
        return *this;
    }

};

NS_ZL_END


#ifdef NDEBUG
# define zlog            (zl::DummyLogger()),
# define zlogg(args...)  (zl::DummyLogger()),
# define zlogo(args...)  (zl::DummyLogger()),
# define zlogf(args...)  (zl::DummyLogger()),
# define zlogfo(args...) (zl::DummyLogger()),
#else
# define zlog                         (zl::Logger()),
# define zlogg(args...)               (zl::Logger(std::cout,##args)),
# define zlogo(out, args...)          (zl::Logger(out,##args)),
# define zlogf(format, args...)       (zl::FormatLogger(format, std::cout,##args)),
# define zlogfo(format, out, args...) (zl::FormatLogger(format, out,##args)),
#endif

#endif // ZLOG_H
