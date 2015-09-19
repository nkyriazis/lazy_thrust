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

#include "transform.hpp"
#include <thrust/iterator/constant_iterator.h> 

namespace lazy_thrust
{
    template < class scalar >
    auto make_scalar(const scalar &s, size_t size)
        RETURN(make_lazy(thrust::make_constant_iterator(s),
                         thrust::make_constant_iterator(s) + size))

#define BINOP(name, op)                                         \
    namespace detail                                            \
    {                                                           \
        struct name                                             \
        {                                                       \
            template < class T, class U >                       \
            __host__ __device__                                 \
            auto operator()(T a, U b) const RETURN(a op b)      \
        };                                                      \
    }

#define BINOP_IMPL(name, op)                                                                                                \
    template < class iter1, class iter2 >                                                                                   \
    auto make_##name(const lazy<iter1> &r1, const lazy<iter2> &r2) RETURN(transform(join(r1, r2), detail::name()))          \
    template < class iter, class scalar >                                                                                   \
    auto make_##name##_lazy_scalar(const lazy<iter> &r, const scalar &s) RETURN(make_##name(r, make_scalar(s, r.size())))   \
    template < class scalar, class iter >                                                                                   \
    auto make_##name##_scalar_lazy(const scalar &s, const lazy<iter> &r) RETURN(make_##name(make_scalar(s, r.size()), r))   \
    template < class iter1, class iter2 >                                                                                   \
    auto operator op(const lazy<iter1> &r1, const lazy<iter2> &r2) RETURN(make_##name(r1, r2))                              \
    template < class iterator, class scalar >                                                                               \
    auto operator op(const lazy<iterator> &r, const scalar &s) RETURN(make_##name##_lazy_scalar(r, s))                      \
    template < class scalar, class iterator >                                                                               \
    auto operator op(const scalar &s, const lazy<iterator> &r) RETURN(make_##name##_scalar_lazy(s, r))                      \

#define UNOP(name, op)                                          \
    namespace detail                                            \
    {                                                           \
        struct name                                             \
        {                                                       \
            template < class T >                                \
            __host__ __device__                                 \
            auto operator()(T a) const RETURN(op a)             \
        };                                                      \
    }

#define UNOP_IMPL(name, op)                                                     \
    template < class iter >                                                     \
    auto make_##name(const lazy<iter> &r) RETURN(transform(r, detail::name()))  \
    template < class iter >                                                     \
    auto operator op(const lazy<iter> &r) RETURN(make_##name(r))                \


#define IMPLEMENT_BIN_OP(name, op) BINOP(name, op) BINOP_IMPL(name, op)
#define IMPLEMENT_UN_OP(name, op) UNOP(name, op) UNOP_IMPL(name, op)

    IMPLEMENT_BIN_OP(plus, +)
    IMPLEMENT_BIN_OP(minus, -)
    IMPLEMENT_BIN_OP(multiplies, *)
    IMPLEMENT_BIN_OP(divides, /)
    IMPLEMENT_BIN_OP(equal_to, ==)
    IMPLEMENT_BIN_OP(not_equal_to, !=)
    IMPLEMENT_BIN_OP(less, <)
    IMPLEMENT_BIN_OP(less_equal, <=)
    IMPLEMENT_BIN_OP(greater, >)
    IMPLEMENT_BIN_OP(greater_equal, >=)
    IMPLEMENT_BIN_OP(logical_or, ||)
    IMPLEMENT_BIN_OP(logical_and, && )
    IMPLEMENT_BIN_OP(bitwise_or, |)
    IMPLEMENT_BIN_OP(bitwise_and, &)
    IMPLEMENT_BIN_OP(bitwise_xor, ^)
    
    IMPLEMENT_UN_OP(negate, -)
    IMPLEMENT_UN_OP(identity, +)
    IMPLEMENT_UN_OP(not, !)

#undef BINOP
#undef BINOP_IMPL
#undef IMPLEMENT_BIN_OP
#undef UNOP
#undef UNOP_IMPL
#undef IMPLEMENT_UN_OP
}