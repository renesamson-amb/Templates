/**
* Lesson 3
*/
#include <vector>
#include <iostream>
#include <assert.h>
#include <cmath>
#include <tuple>
#include <utility>

/** The third function
* An overload to cover the empty list
*/
void printn()
{
    std::cout << "\n";
}


/** The second overload
An overload for printing the last element.....
If this function is listed below the original then the original
would not see this and this would result in the same error
So this should be placed first
*/
template<typename LAST>
void printn(LAST L)
{
    std::cout << L << "\n";
}



/** This is a function
Note the ... indicates that there are multiple arguments follo
*/
template<typename T0, typename ... T1toN> // Before a name: Variable number of elements (Here is the typenames)
void printn(T0 t, T1toN... rest) // Between names: define second name as a list of params given by the first
{
    std::cout << t << ", ";
    printn(rest...);    // after name: expand he name to a list of all elements it represents...
}



/*******************************************************************
**************************SECOND PART*******************************
********************************************************************/

template<typename TUPLE, std::size_t... indices>
void print_tuple_impl(TUPLE t, std::index_sequence<indices...>)
{
    printn(std::get<indices>(t)...);
    /**
    * The above translates to std::get<0>(t), std::get<1>(t) ... and so on...
    */
}

/**
* Print Tuple is a tuple printer
*/
template<typename TUPLE>
void print_tuple(TUPLE t)
{
    print_tuple_impl(t, std::make_index_sequence<std::tuple_size<TUPLE>::value>{});
}



/*******************************************************************
**************************THIRD PART********************************
********************************************************************/

/**
* Tuple printer implementation
*/

template<typename LAST>
void printfn(LAST&& l)
{
    std::cout << l << std::endl;
}

template<typename T0, typename... T1toN>
void printfn(T0&& t, T1toN... rest)
{
    std::cout << t << ", ";
    printfn(rest...);
}

template<typename TUPLE, std::size_t... indices>
void print_f_tuple_impl(TUPLE&& t, std::index_sequence<indices...>)
{
    printfn(std::get<indices>(std::forward<TUPLE>(t))...);
}

/**
* Print tuple
*/
template<typename TUPLE>
void print_f_tuple(TUPLE&& t)
{
    // We can't use std::tuple_size on a reference....
    // So we use the std::remove_reference_t to remove the reference
    print_f_tuple_impl(std::forward<TUPLE>(t), std::make_index_sequence<std::tuple_size<std::remove_reference_t<TUPLE>>::value> {} );
}

int main()
{
    printn(9, "hello", 1.5, true);
    // The following is not covered by the second overload,
    // so we have to have a third function that covers
    // empty list..
    printn();

    // for the second part let's write the tuple
    auto tuple = std::make_tuple(0, "hello", 1.5, true);
    print_tuple(tuple);
    printn();


    // Third part. We use perfect forwarding 
    const auto tup = std::make_tuple(1, "BOLO", 2.5, true);
    print_f_tuple(tup);
    printn();

    return 0;
}