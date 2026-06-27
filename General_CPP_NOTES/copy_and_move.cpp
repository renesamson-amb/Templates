/*
A number is a perfect square, or a square number, if it is the square of a positive integer.
For example,  is a square number because    ; it is also an odd square.

The first 5 square numbers are: , and the sum of the odd squares is    .

Among the first 434 thousand square numbers, what is the sum of all the odd squares?
*/


#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <array>
#include <cmath>
#include <vector>
#include <algorithm>

// std::copy : A and B is the container
// std::copy (A.begin(), A.begin() + 3, B.begin());
//[ NOTE ] A.begin() + 3 means elements at 0, 1, 2 only...
// if B is const then this would not be valid
// This cannot be used to copy the values from one location range to another within the same
// vector as the result
// std::copy(A.begin(), A.begin() + 3, A.begin() + 1);
// This would update the second element in A with the first which is within the range of A.begin() to A.begin() + 3...

// std::copy_n(src begin, len, dst begin); -> It passes the length instead of the end range iterator...
// Many things we can do things like the following
// std::copy_n(std::istream_iterator<std:string>(std::cin), 3, B.begin());
// This is reallly cool....

// For both these methods there is a restriction, the data cannot be copied from the source range of A
// to within the source range of A. Only to range outside of the source destination...
// To overcome this restriction we could use copy_backward()

// std::copy_backward(A.begin(), A.begin() + 3, B.begin()+3); --> here B.begin()+3 is the end iterator....
// This copies the first 3 elements in a backwards order i.e... from index 2 in A to index 2 in B
// from index 1 in A to index 1 in A...
// from index 0 in A to index
// This helps us with one things, we can have the destination range overlap with the source range

// std::move -> Moves elements instead of copy. This is different to the std::move(my_variable)
//  -> std::move(A.begin(), A.begin() + 3, B.begin()); -> This is from <algorithm>
//  -> std::move(my_variable) -> this if from <utility>
// std::move_backward is also the same ....


// std::copy_if(A.begin(), A.begin() + 4, B.begin(), unary_predicate);
// example
// std::copy_if(A.begin(), A.begin() + 4, B.begin(), 
//              [](auto i) {return i%2 == 1;});
// This way the destination iterator is incremented only if the source iterator
// is divisible by 2....

// We do not have move_if, we could achieve the same with the following

int main()
{
    std::vector<int> A{1,2,3,4,5,6};
    std::vector<int> B;
    B.reserve(10);
    auto is_odd = [](auto i) { return i % 2 == 1;};
    std::copy_if(std::make_move_iterator(A.begin()), std::make_move_iterator(A.begin()+5), B.begin(), is_odd);
    return 0;
}
