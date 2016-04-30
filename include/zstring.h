#ifndef ZSTRING_H
#define ZSTRING_H

#include <cctype>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include "zcommon.h"
#include "zalgo.h"
#include "zutils.h"

NS_ZL_BEGIN

template <
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
>
class BasicString
    : public ImplementRelationalOperators<BasicString<CharT>>
{
private:
    using StringT = std::basic_string<CharT, Traits, Allocator>;
    using IListT  = std::initializer_list<CharT>;
    StringT str;
public:
    using traits_type            = typename StringT::traits_type;
    using value_type             = typename StringT::value_type;
    using allocator_type         = typename StringT::allocator_type;
    using size_type              = typename StringT::size_type;
    using difference_type        = typename StringT::difference_type;
    using reference              = typename StringT::reference;
    using const_reference        = typename StringT::const_reference;
    using pointer                = typename StringT::pointer;
    using const_pointer          = typename StringT::const_pointer;
    using iterator               = typename StringT::iterator;
    using const_iterator         = typename StringT::const_iterator;
    using reverse_iterator       = typename StringT::reverse_iterator;
    using const_reverse_iterator = typename StringT::const_reverse_iterator;

    static const size_type npos = StringT::npos;

public:
    BasicString() : BasicString(Allocator()) {}

    explicit BasicString(const Allocator& alloc) : str(alloc) {}

    BasicString(size_type count, CharT ch, const Allocator& alloc = Allocator())
        : str(count, ch, alloc) {}

    BasicString(const BasicString& other,
                size_type pos, size_type count = BasicString::npos,
                const Allocator& alloc = Allocator())
        : str(other, pos, count, alloc) {}

    BasicString(const CharT* s, size_type count) : str(s, count) {}

    BasicString(const CharT* s) : str(s) {}

    BasicString(CharT ch) : str(1, ch) {}

    template <typename InputIt>
    BasicString(InputIt first, InputIt last, const Allocator& alloc = Allocator())
        : str(first, last, alloc) {}

    BasicString(StringT s) : str(std::move(s)) {}
    BasicString(StringT s, const Allocator& alloc) : str(std::move(s), alloc) {}

    BasicString(IListT ilist, const Allocator& alloc = Allocator())
        : str(std::move(ilist), alloc) {}

    BasicString(const boost::basic_format<CharT, Traits, Allocator>& format)
        : str(boost::str(format)) {}

    BasicString(const BasicString& s) : str(s.str) {}
    BasicString(const BasicString& s, const Allocator& alloc) : str(s.str, alloc) {}
    BasicString(BasicString&& s) : str(std::move(s.str)) {}
    BasicString(BasicString&& s, const Allocator& alloc) : str(std::move(s.str), alloc) {}

    operator StringT () {
        return str;
    }
    operator StringT () const {
        return str;
    }

    BasicString& operator = (BasicString s)
    {
        return assign(std::move(s));
    }
    BasicString& operator = (IListT ilist)
    {
        return assign(std::move(ilist));
    }

    BasicString& assign(BasicString s)
    {
        str.assign(std::move(s.str));
        return *this;
    }
    BasicString& assign(IListT ilist)
    {
        str.assign(std::move(ilist));
        return *this;
    }

    allocator_type get_allocator() const
    {
        return str.get_allocator();
    }

    // Element Access

    reference at(size_type pos)
    {
        return str.at(pos);
    }
    const_reference at(size_type pos) const
    {
        return str.at(pos);
    }

    reference operator [] (size_type pos)
    {
        return str[pos];
    }
    const_reference operator [] (size_type pos) const
    {
        return str[pos];
    }

    CharT& front()
    {
        return str.front();
    }
    const CharT& front() const
    {
        return str.front();
    }

    CharT& back()
    {
        return str.back();
    }
    const CharT& back() const
    {
        return str.back();
    }

    const CharT* data() const
    {
        return str.data();
    }

    const CharT* c_str() const
    {
        return str.c_str();
    }

    // Iterators

    iterator begin()
    {
        return str.begin();
    }
    const_iterator begin() const
    {
        return str.begin();
    }
    const_iterator cbegin() const
    {
        return str.cbegin();
    }
    iterator end()
    {
        return str.end();
    }
    const_iterator end() const
    {
        return str.end();
    }
    const_iterator cend() const
    {
        return str.cend();
    }

    iterator rbegin()
    {
        return str.rbegin();
    }
    const_iterator rbegin() const
    {
        return str.rbegin();
    }
    const_iterator crbegin() const
    {
        return str.crbegin();
    }
    iterator rend()
    {
        return str.rend();
    }
    const_iterator rend() const
    {
        return str.rend();
    }
    const_iterator crend() const
    {
        return str.crend();
    }

    // Capacity

    bool empty() const
    {
        return str.empty();
    }
    size_type size() const
    {
        return str.size();
    }
    size_type length() const
    {
        return str.length();
    }
    size_type max_size() const
    {
        return str.max_size();
    }
    size_type capacity() const
    {
        return str.capacity();
    }

    void reserve(size_type new_cap = 0)
    {
        str.reserve(new_cap);
    }
    void shrink_to_fit()
    {
        str.shrink_to_fit();
    }

    // Operations

#define FORWARD_TO_STRING(method)                       \
    template <class... Args>                            \
    decltype(auto) method(Args&&... args)               \
    {                                                   \
        return str.method(std::forward<Args>(args)...); \
    }                                                   \
    template <class... Args>                            \
    decltype(auto) method(Args&&... args) const         \
    {                                                   \
        return str.method(std::forward<Args>(args)...); \
    }

#define FORWARD_AND_RETURN_THIS(method)          \
    template <class... Args>                     \
    BasicString& method(Args&&... args)          \
    {                                            \
        str.method(std::forward<Args>(args)...); \
        return *this;                            \
    }                                            \
    template <class... Args>                     \
    BasicString& method(Args&&... args) const    \
    {                                            \
        str.method(std::forward<Args>(args)...); \
        return *this;                            \
    }

    void clear()
    {
        str.clear();
    }

    FORWARD_AND_RETURN_THIS(insert)

    FORWARD_TO_STRING(erase)
    BasicString& erase(size_type index = 0, size_type count = npos)
    {
        return str.erase(index, count);
    }

    void push_back(CharT ch)
    {
        str.push_back(ch);
    }
    void pop_back()
    {
        str.pop_back();
    }

    FORWARD_AND_RETURN_THIS(append)
    BasicString& append(const StringT& str, size_type pos, size_type count = npos)
    {
        str.append(str, pos, count);
        return *this;
    }

    BasicString& operator += (const StringT& s)
    {
        str += s;
        return *this;
    }
    BasicString& operator += (const IListT& ilist)
    {
        str += ilist;
        return *this;
    }

    FORWARD_TO_STRING(compare)

    FORWARD_AND_RETURN_THIS(replace)

    BasicString substr(size_type pos = 0, size_type count = npos) const
    {
        return str.substr(pos, count);
    }

    size_type copy(CharT* dest, size_type count, size_type pos = 0) const
    {
        return str.copy(dest, count, pos);
    }

    void resize(size_type count) {
        str.resize(count);
    }

    void resize(size_type count, CharT ch) {
        str.resie(count, ch);
    }

    void swap(BasicString& other)
    {
        str.swap(other.str);
    }

#undef FORWARD_TO_STRING
#undef FORWARD_AND_RETURN_THIS

    // Search

    size_type find(const StringT& s, size_type pos = 0 ) const
    {
        return str.find(s, pos);
    }
    size_type find(const CharT* s, size_type pos, size_type count ) const
    {
        return str.find(s, pos, count);
    }
    size_type find(const CharT* s, size_type pos = 0 ) const
    {
        return str.find(s, pos);
    }
    size_type find(CharT ch, size_type pos = 0 ) const
    {
        return str.find(ch, pos);
    }

    // Non-member Functions

    friend BasicString operator + (const BasicString& a, const BasicString& b)
    {
        return a.str + b.str;
    }

    friend bool operator < (const BasicString& a, const BasicString& b)
    {
        return a.str < b.str;
    }
    friend bool operator == (const BasicString& a, const BasicString& b)
    {
        return a.str == b.str;
    }

    friend void swap(BasicString& a, BasicString& b)
    {
        std::swap(a, b);
    }

    friend decltype(auto) operator << (
        std::basic_ostream<CharT, Traits>& os,
        const BasicString& s
    )
    {
        return os << s.str;
    }
    friend decltype(auto) operator >> (
        std::basic_istream<CharT, Traits>& is,
        BasicString& str
    )
    {
        return is >> str.str;
    }

    friend decltype(auto) getline(
        std::basic_istream<CharT, Traits>& input,
        BasicString& str, CharT delim
    )
    {
        return std::getline(input, str.str, delim);
    }
    friend decltype(auto) getline(
        std::basic_istream<CharT, Traits>&& input,
        BasicString& str, CharT delim
    )
    {
        return std::getline(std::move(input), str.str, delim);
    }
    friend decltype(auto) getline(
        std::basic_istream<CharT, Traits>& input,
        BasicString& str
    )
    {
        return std::getline(input, str.str);
    }
    friend decltype(auto) getline(
        std::basic_istream<CharT, Traits>&& input,
        BasicString& str
    )
    {
        return std::getline(std::move(input), str.str);
    }

    // Extended Methods

    int count(char c) const
    {
        return std::count(str.begin(), str.end(), c);
    }

    BasicString lower() const
    {
        return boost::to_lower_copy(str);
    }
    BasicString upper() const
    {
        return boost::to_upper_copy(str);
    }
    BasicString capitalize() const
    {
        BasicString res = lower();
        res[0] = std::toupper(res[0]);
        return res;
    }

    BasicString trim() const
    {
        return boost::trim_copy(str);
    }
    BasicString trim_left() const
    {
        return boost::trim_left_copy(str);
    }
    BasicString trim_right() const
    {
        return boost::trim_right_copy(str);
    }

    bool startswith(const BasicString& s) const
    {
        return boost::starts_with(str, s.str);
    }
    bool endswith(const BasicString& s) const
    {
        return boost::ends_with(str, s.str);
    }

    bool contains(const BasicString& s) const
    {
        return boost::contains(str, s.str);
    }

    template <typename ContainerT>
    BasicString join(const ContainerT& t) const
    {
        return boost::join(t, str);
    }
    BasicString join(const std::initializer_list<BasicString>& t) const
    {
        return boost::join(t, str);
    }

    std::vector<BasicString> split(const BasicString& delim = " \n\r") const
    {
        std::vector<BasicString> v;
        boost::split(v, str, [&](CharT ch) {
            return delim.find(ch) != npos;
        });
        return v;
    }
    std::vector<BasicString> split(const CharT& delim) const
    {
        std::vector<BasicString> v;
        boost::split(v, str, [&](CharT ch) {
            return ch == delim;
        });
        return v;
    }

    BasicString replace_all(const StringT& p, const StringT& s)
    {
        return boost::replace_all_copy(str, p, s);
    }

    // slice

public:
    template <typename String>
    class IView : public ImplementRelationalOperators<IView<String>>
    {
    protected:
        String& str;
        size_type pos, count;
    public:
        IView(String& str, size_type pos, size_type count)
            : str(str), pos(pos), count(count) {}
        IView(String& str): View(str, 0, str.size()) {}
        operator BasicString () const
        {
            return str.substr(pos, count);
        }
        size_type length() const
        {
            return count;
        }
        size_type size() const
        {
            return count;
        }
        int compare(const IView& v) const
        {
            return str.compare(pos, count, v.str, v.pos, v.count);
        }
        friend bool operator < (const IView& a, const IView& b)
        {
            return a.compare(b) < 0;
        }
        friend bool operator == (const IView& a, const IView& b)
        {
            return a.compare(b) == 0;
        }
    };
    class ConstView : public IView<const BasicString>
    {
    private:
        using Base = IView<const BasicString>;
    public:
        using iterator = BasicString::const_iterator;
    public:
        ConstView(const BasicString& str, size_type pos, size_type count)
            : Base(str, pos, count) {}
        ConstView(const BasicString& str): Base(str, 0, str.size()) {}
        auto slice(int begin, int end) const
        {
            if (begin < 0) {
                begin = Base::length() + begin;
            }
            if (end < 0) {
                end = Base::length() + end;
            }
            begin = std::min(static_cast<size_type>(begin), Base::length());
            end = std::min(static_cast<size_type>(end), Base::length());
            return ConstView(Base::str, Base::pos + begin, end - begin);
        }
        auto slice(int begin) const
        {
            return slice(begin, Base::length());
        }
        iterator begin() const
        {
            return Base::str.cbegin() + Base::pos;
        }
        iterator end() const
        {
            return Base::str.cbegin() + Base::pos + Base::count;
        }
        const BasicString::value_type& operator [] (size_type index) const
        {
            return *(begin() + index);
        }
        friend decltype(auto) operator << (
            std::basic_ostream<CharT, Traits>& os,
            const ConstView& v
        )
        {
            return os << (BasicString)v;
        }
    };
    class View : public IView<BasicString>
    {
    private:
        using Base = IView<BasicString>;
    public:
        using iterator = BasicString::iterator;
    public:
        View(BasicString& str, size_type pos, size_type count)
            : Base(str, pos, count) {}
        View(BasicString& str): Base(str, 0, str.size()) {}
        View& operator = (const BasicString& s)
        {
            Base::str.replace(Base::pos, Base::count, s);
            return *this;
        }
        auto slice(int begin, int end) const
        {
            if (begin < 0) {
                begin = Base::length() + begin;
            }
            if (end < 0) {
                end = Base::length() + end;
            }
            begin = std::min(static_cast<size_type>(begin), Base::length());
            end = std::min(static_cast<size_type>(end), Base::length());
            return View(Base::str, Base::pos + begin, end - begin);
        }
        auto slice(int begin) const
        {
            return slice(begin, Base::length());
        }
        iterator begin() const
        {
            return Base::str.begin() + Base::pos;
        }
        iterator end() const
        {
            return Base::str.begin() + Base::pos + Base::count;
        }
        BasicString::value_type& operator [] (size_type index) const
        {
            return *(begin() + index);
        }
        friend decltype(auto) operator << (
            std::basic_ostream<CharT, Traits>& os,
            const View& v
        )
        {
            return os << (BasicString)v;
        }
    };

    auto slice(int begin, int end)
    {
        if (begin < 0) {
            begin = length() + begin;
        }
        if (end < 0) {
            end = length() + end;
        }
        begin = std::min(static_cast<size_type>(begin), length());
        end = std::min(static_cast<size_type>(end), length());
        return View(*this, begin, end - begin);
    }
    auto slice(int begin)
    {
        return slice(begin, length());
    }
    auto slice(int begin, int end) const
    {
        if (begin < 0) {
            begin = length() + begin;
        }
        if (end < 0) {
            end = length() + end;
        }
        begin = std::min(static_cast<size_type>(begin), length());
        end = std::min(static_cast<size_type>(end), length());
        return ConstView(*this, begin, end - begin);
    }
    auto slice(int begin) const
    {
        return slice(begin, length());
    }

};

using String = BasicString<char>;
using WString = BasicString<wchar_t>;

inline auto operator "" _s (const char* s, std::size_t len)
{
    return String(s, len);
}

inline auto operator "" _ws (const wchar_t* s, std::size_t len)
{
    return WString(s, len);
}

inline auto operator "" _f (const char* s, std::size_t len)
{
    return boost::format(s);
}

inline auto operator "" _wf (const wchar_t* s, std::size_t len)
{
    return boost::wformat(s);
}

NS_ZL_END

#endif // ZSTRING_H
