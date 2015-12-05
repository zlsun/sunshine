#ifndef ZPATH_H
#define ZPATH_H

#include <boost/filesystem.hpp>

#include "zalgo.h"
#include "zstring.h"

namespace zl {

namespace detail {
    using namespace boost::system;
    using namespace boost::filesystem;
}

#define MODIFIER_FORWARD(name)              \
    template <class... Args>                \
    Path& name(Args... args) {              \
        detail::path::name(args...);        \
        return *this;                       \
    }

#define DECOMPOSITION_FORWARD(name)         \
    template <class... Args>                \
    Path name(Args... args) const {         \
        return detail::path::name(args...); \
    }

#define OPERATIONAL_FORWARD(name)                           \
    template <class... Args>                                \
    auto name(Args... args) const {                         \
        return detail::name(get_path(), args...);           \
    }

class Path : public detail::path {
public:
    template <class... Args>
    Path(Args... args): detail::path(args...) {}
    Path(const BasicString<value_type>& str): Path(str.c_str()) {}

    detail::path& get_path() {
        return *this;
    }
    const detail::path& get_path() const {
        return *this;
    }

    MODIFIER_FORWARD(make_preferred)
    MODIFIER_FORWARD(remove_filename)

    Path& replace_extension(const Path& new_ext = Path()) {
        detail::path::replace_extension(new_ext.get_path());
        return *this;
    }

    DECOMPOSITION_FORWARD(root_name)
    DECOMPOSITION_FORWARD(root_directory)
    DECOMPOSITION_FORWARD(root_path)
    DECOMPOSITION_FORWARD(relative_path)
    DECOMPOSITION_FORWARD(parent_path)
    DECOMPOSITION_FORWARD(filename)
    DECOMPOSITION_FORWARD(stem)
    DECOMPOSITION_FORWARD(extension)

    static Path current_path() {
        return detail::current_path();
    }

    Path absolute(const Path& base = current_path()) {
        return detail::absolute(get_path(), base.get_path());
    }

    OPERATIONAL_FORWARD(canonical)
    Path canonical(const Path& base = current_path()) {
        return detail::canonical(get_path(), base.get_path());
    }

    OPERATIONAL_FORWARD(exists)
    OPERATIONAL_FORWARD(copy)
    OPERATIONAL_FORWARD(create_directories)
    OPERATIONAL_FORWARD(equivalent)
    OPERATIONAL_FORWARD(file_size)
    OPERATIONAL_FORWARD(hard_link_count)
    OPERATIONAL_FORWARD(is_directory)
    OPERATIONAL_FORWARD(is_empty)
    OPERATIONAL_FORWARD(is_regular_file)
    OPERATIONAL_FORWARD(is_other)
    OPERATIONAL_FORWARD(is_symlink)
    OPERATIONAL_FORWARD(last_write_time)
    OPERATIONAL_FORWARD(permissions)
    OPERATIONAL_FORWARD(read_symlink)
    OPERATIONAL_FORWARD(remove)
    OPERATIONAL_FORWARD(remove_all)
    OPERATIONAL_FORWARD(rename)
    OPERATIONAL_FORWARD(resize_file)
    OPERATIONAL_FORWARD(space)
    OPERATIONAL_FORWARD(status)
    OPERATIONAL_FORWARD(status_known)
    OPERATIONAL_FORWARD(symlink_status)
    OPERATIONAL_FORWARD(system_complete)

    template <class... Args>
    static Path temp_directory_path(Args... args) {
        return detail::temp_directory_path(args...);
    }
    template <class... Args>
    static Path unique_path(Args... args) {
        return detail::unique_path(args...);
    }
    static Path unique_path(const Path& model="%%%%-%%%%-%%%%-%%%%") {
        return detail::unique_path(model);
    }
};

Path operator "" _path (const char* s, size_t len) {
    return Path(s);
}

}

#endif // ZPATH_H
