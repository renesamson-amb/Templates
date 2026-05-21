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


## LESSON 5

# Recap

So far we have covered the following

using my_list = type_list<int, bool, float>;

empty_v<my_list>                =>  false
front_t<my_list>                =>  int
pop_front_t<my_list>            =>  type_list<bool, float>

contains_type_v<bool, my_list>  => true

In this lesson we are going to extend the functionalities

We'll start by writing the "at" function
This function should take the LIST typename and then
a size_t for index. The logic is keep poping the front item until the
index becomes zero. In that case, use the front_t to return the front
item...

In the course of this lesson we introduce the struct has_type

    template<typename T>
    struct has_type
    {
        using type = T;
    };


so we can prune the other structs to inherit from this struct...
for ex, struct push_back becomes this...

    template<typename T0, typename... T1toN>
    struct push_back : has_type<type_list<T1toN...>>
    {};

In this lesson we talk __back__ and __pop_back__

Then pop_back is an interesting function. To achieve this we rebuild the
list from scratch with the first type and then pass the rest to the
recursive function...

HOME WORK: CREATE type_list<> and then just build the back and pop_back
as exercise...

