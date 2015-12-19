#ifndef ZALGO_H
#define ZALGO_H

#include <algorithm>
#include <iterator>

// define all algorithm wrapper functions in std namespace
namespace std {

// macros for generating code

#define FW(T, t)      std::forward<T>(t)
#define FWB(T, ctn)   std::begin(FW(T, ctn))
#define FWE(T, ctn)   std::end(FW(T, ctn))
#define FWBE(T, ctn)  FWB(T, ctn), FWE(T, ctn)
#define FW_ARGS       FW(Args, args)...

// ALGO(Iter, Iter, ...)
#define GEN_II(ALGO)                                        \
    template <typename T, typename... Args>                 \
    inline auto ALGO(T&& ctn, Args&&... args)               \
        -> decltype(std::ALGO(FWBE(T, ctn), FW_ARGS)) {     \
             return std::ALGO(FWBE(T, ctn), FW_ARGS);       \
    }

// ALGO(Iter1, Iter1, Iter2...)
#define GEN_IIT(ALGO)                                                   \
    template <typename T1, typename T2, typename... Args>               \
    inline auto ALGO(T1&& ctn, T2&& tgt, Args&&... args)                \
        -> decltype(std::ALGO(FWBE(T1, ctn), FWB(T2, tgt), FW_ARGS)) {  \
             return std::ALGO(FWBE(T1, ctn), FWB(T2, tgt), FW_ARGS);    \
    }

// ALGO(Iter, Iter, Iter...)
#define GEN_III(ALGO)                                                               \
    template <typename T, typename Iter, typename... Args>                          \
    inline auto ALGO(T&& ctn, Iter&& mid, Args&&... args)                           \
        -> decltype(std::ALGO(FWB(T, ctn), FW(Iter, mid), FWE(T, ctn), FW_ARGS)) {  \
             return std::ALGO(FWB(T, ctn), FW(Iter, mid), FWE(T, ctn), FW_ARGS);    \
    }

// ALGO(Iter1, Iter1, Iter1, Iter2...)
#define GEN_IIIT(ALGO)                                                                              \
    template <typename T1, typename Iter, typename T2, typename... Args>                            \
    inline auto ALGO(T1&& ctn, Iter&& mid, T2&& tgt, Args&&... args)                                \
        -> decltype(std::ALGO(FWB(T1, ctn), FW(Iter, mid), FWE(T1, ctn), FWB(T2, tgt), FW_ARGS)) {  \
             return std::ALGO(FWB(T1, ctn), FW(Iter, mid), FWE(T1, ctn), FWB(T2, tgt), FW_ARGS);    \
    }

// ALGO(Iter1, Iter1, Iter2, Iter2...)
#define GEN_IITT(ALGO)                                                      \
    template <typename T1, typename T2, typename... Args>                   \
    inline auto ALGO(T1&& ctn1, T2&& ctn2, Args&&... args)                  \
        -> decltype(std::ALGO(FWBE(T1, ctn1), FWBE(T2, ctn2), FW_ARGS)) {   \
             return std::ALGO(FWBE(T1, ctn1), FWBE(T2, ctn2), FW_ARGS);     \
    }

// ALGO(Iter1, Iter1, Iter2, Iter2, Iter3...)
#define GEN_IITTE(ALGO)                                                                 \
    template <typename T1, typename T2, typename T3, typename... Args>                  \
    inline auto ALGO(T1&& ctn1, T2&& ctn2, T3&& tgt, Args&&... args)                    \
        -> decltype(std::ALGO(FWBE(T1, ctn1), FWBE(T2, ctn2), FWB(T2, tgt), FW_ARGS)) { \
             return std::ALGO(FWBE(T1, ctn1), FWBE(T2, ctn2), FWB(T2, tgt), FW_ARGS);   \
    }

#define GEN_IIX(ALGO)           GEN_II(ALGO)       GEN_IIT(ALGO)
#define GEN_IIIX(ALGO)          GEN_III(ALGO)      GEN_IIIT(ALGO)
#define GEN_IITTX(ALGO)         GEN_IITT(ALGO)     GEN_IITTE(ALGO)
#define GEN_IF_II(ALGO)         GEN_II(ALGO)       GEN_II(ALGO##_if)
#define GEN_IF_IIX(ALGO)        GEN_IIX(ALGO)      GEN_IIX(ALGO##_if)
#define GEN_COPY_II(ALGO)       GEN_II(ALGO)       GEN_IIX(ALGO##_copy)
#define GEN_COPY_III(ALGO)      GEN_III(ALGO)      GEN_IIIX(ALGO##_copy)
#define GEN_COPYIF_II(ALGO)     GEN_COPY_II(ALGO)  GEN_II(ALGO##_if)     GEN_IIX(ALGO##_copy_if)

// generate algorithm wrapper functions

GEN_II          (adjacent_find)
GEN_II          (all_of)
GEN_II          (any_of)
GEN_II          (binary_search)
GEN_IF_IIX      (copy)
GEN_IIX         (copy_backward)
GEN_IF_II       (count)
GEN_IIX         (equal)
GEN_II          (equal_range)
GEN_II          (fill)
GEN_IF_II       (find)
GEN_IITT        (find_end)
GEN_IITT        (find_first_of)
GEN_II          (find_if_not)
GEN_II          (for_each)
GEN_II          (generate)
GEN_IITT        (includes)
GEN_III         (inplace_merge)
GEN_II          (is_heap)
GEN_II          (is_heap_until)
GEN_II          (is_partitioned)
GEN_IIX         (is_permutation)
GEN_II          (is_sorted)
GEN_II          (is_sorted_until)
GEN_IITT        (lexicographical_compare)
GEN_II          (lower_bound)
GEN_II          (make_heap)
GEN_II          (max_element)
GEN_IITTX       (merge)
GEN_II          (min_element)
GEN_II          (minmax_element)
GEN_IIX         (mismatch)
GEN_II          (next_permutation)
GEN_II          (none_of)
GEN_III         (nth_element)
GEN_III         (partial_sort)
GEN_IITT        (partial_sort_copy)
GEN_II          (partition)
GEN_IITT        (partition_copy)
GEN_II          (partition_point)
GEN_II          (pop_heap)
GEN_II          (prev_permutation)
GEN_II          (push_heap)
GEN_II          (random_shuffle)
GEN_COPYIF_II   (remove)
GEN_COPYIF_II   (replace)
GEN_COPY_II     (reverse)
GEN_COPY_III    (rotate)
GEN_IITT        (search)
GEN_II          (search_n)
GEN_IITTX       (set_difference)
GEN_IITTX       (set_intersection)
GEN_IITTX       (set_symmetric_difference)
GEN_IITTX       (set_union)
GEN_II          (shuffle)
GEN_II          (sort)
GEN_II          (sort_heap)
GEN_II          (stable_partition)
GEN_II          (stable_sort)
GEN_IIX         (swap_ranges)
GEN_IIX         (transform)
GEN_COPY_II     (unique)
GEN_II          (upper_bound)

// wrapper function of transform(Iter1, Iter1, Iter2, Iter3, BinaryOp),
// it will be confused with transform(Iter1, Iter1, Iter2, UnaryOp),
// so rename it to transform2
template <typename T1, typename T2, typename T3, typename Op>
inline auto transform2(T1&& ctn1, T2&& ctn2, T3&& tgt, Op&& op)
-> decltype(std::transform(FWBE(T1, ctn1), FWB(T2, ctn2), FWB(T3, tgt), FW(Op, op))) {
     return std::transform(FWBE(T1, ctn1), FWB(T2, ctn2), FWB(T3, tgt), FW(Op, op));
}

// get offset iterator
template <typename T>
inline auto operator >> (T&& ctn, int n)
-> decltype(std::begin(ctn)) {
    auto it = std::begin(ctn);
    std::advance(it, n);
    return it;
}

template <typename T>
inline auto operator << (T&& ctn, int n)
-> decltype(std::end(ctn)) {
    auto it = std::end(ctn);
    std::advance(it, -n);
    return it;
}

// undefine all macros for generating code
#undef FW
#undef FWB
#undef FWE
#undef FWBE
#undef FW_ARGS

#undef GEN_II
#undef GEN_IIT
#undef GEN_III
#undef GEN_IIIT
#undef GEN_IITT
#undef GEN_IITT

#undef GEN_IIX
#undef GEN_IIIX
#undef GEN_IITTX
#undef GEN_IF_II
#undef GEN_IF_IIX
#undef GEN_COPY_II
#undef GEN_COPY_III
#undef GEN_COPYIF_II

} // namespace std

#endif // ZALGO_H
