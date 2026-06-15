In this lesson lets consider std::tuple 
![alt text](image.png)


We also need to figure out how to map the values to the corresponding types in tuple. For that we need to figure out steps at compile time and ensure __no unnecessary copies/moves__ at runtime
+ lvalue + rvalue
+ perfect forwarding

__WHAT IS TUPLE__
+ Recursive inheritance
    - The tuple is only responsible for the first element
    
    Example
        
        Tuple<int, bool, char > { char data; }
            🠋 (inherit from)
        Tuple<bool, char> { char data; }
            🠋 (inherits from)
        Tuple<char> { char data; }

+ Multiple Inheritance
    - The tuple inherits from each of the types. This boils down to a static_cast
    
    Example

        Tuple<int, bool, float> tuple;
        To get float static_cast<float>(tuple) = true;
    
    But this approach has a problem. If there are two items in the tuple of the same type. Say tuple<int, bool, bool> t

    The common approach is
    template<size_t index, typename T>
    struct TupleData {
        T data;
    }

        TupleData<0, int> TupleData< 1, bool> TupleData<2, bool>
                🠋           🠋                   🠋
        Tuple< int,         bool,               bool>

        In this case we can get the second element by casting the tuple to the correct base class

        
        Here we aim for simplicity and speed....
        recursive allows for simpler algorithms -> so this is what we will start with.....
	But we will revisit the multiple inheritance as well...
