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


template<bool condition, typename THEN, typename ELSE>
struct if_
{};

template<typename THEN, typename ELSE>
struct if_<true, THEN, ELSE>
{
    using type = THEN;
};

template<typename THEN, typename ELSE>
struct if_<false, THEN, ELSE>
{
    using type = ELSE;
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
struct front<type_list<t0, t1toN...>>
{
    using type = t0;
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
struct pop_front<type_list<t0, t1toN...>>
{
    using type = type_list<t1toN...>;
};

static_assert(std::is_same_v<pop_front<type_list<int, bool, float>>::type, type_list<bool, float>>);

// This is a convenience to avoid ::value or ::type
template<typename T>
using front_t = typename front<T>::type;

template<typename T>
using pop_front_t = typename pop_front<T>::type;

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



int main()
{
    // First part the runtime equivalent ....
    std::list<std::string> list{"int", "bool", "double"};
    std::cout << std::boolalpha;
    std::cout << contains("bool", list) << std::endl;
    std::cout << contains("float", list) << std::endl;

    // For the seccond part we will create a new type list
    type_list<int, bool, float>  ts;
    std::cout << contains_type<bool, decltype(ts)>::value ;
    // This is were things start failing... Look for the type
    // that is not in the list...
    std::cout << contains_type<double, decltype(ts)>::value << "\n";

    return 0;
}