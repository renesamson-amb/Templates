/**
 * Main function
 */

#include "Metaprogramming.h"
#include "Tuple.h"

namespace bits_of_q {} // namespace bits_of_q

int main()
{
    using namespace bits_of_q;

    [[maybe_unused]]auto tup = make_tuple(8, true, 1.5);
    std::cout << tup.data<< "\n";

    Tuple tup2{5, false, 4};
    std::cout <<  get<1, decltype(tup2)>(tup2) << std::endl;
    std::cout << tup2.data << std::endl;

    const Tuple tup3{5, true, 10};
    std::cout << get<2>(tup2) << std::endl;
    std::cout << tup.data << std::endl;

    // Now Rvalues Tuples ...
     std::cout << get<2>(Tuple {10, false, 15}) << std::endl;

    return 0;
}
