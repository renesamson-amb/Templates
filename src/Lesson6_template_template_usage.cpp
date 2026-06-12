#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <bitset>

/**
* Lesson 3
*/
#include <vector>
#include <iostream>
#include <assert.h>
#include <cmath>
#include <tuple>
#include <utility>
#include <list>

/**
* This function is just a helper....
*/
template<typename T>
struct has_type
{
    using type = T;
};


template<bool condition, typename THEN, typename ELSE>
struct if_
{};

template<typename THEN, typename ELSE>
struct if_<true, THEN, ELSE>: has_type<THEN>
{
    // using type = THEN;
};

template<typename THEN, typename ELSE>
struct if_<false, THEN, ELSE> : has_type<ELSE>
{
    // using type = ELSE;
};


/** 
* Runtime version
*/
bool contains(const std::string& search, std::list<std::string> l)
{
    if(l.empty())
    {
        return false;
    }
    else
    {
        if(l.front() == search)
        {
            return true;
        }
        else
        {
            l.pop_front();
            return contains(search, l);
        }
    }
}

/**
* Second part compile time opimisation, we are creating a
* variadic template based list type
*/
template <typename...>
struct type_list
{};

// Based on the runtime equivalen we are creating what true and
// false is...
template<typename LIST>
struct empty: std::false_type{};

template<>
struct empty<type_list<>>: std::true_type{};

static_assert(empty<type_list<>>::value );
static_assert(empty<type_list<int, bool>>::value == false);

// Now we need the front function
template<typename LIST>
struct front;

/* Lesson 6 addition .... Indicate that we are using a template*/
template<template<typename...> class LIST, typename t0, typename... t1toN>
struct front<LIST<t0, t1toN...>> : has_type<t0>
{
    // using type = t0;
};

// Partial specilization where the type_list has only one member...
template<template<typename> class LIST, typename LAST>
struct front<LIST<LAST>>
{
    using type = LAST;
};

static_assert(std::is_same_v<front<type_list<int, bool>>::type, int>);

// Next we need the pop front equivalent
template<typename LIST>
struct pop_front;

// Note: pop_front does not return the type at the front, but rather
// removes the type at the front and then returns the type_list with the
// first type removed...
template<typename t0, typename ... t1toN>
struct pop_front<type_list<t0, t1toN...>>:  has_type<type_list<t1toN...>>
{};

static_assert(std::is_same_v<pop_front<type_list<int, bool, float>>::type, type_list<bool, float>>);

// This is a convenience to avoid ::value or ::type
template<typename T>
using front_t = typename front<T>::type;

template<typename T>
using pop_front_t = typename pop_front<T>::type;



/**
////////// AT function ////////////////
@Returns a type that is at the given index
*/

template<typename LIST, size_t index>
struct at
{
    using type = typename at<pop_front_t<LIST>, index-1>::type;
};

template<typename LIST>
struct at<LIST, 0>
{
    using type = front_t<LIST>;
};


template<typename LIST, size_t index>
using at_t = typename at<LIST, index>::type;

static_assert(std::is_same_v<at_t<type_list<int, bool, float>, 1>, bool>);

/**
////////// AT function ////////////////
@Returns a type that is at the given index
This is an improved version of the at function
This function uses the has_type structure
*/

template<typename LIST, size_t index>
struct At : has_type<typename At<pop_front_t<LIST>, index-1>::type>
{};

template<typename LIST>
struct At<LIST, 0>: has_type<front_t<LIST>>
{
};

// TO convert something like true_type or false_Type  we use ::value
// which is like runtime, so we use static constexpr to get it in runtime
template<typename T>
static constexpr bool empty_v = empty<T>::value;

// Lets write the contains_type
/**
When looking for items in the list that is not in the list
an issue comes up: The front_t is does not have a partial
specilization when the type_list is empty. We don't have a none
type  to mark the list as empty. SO we can use one of the two methods
1) We could create a template fn that takes care of the non empty
set
2) We could write a contains_type specilization...

*/

// template<typename SEARCH , typename LIST>
// struct contains_type;

// template<typename SEARCH, typename LIST>
// struct non_empty_contains_type:
//     if_<// IF
//         std::is_same_v<SEARCH, front_t<LIST>>,
//         // THEN
//         std::true_type,
//         // ELSE
//         contains_type<SEARCH, pop_front_t<LIST>>
//     >::type
// {};


// template<typename SEARCH , typename LIST>
// struct contains_type:
//     if_<
//         empty_v<LIST>,
//         // THEN
//         std::false_type,
//         // ELSE
//         non_empty_contains_type<SEARCH, LIST>
//     >::type
// {};

// Now we use the second method, the template specilization
template<typename SEARCH, typename LIST>
struct contains_type;

// Non empty set
template<typename SEARCH, typename LIST>
struct contains_type :
    if_< // IF 
        std::is_same_v<SEARCH, front_t<LIST>>,
        // THEN
        std::true_type,
        // ELSE
        contains_type<SEARCH, pop_front_t<LIST>>
    >::type
{};

// Empty set
template<typename SEARCH>
struct contains_type<SEARCH, type_list<>> : std::false_type
{};


template<typename SEARCH, typename LIST>
static constexpr bool contains_type_v = contains_type<SEARCH, LIST>::value;

static_assert(contains_type_v<double, type_list<int, bool, float>> == false);


/**************************************
*******LESSON 5 additions**************
**************************************/
template<typename LIST>
struct back : has_type<typename back<pop_front_t<LIST>>::type>
{};

// Partial specilization: When we get a list
// with a single type then the type is returned...
template<typename T0>
struct back<type_list<T0>>: has_type<T0>
{};

template<typename LIST>
using back_t = typename back<LIST>::type;

static_assert(std::is_same_v<back_t<type_list<int, float>>, float>);

// ****************** PUSH BACK *****************
template<typename LIST, typename T>
struct push_back;

template<typename... T0toN, typename T>
struct push_back<type_list<T0toN...>, T>: has_type<type_list<T0toN..., T>>
{};

template<typename LIST, typename T>
using push_back_t = typename push_back<LIST, T>::type;

static_assert(std::is_same_v<push_back_t<type_list<>, int>, type_list<int>>);
static_assert(std::is_same_v<push_back_t<type_list<int, float, bool>, char>, type_list<int, float, bool, char>>);

///////////////////// POP BACK ///////////////////
// See how we are assigning the empty list type....
template<typename LIST, typename RET_LIST = type_list<>>
struct pop_back;

// The second parameter is a List type
template<typename T0, typename RET_LIST>
struct pop_back<type_list<T0>, RET_LIST>: has_type<RET_LIST>
{};

template<typename T0, typename T1, typename ... T2toN, typename RET_LIST>
struct pop_back<type_list<T0, T1, T2toN...>, RET_LIST>: 
    pop_back<type_list<T1, T2toN...>, push_back_t<RET_LIST, T0>>
{};

// Always think about the simplest case
template<typename T0>
struct pop_back<type_list<T0>> : has_type<type_list<>>
{};



template<typename LIST>
using pop_back_t = typename pop_back<LIST>::type;

static_assert(std::is_same_v<pop_back_t<type_list<int>>, type_list<>>);

static_assert(std::is_same_v<pop_back_t<type_list<int, bool, float>>, type_list<int, bool>>);
/* LESSON 6*/
// We can use template<template to create a input
// we are going to create a ANY function to check
// if any of the types in the template will match the predicate
// PREDICATE is the class to compare to LIST is our container
template<template<typename> class PREDICATE, typename LIST>
struct any;
// In real cases, our container will contain multiple types...
// Note in this case the PREDICATE is something like std::is_integral
// This returns true if the type being compared currently is int...
template<template<typename> class PREDICATE, template<typename...> class LIST>
struct any<PREDICATE, LIST<>>: std::false_type {};
// Now the implementation
template<template<typename> class PREDICATE, typename LIST>
struct any : if_< 
                PREDICATE<front_t<LIST>>::value,
                std::true_type,
                typename any<PREDICATE, pop_front_t<LIST>>::type
                >::type
{};

template<template<typename> class PREDICATE, typename LIST>
static constexpr bool any_v= any<PREDICATE, LIST>::value;

// Static asserts ....
static_assert(any_v<std::is_integral, type_list<int, double, std::string>>);
static_assert(any_v<std::is_integral, type_list< std::string, float, int>>);
// The below statement fails because there is no int in the container
// static_assert(any_v<std::is_integral, type_list<std::string, float, double>>);
static_assert(!any_v<std::is_integral, type_list<std::string, float, double>>);





/*Now we want to express our contains type using any
  We want something like shown below
  */
// template<typename SEARCH, typename LIST>
// static_constexpr bool contains_type_v = any<"is_same as SEARCH preicate", LIST>::value
// We could use the std::is_same -> but this requires two parameters we can't simply pass
// Only SEARCH....

// So we will create our own PREDICATE ...
template<typename T>
struct same_as_pred
{
    template<typename U>
    struct predicate: std::is_same<T, U> {};
};

template<typename SEARCH, typename LIST>
static constexpr bool contains_type_vv = any<same_as_pred<SEARCH>::template predicate, LIST>::value;

static_assert(contains_type_vv<int, type_list<int, bool, float>>);
// This is apowerful tool to write more complex meta functions and pass these metafunctions
// as inputs to other meta functions....
int main()
{
    // // First part the runtime equivalent ....
    // std::list<std::string> list{"int", "bool", "double"};
    // std::cout << std::boolalpha;
    // std::cout << contains("bool", list) << std::endl;
    // std::cout << contains("float", list) << std::endl;

    // // For the seccond part we will create a new type list
    // type_list<int, bool, float>  ts;
    // // std::cout << sizeof(ts) << std::endl;
    // std::cout << contains_type<bool, decltype(ts)>::value << "\n" ;
    // // This is were things start failing... Look for the type
    // // that is not in the list...
    // std::cout << contains_type<double, decltype(ts)>::value << "\n";


    ////// LESSON 6/////////////////////////
    // We can't use our library using standard templates such as std::tuple<>()
    std::tuple<int, float, bool> t;
    front_t<decltype(t)> first; // <- This won't work becoz we use type_list

    return 0;
}