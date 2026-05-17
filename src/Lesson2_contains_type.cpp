#include <vector>
#include <iostream>
#include <assert.h>
#include <cmath>
#include <tuple>

// We also need IF template
template<bool condition, typename THEN, typename ELSE>
struct if_;

// Partial specilization part 1
template<typename THEN, typename ELSE>
struct if_<true, THEN, ELSE>
{
    using type = THEN;
};

// Partial specilization part 2
template<typename THEN, typename ELSE>
struct if_<false, THEN, ELSE>
{
    using type = ELSE;
};

// Contains type...
template<typename SEARCH, typename TUPLE, size_t start_from=0>
struct contains_type : 
        if_<std::is_same<std::tuple_element_t<start_from, TUPLE>, SEARCH>::value,
            // THEN
            std::true_type,
            // ELSE
            typename if_<
            start_from == std::tuple_size<TUPLE>::value - 1,
            // THEN
            std::false_type,
            // ELSE
            contains_type<SEARCH, TUPLE, start_from+1>
            >::type
        >::type
{
};

template<typename SEARCH>
struct contains_type<SEARCH, std::tuple<>, 0> : std::false_type
{};

// We will use recursive approach ....
// bool contains(const std::string& search, const std::vector<std::string>& v, size_t start=0)
// {
//     if(v[start] == search)
//         return true;
//     else if(start == v.size()-1)
//         return false;
//     else
//         return contains(search, v, start+1);
// }

int main()
{
    std::vector<std::string> vec{"int", "float", "bool"};
    // std::cout << std::boolalpha << contains("bool", vec) << std::endl;
    // std::cout << std::boolalpha << contains("char", vec) << std::endl;

    std::tuple<int, float> t;
    // Test for the if case created...
    std::cout << std::boolalpha << std::is_same_v<int, if_<(5<10), int, float>::type> << std::endl;;
    std::cout << std::boolalpha << contains_type<bool, decltype(t)>::value << std::endl;

    // What about the empty type
    std::cout << "Not Empty ? " << std::boolalpha << contains_type<bool, std::tuple<>>::value << std::endl; 
}