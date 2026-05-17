#include <vector>
#include <iostream>
#include <assert.h>
#include <cmath>
// We will use recursive approach ....
bool contains(const std::string& search, const std::vector<std::string>& v, size_t start=0)
{
    if(v[start] == search)
        return true;
    else if(start == v.size()-1)
        return false;
    else
        return contains(search, v, start+1);
}

int main()
{
    std::vector<std::string> vec{"int", "float", "bool"};
    std::cout << std::boolalpha << contains("bool", vec) << std::endl;
    std::cout << std::boolalpha << contains("char", vec) << std::endl;

    // std::tuple<int, float> t;
    // std::cout << std::boolalpha << contains_type<bool, t>::value << std::endl;
}