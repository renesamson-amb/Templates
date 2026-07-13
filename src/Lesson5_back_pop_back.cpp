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

template<typename t0, typename... t1toN>
struct front<type_list<t0, t1toN...>> : has_type<t0>
{
    // using type = t0;
};

// Partial specilization where the type_list has only one member...
template<typename LAST>
struct front<type_list<LAST>>
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

// template<typename 

int main()
{
    // First part the runtime equivalent ....
    std::list<std::string> list{"int", "bool", "double"};
    std::cout << std::boolalpha;
    std::cout << contains("bool", list) << std::endl;
    std::cout << contains("float", list) << std::endl;

    // For the seccond part we will create a new type list
    type_list<int, bool, float>  ts;
    // std::cout << sizeof(ts) << std::endl;
    std::cout << contains_type<bool, decltype(ts)>::value << "\n" ;
    // This is were things start failing... Look for the type
    // that is not in the list...
    std::cout << contains_type<double, decltype(ts)>::value << "\n";

    return 0;
}