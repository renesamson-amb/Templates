/**
 * Main function
 */

#include "Metaprogramming.h"
#include "Tuple.h"
// We add the Test utilities
#include "TestUtilities.h"

namespace bits_of_q {} // namespace bits_of_q

int main()
{
    using namespace bits_of_q;

    [[maybe_unused]]auto tup = make_tuple(8, true, 1.5);
    std::cout << tup.data<< "\n";

    Tuple tup2{5, false, 4};
    std::cout <<  get<1>(tup2) << std::endl;
    std::cout << tup2.data << std::endl;

    // const Tuple tup3{5, true, 10};
    // std::cout << get<2>(tup2) << std::endl;
    // std::cout << tup.data << std::endl;

    // Now Rvalues Tuples ...
    std::cout << get<2>(Tuple {10, false, 15}) << std::endl;

    // Finally we are tyring to assign a value to a type...
    get<2>(tup) = 4.2; // Expression is not assignable...
    std::cout << get<2>(tup) << std::endl;

    // Copycounter
    CopyCounter c;
    CopyCounter::reset();

    // The following are gonna set 1 to default constructor, copy constructor and move constructor    
    // CopyCounter c2;
    // c2 = c;
    // c2 = std::move(c2);
    // std::cout << c2.stats << "\n";
    // The result is 
    // {default constructs : 1, copies:1, moves:1}

    make_tuple(8, c, 1.5);
    std::cout << c.stats << "\n"; //{default constructs : 0, copies:4, moves:0}



    return 0;
}
