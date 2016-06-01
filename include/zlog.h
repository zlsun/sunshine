#ifndef ZLOG_H
#define ZLOG_H

#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <queue>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <utility>

#include "zcommon.h"
#include "zinit.h"
#include "zstatic.h"
#include "zconcept.h"
#include "zattribute.h"

NS_ZL_BEGIN

template <typename SequenceT>
std::ostream& printContainer(std::ostream& out, const SequenceT& seq, char open, char close)
{
    if (seq.size() == 0) {
        return out << open << close;
    }
    auto it = seq.begin(), end = seq.end();
    out << open << *it;
    while (++it != end) {
        out << ", " << *it;
    }
    out << close;
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

template <typename Tuple, std::size_t... Is>
void print_tuple(std::ostream& os, const Tuple & t, std::index_sequence<Is...>)
{
    using swallow = int[]; // guaranties left to right order
    (void)swallow{0, (void(os << (Is == 0? "" : ", ") << std::get<Is>(t)), 0)...};
}

NS_ZL_END

namespace std
{

#define DEF_FOR_CONTAINER(C, open, close) \
    template <typename T>\
    std::ostream& operator << (std::ostream& out, const C<T>& c)\
    {\
        return zl::printContainer(out, c, open, close);\
    }

    // DEF_FOR_CONTAINER(std::array, '[', ']')
    DEF_FOR_CONTAINER(std::vector, '[', ']')
    DEF_FOR_CONTAINER(std::deque, '[', ']')
    DEF_FOR_CONTAINER(std::list, '[', ']')
    DEF_FOR_CONTAINER(std::set, '{', '}')
    DEF_FOR_CONTAINER(std::initializer_list, '[', ']')

#undef DEF_FOR_CONTAINER

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

    template <typename... Args>
    std::ostream& operator<<(std::ostream& out, const std::tuple<Args...>& t)
    {
        out << "(";
        zl::print_tuple(out, t, std::index_sequence_for<Args...>{});
        return out << ")";
    }

}

NS_ZL_BEGIN

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

#ifdef ENABLE_ZLOG_TO
private:
    static std::ostream* os;
public:
    static void to(std::ostream& o)
    {
        os = &o;
    }
#else
private:
    constexpr static std::ostream* os = &std::cout;
public:
    static void to(std::ostream& o)
    {
    }
#endif

private:
    bool addNewline;
    bool addSpace;
    bool space = false;

public:
    Logger(bool addNewline = true, bool addSpace = true)
        : addNewline(addNewline), addSpace(addSpace)
    {
    }

    ~Logger()
    {
        if (os && addNewline) *os << std::endl;
    }

    template <typename T>
    Logger& operator , (T&& t)
    {
        if (os) {
            printSpace();
            print(std::forward<T>(t));
        }
        return *this;
    }

private:
    void printSpace()
    {
        if (addSpace && space) *os << " ";
        else space = true;
    }

    template <typename T>
    CONCEPT_CHECK(CanCout<T>)
    print(T&& t)
    {
        *os << std::forward<T>(t);
    }
    template <typename T>
    CONCEPT_CHECK(!CanCout<T>)
    print(T&& t)
    {
        *os << "<unknown>";
    }

    // for array, because it's ambiguous to overloading 'operator<<' for array
    template <typename T, size_t N>
    void print(const T (&array)[N])
    {
        if (N == 0) {
            *os << "[]";
            return;
        }
        *os << "[" << array[0];
        for (int i = 1; i < N; ++i) {
            *os << ", " << array[i];
        }
        *os << "]";
    }

    // for const T (&array)[N], print the string directly
    template <size_t N>
    void print(const char (&str)[N])
    {
        *os << str;
    }
};

#ifdef ENABLE_ZLOG_TO
#ifdef WEAK_LINKED
WEAK_LINKED std::ostream* Logger::os = &std::cout;
#endif
#endif

NS_ZL_END

#ifdef DISABLE_ZLOG
# define zlog       (zl::DummyLogger()),
# define zloga(...) (zl::DummyLogger()),
#else
# define zlog       (zl::Logger()),
# define zloga(...) (zl::Logger(__VA_ARGS__)),
#endif

#endif // ZLOG_H
