#include <iostream>
#include <type_traits>

template<typename T>
struct is_pointer
{
    static constexpr bool value = false;
};

template<typename T>
struct is_pointer<T*>
{
    static constexpr bool value = true;
};

// Method to strip the pointer of the type being passed
template<typename T>
struct without_pointer
{
    using type = T;
};

template<typename T>
struct without_pointer<T*>
{
    using type = T;
};

template <typename T>
void Print1(T t1)
{
    // Get the type and store it in the below var...
    using T_without_Pointer_type = without_pointer<T>::type;
    // A special requirement: Check if the type is a pointer type and 
    // also specifically a floating pointer type...
    if constexpr(is_pointer<T>::value && std::is_floating_point<T_without_Pointer_type>::value)
    {
        std::cout << *t1 ;
    }
    else
    {
        std::cout << t1;
    }
}

template<typename A, typename B, typename C>
void Print(A a, B b, C c)
{
    Print1<A>(a);
    std::cout << " , " ;
    Print1<B>(b);
    std::cout << " , " ;
    Print1<C>(c);
    std::cout << std::endl;
}

/**
* Initial requirement: Create a print func that can print any type of variable
* Including pointers
*
* Second stage requirements: Dereference pointer types if and only if the pointer
* type is float
*/

int main()
{
    float b = 10.0f;
    int a = 20;
    std::string c = "Hello";
    Print(&b, a , c);

    float *x;
    x = &b;
    Print(x, a, c);
    return 0;
}