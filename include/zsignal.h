#ifndef ZSIGNAL_H
#define ZSIGNAL_H

#include <set>
#include <memory>
#include "zfunction.h"
#include "zlazy.h"

NS_ZL_BEGIN

template <typename F>
class Signal {
public:
    using SlotType = zl::Function<F>;
    using SlotResultType = typename SlotType::result_type;
    using SlotDecl = std::shared_ptr<SlotType>;
    using SlotSet = std::set<SlotDecl>;
    using This = Signal<F>;
private:
    SlotSet slots;
public:
    template <typename Func>
    SlotDecl connect(Func f) {
        SlotDecl decl(new SlotType(f));
        slots.insert(decl);
        return decl;
    }
    bool remove(SlotDecl decl) {
        auto it = slots.find(decl);
        if (it != slots.end()) {
            slots.erase(it);
            return true;
        }
        return false;
    }
    template <typename Func>
    This& operator << (Func f) {
        connect(f);
        return *this;
    }
    This& operator >> (SlotDecl f) {
        remove(f);
        return *this;
    }
    template <typename... Args, typename ResultType = SlotResultType>
    typename std::enable_if<
        std::is_void<ResultType>::value,
        void
    >::type
    invoke(Args&&... args) const {
        for (const auto& s : slots) {
            (*s)(std::forward<Args>(args)...);
        }
    }
    template <typename... Args, typename ResultType = SlotResultType>
    typename std::enable_if<
        !std::is_void<ResultType>::value,
        ResultType
    >::type
    invoke(Args&&... args) const {
        LazyInit<SlotResultType> result;
        for (const auto& s : slots) {
            result = (*s)(std::forward<Args>(args)...);
        }
        return result;
    }

};

NS_ZL_END

#endif // ZSIGNAL_H
