//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/core/injector.hpp"
#include "boost/di/concepts/boundable.hpp"
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/concepts/configurable.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di { inline namespace v1 { namespace detail {

template<class>
void create(const std::true_type&) { }

template<class>
BOOST_DI_CONCEPTS_CREATABLE_ATTR
void
    create
(const std::false_type&) { }

template<class... T>
class injector<int, T...> : public core::injector<::BOOST_DI_CFG, core::pool<>, T...> {
public:
    template<class TConfig, class TPolicies, class... TDeps>
    injector(const core::injector<TConfig, TPolicies, TDeps...>& injector) noexcept // non explicit
        : core::injector<::BOOST_DI_CFG, core::pool<>, T...>(injector) {
            using injector_t = core::injector<TConfig, TPolicies, TDeps...>;
            int _[]{0, (
                detail::create<T>(
                    std::integral_constant<bool,
                        injector_t::template is_creatable<T>::value ||
                        injector_t::template is_creatable<T*>::value
                    >{}
                )
            , 0)...}; (void)_;
    }
};

} // namespace detail

template<class... T>
using injector = detail::injector<BOOST_DI_REQUIRES_TYPE(concepts::boundable<aux::type<T...>>), T...>;

}}} // boost::di::v1

#endif

