#ifndef ZALGO_H
#define ZALGO_H

#include <algorithm>

// define all algorithm wrapper functions in std namespace
namespace std {

// a useful macro, don't undefine this macro
#define BEG_END(ctn) begin(ctn), end(ctn)

// macros for generating code
#define VREF(T) T&
#define CREF(T) const T&
#define GEN(NAME, ALGO) NAME##_IMPL(VREF, ALGO) NAME##_IMPL(CREF, ALGO)

// for ALGO(Iter, Iter, ...)
#define GEN_II_IMPL(IN, ALGO)                           \
    template <typename ContainerT, typename... Args>    \
    inline auto ALGO(IN(ContainerT) ctn, Args... args)  \
        -> decltype(std::ALGO(BEG_END(ctn), args...)) { \
             return std::ALGO(BEG_END(ctn), args...);   \
    }
#define GEN_II(ALGO) GEN(GEN_II, ALGO)

// for ALGO(Iter1, Iter1, Iter2...)
#define GEN_IIT_IMPL(IN, ALGO)                                                  \
    template <typename ContainerT_1, typename ContainerT_2, typename... Args>   \
    inline auto ALGO(IN(ContainerT_1) ctn, ContainerT_2& tgt, Args... args)     \
        -> decltype(std::ALGO(BEG_END(ctn), begin(tgt), args...)) {             \
             return std::ALGO(BEG_END(ctn), begin(tgt), args...);               \
    }
#define GEN_IIT(ALGO) GEN(GEN_IIT, ALGO)

// for ALGO(Iter, Iter, Iter...)
#define GEN_III_IMPL(IN, ALGO)                                          \
    template <typename ContainerT, typename Iter, typename... Args>     \
    inline auto ALGO(IN(ContainerT) ctn, Iter mid, Args... args)        \
        -> decltype(std::ALGO(begin(ctn), mid, end(ctn), args...)) {    \
             return std::ALGO(begin(ctn), mid, end(ctn), args...);      \
    }
#define GEN_III(ALGO) GEN(GEN_III, ALGO)

// for ALGO(Iter1, Iter1, Iter1, Iter2...)
#define GEN_IIIT_IMPL(IN, ALGO)                                                                 \
    template <typename ContainerT_1, typename Iter, typename ContainerT_2, typename... Args>    \
    inline auto ALGO(IN(ContainerT_1) ctn, Iter mid, ContainerT_2& tgt, Args... args)           \
        -> decltype(std::ALGO(begin(ctn), mid, end(ctn), begin(tgt), args...)) {                \
             return std::ALGO(begin(ctn), mid, end(ctn), begin(tgt), args...);                  \
    }
#define GEN_IIIT(ALGO) GEN(GEN_IIIT, ALGO)

// for ALGO(Iter1, Iter1, Iter2, Iter2...)
#define GEN_IITT_IMPL(IN, ALGO)                                                     \
    template <typename ContainerT_1, typename ContainerT_2, typename... Args>       \
    inline auto ALGO(IN(ContainerT_1) ctn1, IN(ContainerT_2) ctn2, Args... args)    \
        -> decltype(std::ALGO(BEG_END(ctn1), BEG_END(ctn2), args...)) {             \
             return std::ALGO(BEG_END(ctn1), BEG_END(ctn2), args...);               \
    }
#define GEN_IITT(ALGO) GEN(GEN_IITT, ALGO)

// for ALGO(Iter1, Iter1, Iter2, Iter2, Iter3...)
#define GEN_IITTE_IMPL(IN, ALGO)                                                                        \
    template <typename ContainerT_1, typename ContainerT_2, typename ContainerT_3, typename... Args>    \
    inline auto ALGO(IN(ContainerT_1) ctn1, IN(ContainerT_2) ctn2, ContainerT_3& tgt, Args... args)     \
        -> decltype(std::ALGO(BEG_END(ctn1), BEG_END(ctn2), begin(tgt), args...)) {                     \
             return std::ALGO(BEG_END(ctn1), BEG_END(ctn2), begin(tgt), args...);                       \
    }
#define GEN_IITTE(ALGO) GEN(GEN_IITTE, ALGO)

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

// wrapper function of transform(Iter1, Iter1, Iter2, Iter3, BinaryOperation),
// it can be confused with transform(Iter1, Iter1, Iter2, UnaryOperation),
// so rename it to transform_2
template <
    typename ContainerT_1,
    typename ContainerT_2,
    typename ContainerT_3,
    typename BinaryOperation
>
inline auto transform_2(
    const ContainerT_1& ctn1,
    const ContainerT_2& ctn2,
          ContainerT_3& tgt,
    BinaryOperation binary_op
)
-> decltype(transform(BEG_END(ctn1), begin(ctn2), begin(tgt), binary_op)){
     return transform(BEG_END(ctn1), begin(ctn2), begin(tgt), binary_op);
}

// shift operators for getting iterator
template <typename ContainerT>
inline auto operator >> (ContainerT& ctn, size_t n)
-> decltype(begin(ctn) + n) {
     return begin(ctn) + n;
}

template <typename ContainerT>
inline auto operator << (ContainerT& ctn, size_t n)
-> decltype(end(ctn) - n) {
     return end(ctn) - n;
}

// undefine all macros for generating code
#undef VREF
#undef CREF
#undef GEN

#undef GEN_II_IMPL
#undef GEN_II
#undef GEN_IIT_IMPL
#undef GEN_IIT
#undef GEN_III_IMPL
#undef GEN_III
#undef GEN_IIIT_IMPL
#undef GEN_IIIT
#undef GEN_IITT_IMPL
#undef GEN_IITT
#undef GEN_IITT_IMPL
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