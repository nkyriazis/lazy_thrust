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

#include "config.hpp"
#include <utility>
#include <thrust/distance.h>
#include <thrust/copy.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/iterator/reverse_iterator.h>
#include <thrust/iterator/permutation_iterator.h>

namespace lazy_thrust
{
    template < class iterator_ >
    class lazy;

    template < class iterator >
    auto make_lazy(iterator first, iterator last)
        RETURN(lazy<iterator>(first, last))

    template < class range >
    auto make_lazy(range &r)
        RETURN(make_lazy(r.begin(), r.end()))

    template < class iterator_ >
    class lazy
        : std::pair<iterator_, iterator_>
    {
    public:
        typedef iterator_ iterator;
        typedef std::pair<iterator, iterator> base;

        template < class range >
        explicit lazy(range &r) : base(r.begin(), r.end()) {}

        template < class iterator >
        explicit lazy(iterator first, iterator last) : base(first, last) {}

        iterator begin() const { return first; }
        iterator end() const { return second; }
        auto size() const RETURN(thrust::distance(begin(), end()))

        template < class other_iterator >
        lazy& operator=(const lazy<other_iterator> &r)
        {
            thrust::copy(r.begin(), r.end(), begin());
            return *this;
        }

        template < class scalar >
        lazy& operator=(const scalar &s)
        {
            thrust::fill(begin(), end(), s);
            return *this;
        }

        template < class other_iterator >
        auto operator[](const lazy<other_iterator> &i) const
            RETURN(make_lazy(thrust::make_permutation_iterator(begin(), i.begin()),
                             thrust::make_permutation_iterator(begin(), i.end())))
    };

    auto make_range(size_t size)
        RETURN(make_lazy(thrust::make_counting_iterator(0),
                         thrust::make_counting_iterator(0) + size))

    template < class iterator >
    auto reverse(const lazy<iterator> &r)
        RETURN(make_lazy(thrust::make_reverse_iterator(r.end()),
            thrust::make_reverse_iterator(r.begin())))

}