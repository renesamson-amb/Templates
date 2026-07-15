/**
 * TUPLE implementation
 */

#pragma once

#include <memory>
#include <type_traits>
#include <functional>

#include "Metaprogramming.h"

namespace bits_of_q
{
    template<typename... ELEMS>
    struct Tuple
    {
        constexpr Tuple() = default;
    };

    template<typename ELEM0, typename... ELEMS1toN>
    struct Tuple<ELEM0, ELEMS1toN...> : Tuple<ELEMS1toN...>
    {
        explicit constexpr Tuple(ELEM0 e1, ELEMS1toN... rest) :
                    Tuple<ELEMS1toN...>(rest ...)
                    , data(e1)
        {}
        ELEM0 data;
    };

    // Following is a deduction guide
    template<typename T, typename ...Ts>
    Tuple(T e1, Ts... rest) -> Tuple<std::unwrap_ref_decay_t<T> , std::unwrap_ref_decay_t<Ts>...>;

    template<typename... ELEMS>
    auto make_tuple(ELEMS&&... elems)
    {
        return Tuple<std::unwrap_ref_decay_t<ELEMS>...>(std::forward<ELEMS>(elems)...);
    }

    ///////////////////////////// Get ////////////////////////////////////////////
    namespace detail{
        template<size_t i, typename TUPLE>
        struct get_impl: get_impl<i-1, pop_front_t<TUPLE>>
        {};
        template<typename TUPLE>
        struct get_impl<0, TUPLE>
        {
            template<typename T>
            constexpr static decltype(auto) get(T&& t)
            {
                constexpr bool is_lvalue = std::is_lvalue_reference_v<T>;
                constexpr bool is_const = std::is_const_v<std::remove_reference_t<T>>;

                using data_t = front_t<TUPLE>;

                if constexpr (is_lvalue && is_const)
                    return static_cast<const data_t&>(static_cast<const TUPLE&>(t).data);
                if constexpr (is_lvalue && !is_const)
                    return static_cast<data_t&>(static_cast<TUPLE&>(t).data);
                if constexpr (!is_lvalue && !is_const)
                    return static_cast<data_t&&>(static_cast<TUPLE&&>(t).data);
                if constexpr (!is_lvalue && is_const)
                    return static_cast<const data_t&&>(static_cast<const TUPLE&&>(t).data);
            }
        };
    }
    // namespace detail
    template<size_t i, typename TUPLE>
    constexpr decltype(auto) get(TUPLE&& tuple) // ---> Perfect forwarding applied, instead of simply a reference its a rvalue ref...
    {
        return detail::get_impl<i, std::remove_cvref_t<TUPLE>>::get(std::forward<TUPLE>(tuple));
    }
} // namespace_bits_of_q
