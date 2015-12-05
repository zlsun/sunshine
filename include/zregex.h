#ifndef ZREGEX_H
#define ZREGEX_H

#include <regex>

#include "zstring.h"

#define FORWARD_METHOD_TO(reciver, method)                  \
    template <typename... Args>                             \
    decltype(auto) method(Args&&... args) {                 \
        return reciver.method(std::forward<Args>(args)...); \
    }
#define FORWARD_TO_REGEX(method) FORWARD_METHOD_TO(regex, method)

namespace zl {

template <
    typename CharT,
    typename Traits = std::regex_traits<CharT>
    >
class BasicRegex {
private:
    using RegexT = std::basic_regex<CharT, Traits>;
    using IListT  = std::initializer_list<CharT>;
    RegexT regex;
public:
    class Flag {
    private:
        using T = std::regex_constants::syntax_option_type;
    public:
        static const T ICase      = std::regex_constants::icase;
        static const T NoSubs     = std::regex_constants::nosubs;
        static const T Optimize   = std::regex_constants::optimize;
        static const T Collate    = std::regex_constants::collate;
        static const T ECMAScript = std::regex_constants::ECMAScript;
        static const T Basic      = std::regex_constants::basic;
        static const T Extended   = std::regex_constants::extended;
        static const T Awk        = std::regex_constants::awk;
        static const T Grep       = std::regex_constants::grep;
        static const T EGrep      = std::regex_constants::egrep;
    public:
        T flag;
    public:
        Flag(T f = ECMAScript): flag(f) {}
        operator T() const {
            return flag;
        }
    };
    class Error {
    private:
        using T = std::regex_constants::error_type;
    public:
        static const T Collate    = std::regex_constants::error_collate;
        static const T CType      = std::regex_constants::error_ctype;
        static const T Escape     = std::regex_constants::error_escape;
        static const T BackRef    = std::regex_constants::error_backref;
        static const T Brack      = std::regex_constants::error_brack;
        static const T Paren      = std::regex_constants::error_paren;
        static const T Brace      = std::regex_constants::error_brace;
        static const T BadBrace   = std::regex_constants::error_badbrace;
        static const T Range      = std::regex_constants::error_range;
        static const T Space      = std::regex_constants::error_space;
        static const T BadRepeat  = std::regex_constants::error_badrepeat;
        static const T Complexity = std::regex_constants::error_complexity;
        static const T Stack      = std::regex_constants::error_stack;
    public:
        T error;
    public:
        Error(T e): error(e) {}
        operator T() const {
            return error;
        }
    };
    class Match {
    private:
        using T = std::regex_constants::match_flag_type;
    public:
        static const T Default    = std::regex_constants::match_default;
        static const T NotBOL     = std::regex_constants::match_not_bol;
        static const T NotEOL     = std::regex_constants::match_not_eol;
        static const T NotBOW     = std::regex_constants::match_not_bow;
        static const T NotEOW     = std::regex_constants::match_not_eow;
        static const T Any        = std::regex_constants::match_any;
        static const T NotNull    = std::regex_constants::match_not_null;
        static const T Continuous = std::regex_constants::match_continuous;
        static const T PrevAvail  = std::regex_constants::match_prev_avail;
    public:
        T match;
    public:
        Match(T m): match(m) {}
        operator T() const {
            return match;
        }
    };
    class Format {
    private:
        using T = std::regex_constants::match_flag_type;
    public:
        static const T Default   = std::regex_constants::format_default;
        static const T Sed       = std::regex_constants::format_sed;
        static const T NoCopy    = std::regex_constants::format_no_copy;
        static const T FirstOnly = std::regex_constants::format_first_only;
    public:
        T format;
    public:
        Format(T f): format(f) {}
        operator T() const {
            return format;
        }
    };

    BasicRegex() {}
    explicit BasicRegex(const CharT* s, Flag f = Flag::ECMAScript)
        : regex(s, f) {}
    BasicRegex(const CharT* s, std::size_t count, Flag f = Flag::ECMAScript)
        : regex(s, count, f) {}
    BasicRegex(const BasicRegex& other)
        : regex(other.regex) {}
    BasicRegex(BasicRegex&& other)
        : regex(std::move(other.regex)) {}
    template <typename ST, typename SA>
    BasicRegex(BasicString<CharT, ST, SA> s, Flag f = Flag::ECMAScript)
        : regex(s, f) {}

    BasicRegex& operator = (const BasicRegex& other) {
        return assign(other);
    }
    BasicRegex& operator = (BasicRegex&& other) {
        return assign(std::move(other));
    }
    BasicRegex& operator = (IListT ilist) {
        return assign(std::move(ilist));
    }

    BasicRegex& assign(const BasicRegex& other) {
        regex.assign(other.regex);
        return *this;
    }
    BasicRegex& assign(BasicRegex&& other) {
        regex.assign(std::move(other.regex));
        return *this;
    }
    BasicRegex& assign(IListT ilist) {
        regex.assign(std::move(ilist));
        return *this;
    }

    unsigned mark_count() const {
        return regex.mark_count();
    }

    Flag flags() const {
        return (Flag)regex.flags();
    }

    void swap(BasicRegex& other) {
        regex.swap(other.regex);
    }

    friend void swap(BasicRegex<CharT, Traits>& lhs, BasicRegex<CharT, Traits>& rhs) {
        return std::swap(lhs, rhs);
    }

    // regex_match

    template <class BiIter>
    bool is_match(BiIter first, BiIter last, Match flags = Match::Default) {
        return std::regex_match(first, last, regex, flags);
    }
    bool is_match(const CharT* str, Match flags = Match::Default) {
        return is_match(str, str + Traits::length(str), flags);
    }
    template <class STraits, class SAlloc>
    bool is_match(const std::basic_string<CharT, STraits, SAlloc>& s, Match flags = Match::Default) {
        return is_match(s.begin(), s.end(), flags);
    }
    template <class STraits, class SAlloc>
    bool is_match(const BasicString<CharT, STraits, SAlloc>& s, Match flags = Match::Default) {
        return is_match(s.begin(), s.end(), flags);
    }

    template <class BiIter, class Alloc = std::allocator<CharT>>
    decltype(auto)
    match(BiIter first, BiIter last, Match flags = Match::Default) {
        using MatchResult = std::match_results<BiIter, Alloc>;
        MatchResult result;
        std::regex_match(first, last, result, regex, flags);
        return result;
    }
    decltype(auto) match(const CharT* str, Match flags = Match::Default) {
        return match(str, str + Traits::length(str), flags);
    }
    template <class STraits, class SAlloc>
    decltype(auto) match(const std::basic_string<CharT, STraits, SAlloc>& s, Match flags = Match::Default) {
        return match(s.begin(), s.end(), flags);
    }
    template <class STraits, class SAlloc>
    decltype(auto) match(const BasicString<CharT, STraits, SAlloc>& s, Match flags = Match::Default) {
        return match(s.begin(), s.end(), flags);
    }

    // regex_search

    template <class BiIter>
    bool found(BiIter first, BiIter last, Match flags = Match::Default) {
        return std::regex_search(first, last, regex, flags);
    }
    bool found(const CharT* str, Match flags = Match::Default) {
        return found(str, str + Traits::length(str), flags);
    }
    template <class STraits, class SAlloc>
    bool found(const std::basic_string<CharT, STraits, SAlloc>& s, Match flags = Match::Default) {
        return found(s.begin(), s.end(), flags);
    }
    template <class STraits, class SAlloc>
    bool found(const BasicString<CharT, STraits, SAlloc>& s, Match flags = Match::Default) {
        return found(s.begin(), s.end(), flags);
    }

    template <class BiIter, class Alloc = std::allocator<CharT>>
    std::match_results<BiIter, Alloc>
    search(BiIter first, BiIter last, Match flags = Match::Default) {
        using MatchResult = std::match_results<BiIter, Alloc>;
        MatchResult result;
        std::regex_search(first, last, result, regex, flags);
        return result;
    }
    decltype(auto) search(const CharT* str, Match flags = Match::Default) {
        return search(str, str + Traits::length(str), flags);
    }
    template <class STraits, class SAlloc>
    decltype(auto) search(const std::basic_string<CharT, STraits, SAlloc>& s, Match flags = Match::Default) {
        return search(s.begin(), s.end(), flags);
    }
    template <class STraits, class SAlloc>
    decltype(auto) search(const BasicString<CharT, STraits, SAlloc>& s, Match flags = Match::Default) {
        return search(s.begin(), s.end(), flags);
    }

    // regex_replace

    template <class OutIter, class BiIter>
    OutIter
    replace(OutIter out, BiIter first, BiIter last,
            const CharT* fmt,
            Match flags = Match::Default) {
        return std::regex_replace(out, first, last, regex, fmt, flags);
    }
    template <class OutIter, class BiIter, class SAlloc>
    OutIter
    replace(OutIter out, BiIter first, BiIter last,
            const BasicString<CharT, Traits, SAlloc>& fmt,
            Match flags = Match::Default) {
        return std::regex_replace(out, first, last, regex, fmt, flags);
    }

    BasicString <CharT>
    replace(const CharT* s,
            const CharT* fmt,
            Match flags = Match::Default) {
        return std::regex_replace(s, regex, fmt, flags);
    }
    template <class FTraits, class FAlloc>
    BasicString <CharT>
    replace(const CharT* s,
            const BasicString<CharT, FTraits, FAlloc>& fmt,
            Match flags = Match::Default) {
        return std::regex_replace(s, regex, fmt, flags);
    }

    template <class STraits, class SAlloc>
    BasicString <CharT, STraits, SAlloc>
    replace(const BasicString<CharT, STraits, SAlloc>& s,
            const CharT* fmt,
            Match flags = Match::Default) {
        return std::regex_replace(s.c_str(), regex, fmt, flags);
    }
    template <class STraits, class SAlloc, class FTraits, class FAlloc>
    BasicString <CharT, STraits, SAlloc>
    replace(const BasicString<CharT, STraits, SAlloc>& s,
            const BasicString<CharT, FTraits, FAlloc>& fmt,
            Match flags = Match::Default) {
        return std::regex_replace(s.c_str(), regex, fmt, flags);
    }

};

using Regex = BasicRegex<char>;
using WRegex = BasicRegex<wchar_t>;

Regex operator "" _r (const char* s, std::size_t len) {
    return Regex(s, len);
}

WRegex operator "" _wr (const wchar_t* s, std::size_t len) {
    return WRegex(s, len);
}

}

#endif // ZREGEX_H
