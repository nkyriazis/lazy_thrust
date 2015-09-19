/*
*  Copyright 2015 Nikolaos Kyriazis
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#pragma once

#include "multi.hpp"
#include <thrust/iterator/transform_iterator.h>
#include <thrust/iterator/discard_iterator.h>

namespace lazy_thrust
{
    namespace detail
    {
        template<int ...>
        struct seq { };

        template<int N, int ...S>
        struct gens : gens<N - 1, N - 1, S...> { };

        template<int ...S>
        struct gens<0, S...> { typedef seq<S...> type; };

        template<class function, class tuple, int ...S>
        auto callFunc(function fn, const tuple &params, seq<S...>)
            RETURN(fn(thrust::get<S>(params) ...))

        template < class function >
        struct expand_tuple
        {
            expand_tuple(function fn) : m_fn(fn) {}

            template < class tuple >
            auto operator()(const tuple &t) const
                RETURN(callFunc(m_fn, t, typename gens<thrust::tuple_size<tuple>::value>::type()))

            function m_fn;
        };

        template < class function >
        auto make_expand_tuple(function fn) RETURN(expand_tuple<function>(fn))

        template < class iterator >
        struct wrap_for_tuples
        {
            template < class function >
            static auto wrap(function fn) RETURN(fn)
        };

        template < class iterator_tuple >
        struct wrap_for_tuples<thrust::zip_iterator<iterator_tuple>>
        {
            template < class function >
            static auto wrap(function fn) RETURN(make_expand_tuple(fn))
        };
    }

    template < class iterator, class function >
    auto transform(const lazy<iterator> &r, function fn)
        RETURN(make_lazy(
            thrust::make_transform_iterator(r.begin(), detail::wrap_for_tuples<iterator>::wrap(fn)),
            thrust::make_transform_iterator(r.end(), detail::wrap_for_tuples<iterator>::wrap(fn))))

    template < class iterator, class function >
    auto for_each(const lazy<iterator> &r, function fn)
        RETURN(thrust::for_each(r.begin(), r.end(), detail::wrap_for_tuples<iterator>::wrap(fn)))
}