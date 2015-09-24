# lazy_thrust

Easily compose operations on ranges of values. The evaluation is delayed, thus the name "lazy".

## Motivation

The [boost library](http://www.boost.org/) and the [thrust library](https://github.com/thrust/thrust) include (fancy) iterators which facilitate implicit and/or delayed computations. While these iterators help in fitting even more classes of ranges in standard algorithms, they incur a severe increase in locs. The most important examples are the following:

### counting_iterator

An iterator which when evaluated returns a number in a range.

This example:
```cpp
thrust::copy(
    thrust::make_counting_iterator(0),
    thrust::make_counting_iterator(10),
    std::ostream_iterator<int>(std::cout, " "));
```
Yields this output:
<tt>0 1 2 3 4 5 6 7 8 9</tt>

### transform_iterator

An iterator which wraps another iterator and when evaluated yields the result of the application of a unary function on the dereferenced value.

This example:
```cpp
auto pow = [](int a) { return a * a };
thrust::copy(
    thrust::make_transform_iterator(thrust::make_counting_iterator(0), pow),
    thrust::make_transform_iterator(thrust::make_counting_iterator(10), pow),
    std::ostream_iterator<int>(std::cout, " "));
```
Yields this output:
<tt>0 1 4 9 16 25 36 49 64 81</tt>

### permutation_iterator

An iterator which uses a range as indexes in another range and dereferences the latter in the order dictated by the former.

This example:
```cpp
int values = [] = { 10, 20, 30, 40, 50 };
int reorder[] = { 4, 3, 2, 1, 0 };
thrust::copy(
    thrust::make_permutation_iterator(std::begin(values), std::begin(reorder)),
    thrust::make_permutation_iterator(std::begin(values), std::end(reorder)),
    std::ostream_iterator<int>(std::cout, " "));
```
Yields this output:
<tt>50 40 30 20 10</tt>

### Bottom-line

The introduction of important features in C++11, such as lambdas, auto, decltype, variadic tempaltes, etc., introduces the ability to severely decrease the amount of code required to utilize implicit computations in iterator-based algorithms. This library aims exactly there.

## Contribution

This library facilitates the quick and easy construction of lazily evaluated iterator ranges. To do so it exploits C++11 features. The main use evolves around the use of a special operand, which is overloaded over common operations in order to easily synthesize computations. Some examples follow:

### Example: simple copy
#### thrust
```cpp
int values[] = { 0, 1, 2, 3, 4 };
int results[5];

thrust::copy(std::begin(values), std::end(values), std::begin(results))
```

#### lazy_thrust
```cpp
int values[] = { 0, 1, 2, 3, 4 };
int results[5];

using namespace lazy_thrust;

make_lazy(results) = make_lazy(values);
```

### Example: simple transform
#### thrust
```cpp
int values[] = { 0, 1, 2, 3, 4 };
int results[5];

thrust::transform(
    std::begin(values),
    std::end(values),
    std::begin(results),
    [](int i) { return 2 * i });
```

#### lazy_thrust
```cpp
int values[] = { 0, 1, 2, 3, 4 };
int results[5];

using namespace lazy_thrust;

make_lazy(results) = transform(make_lazy(values), [](int i) { return 2 * i });
```

### Example: zip transform
#### thrust
```cpp
int values1[] = { 0, 1, 2, 3, 4 };
int values2[] = { 0, 1, 2, 3, 4 };
int values3[] = { 0, 1, 2, 3, 4 };
int results[5];

auto iter = thrust::make_zip_iterator(thrust::make_tuple(
                std::begin(values1), std::begin(values2), std::begin(values3)));

thrust::transform(
    iter, iter + 5,
    std::begin(results),
    [](const thrust::tuple<int, int, int> &t)
    { return thrust::get<0>(t) + thrust::get<1>(t) + thrust::get<2>(t) });
```

#### lazy_thrust
```cpp
int values1[] = { 0, 1, 2, 3, 4 };
int values2[] = { 0, 1, 2, 3, 4 };
int values3[] = { 0, 1, 2, 3, 4 };
int results[5];

using namespace lazy_thrust;

make_lazy(results) = transform(join(make_lazy(values1),
                                    make_lazy(values2),
                                    make_lazy(values3)),
                                //  notice how the tuple is expanded into arguments
                                //  instead of a cumbersome thrust::tuple
                                [](int a, int b, int c) { return a + b + c; })
```

### Example: common operators
#### thrust
```cpp
int values1[] = { 0, 1, 2, 3, 4 };
int values2[] = { 0, 1, 2, 3, 4 };
int values3[] = { 0, 1, 2, 3, 4 };
int results[5];

auto iter = thrust::make_zip_iterator(thrust::make_tuple(
                std::begin(values1), std::begin(values2), std::begin(values3)));

thrust::transform(
    iter, iter + 5,
    std::begin(results),
    [](const thrust::tuple<int, int, int> &t)
    { return thrust::get<0>(t) + thrust::get<1>(t) + thrust::get<2>(t) });
```

#### lazy_thrust
```cpp
int values1[] = { 0, 1, 2, 3, 4 };
int values2[] = { 0, 1, 2, 3, 4 };
int values3[] = { 0, 1, 2, 3, 4 };
int results[5];

using namespace lazy_thrust;

make_lazy(results) = make_lazy(values1) + make_lazy(values2) + make_lazy(values3);
```
