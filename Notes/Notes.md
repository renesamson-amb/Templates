## LESSON 3 & 4
in the previous lesson we used 
std::tuple_element_t<#Index, TUPLE>

Note: 

    std::tuple<int, float, bool> is a tuple of collection of
    type <int, float, bool>. So std::tuple_element_t will return the type of the element in the particular index...


We are going to write our own version of getting the type of an element for a given index

In this lesson we want to use __Variadic templates__




NOTES:

    std::forward should be used with a template type
    std::forward<TUPLE>(t);

    The above passing should be handled by the receiving function
    as shown below
    void handle(TUPLE&& t)

    ## index_sequence

    In C++, std::index_sequence is a compile-time utility used to represent a sequence of non-negative integers of type std::size_t. 
    
    Defined in the <utility> header since C++14, it is a specialization of the more general std::integer_sequence.
    
    Its primary purpose is template metaprogramming, specifically for deducing and expanding parameter packs.Key Concepts and HelpersDefinition: It is an alias for std::integer_sequence<std::size_t, Ints...>.Creation Helpers:std::make_index_sequence<N>: Generates a sequence from 0 to N-1. For example, std::make_index_sequence<3> produces std::index_sequence<0, 1, 2>.

    ## Remove reference
    std::remove_reference_t<TUPLE> -> Removes references from the type
    say TUPLE = std::tuple<int, float>&& type then the above funciton removes the &&
    and simply returns std::tuple<int, float> type

For further notes on the design of the program refer to the code ...



## LESSON 4

We are going to create a new container and a corresponding contains_type....

    int main() {
        bool has_float = contains_type<float, std::tuple<float, int, bool>>::value;
    }

    We also used the fns associated with std::tuple

    std::tuple_element_t<start_from, TUPLE>