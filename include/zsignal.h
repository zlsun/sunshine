#ifndef ZSIGNAL_H
#define ZSIGNAL_H

#include <set>
#include <memory>

#include "zcommon.h"
#include "zfunction.h"
#include "ztraits.h"
#include "zlazy.h"

NS_ZL_BEGIN

template <typename F>
class Signal
{
public:
    using SlotType = F;
    using SlotResultType = typename function_traits<F>::result_type;
    using SlotDecl = F*;
    using SlotSet = std::set<SlotDecl>;
private:
    SlotSet slots;
public:
    void connect(const SlotDecl& decl)
    {
        slots.insert(decl);
    }
    bool disconnect(const SlotDecl& decl)
    {
        auto it = slots.find(decl);
        if (it != slots.end()) {
            slots.erase(it);
            return true;
        }
        return false;
    }
    void disconnect_all()
    {
        slots.clear();
    }
    Signal& operator << (const SlotDecl& f)
    {
        connect(f);
        return *this;
    }
    Signal& operator >> (const SlotDecl& f)
    {
        disconnect(f);
        return *this;
    }
    template <typename... Args, typename ResultType = SlotResultType, typename =
              typename std::enable_if<std::is_void<ResultType>::value>::type>
    void emit(Args&& ... args) const
    {
        for (const auto& s : slots) {
            (*s)(std::forward<Args>(args)...);
        }
    }
    template <typename... Args, typename ResultType = SlotResultType, typename =
              typename std::enable_if<!std::is_void<ResultType>::value>::type>
    ResultType emit(Args&& ... args) const
    {
        LazyInit<SlotResultType> result;
        for (const auto& s : slots) {
            result = (*s)(std::forward<Args>(args)...);
        }
        return result;
    }
    template <typename... Args, typename ResultType = SlotResultType, typename =
              typename std::enable_if<std::is_void<ResultType>::value>::type>
    void operator () (Args&& ... args) const
    {
        emit(std::forward<Args>(args)...);
    }
    template <typename... Args, typename ResultType = SlotResultType, typename =
              typename std::enable_if<!std::is_void<ResultType>::value>::type>
    ResultType operator () (Args&& ... args) const
    {
        return emit(std::forward<Args>(args)...);
    }
};

NS_ZL_END

#endif // ZSIGNAL_H
