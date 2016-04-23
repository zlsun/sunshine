#ifndef ZPROPERTY_H
#define ZPROPERTY_H

#include <functional>

#include "zcommon.h"

NS_ZL_BEGIN

struct EmptyAddition {};

#define DEFAULT_GETTER [](Property& prop) -> Object& { return prop.getObj(); }
#define DEFAULT_SETTER [](Property& prop, const Object& newObj) { prop.getObj() = newObj; }

template <class Object, class Addition = EmptyAddition>
class Property
{
public:
    using Getter = std::function<Object&(Property&)>;
    using Setter = std::function<void(Property&, const Object&)>;
private:
    Object obj;
    Addition add;
    Getter getter = DEFAULT_GETTER;
    Setter setter = DEFAULT_SETTER;
public:

    Property() {}
    Property(const Object& o): obj(o) {}

    Property(const Getter& get): getter(get) {}
    Property(const Object& o, const Getter& get): obj(o), getter(get) {}

    Property(const Setter& set): setter(set) {}
    Property(const Object& o, const Setter& set): obj(o), setter(set) {}

    Property(const Getter& get, const Setter& set): getter(get), setter(set) {}
    Property(const Object& o, const Getter& get, const Setter& set)
        : obj(o), getter(get), setter(set) {}

    Object& getObj() { return obj; }
    const Object& getObj() const { return obj; }

    Addition& getAdd() { return add; }
    const Addition& getAdd() const { return add; }

    Object& get() { return getter(*this); }
    const Object& get() const { return getter(const_cast<Object&>(*this)); }

    void set(const Object& newObj) { setter(*this, newObj); }

    operator Object () { return get(); }

    Property& operator = (const Object& newObj)
    {
        set(newObj);
        return *this;
    }
};

#undef DEFAULT_GETTER
#undef DEFAULT_SETTER

NS_ZL_END

#endif // ZPROPERTY_H