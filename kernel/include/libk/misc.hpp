#ifndef LIBK_MISC_HPP
#define LIBK_MISC_HPP

template<class T, T v>
struct integral_constant
{
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant; // using injected-class-name
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; } // since c++14
};

template<class T, class U>
struct is_same : integral_constant<bool, false> {};

template<class T>
struct is_same<T, T> : integral_constant<bool, true> {};

#endif