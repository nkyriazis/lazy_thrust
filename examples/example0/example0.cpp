//  including operators includes everything
#include <lazy_thrust/operators.hpp>

//  the lazy_thrust namespace
using namespace lazy_thrust;

int main(int argc, char **argv)
{
    //  A container which supports iteration through std::begin/std::end
    int values[] = { 0, 1, 2, 3, 4, 5 };
   
    //  Simple lazy
    auto r = make_lazy(values);
}