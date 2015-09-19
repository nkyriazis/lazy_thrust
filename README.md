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
Yields thsi output:
<tt>50 40 30 20 10</tt>

### Bottom-line

The introduction of important features in C++11, such as lambdas, auto, decltype, variadic tempaltes, etc., introduces the ability to severely decrease the amount of code required to utilize implicit computations in iterator-based algorithms. This library aims exactly there.

## Contribution
