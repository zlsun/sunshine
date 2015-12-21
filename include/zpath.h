#ifndef ZPATH_H
#define ZPATH_H

#include <boost/filesystem.hpp>

#include "zcommon.h"
#include "zalgo.h"
#include "zstring.h"

NS_ZL_BEGIN

namespace detail {
    using namespace boost::system;
    using namespace boost::filesystem;
}

#define MODIFIER_FORWARD(name)                           \
    template <class... Args>                             \
    Path& name(Args&&... args) {                         \
        detail::path::name(std::forward<Args>(args)...); \
        return *this;                                    \
    }

#define DECOMPOSITION_FORWARD(name)                             \
    template <class... Args>                                    \
    Path name(Args&&... args) const {                           \
        return detail::path::name(std::forward<Args>(args)...); \
    }

#define OPERATIONAL_FORWARD(name)                                     \
    template <class... Args>                                          \
    auto name(Args&&... args) const {                                 \
        return detail::name(get_path(), std::forward<Args>(args)...); \
    }

class Path : public detail::path {
public:
    using detail::path::value_type;
    using detail::path::string_type;
    using detail::path::codecvt_type;

    template <class... Args>
    Path(Args&&... args): detail::path(std::forward<Args>(args)...) {}
    Path(const BasicString<value_type>& str): Path(str.c_str()) {}
    Path(BasicString<value_type>&& str): Path(str.c_str()) {}

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
    static Path temp_directory_path(Args&&... args) {
        return detail::temp_directory_path(std::forward<Args>(args)...);
    }
    template <class... Args>
    static Path unique_path(Args&&... args) {
        return detail::unique_path(std::forward<Args>(args)...);
    }
    static Path unique_path(const Path& model="%%%%-%%%%-%%%%-%%%%") {
        return detail::unique_path(model);
    }
};

Path operator "" _path (const char* s, size_t len) {
    return Path(s);
}

NS_ZL_END

#endif // ZPATH_H
