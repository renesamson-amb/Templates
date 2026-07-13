## Lesson 6

The problem we have with the lib we've created is that all the funcs
push_back, pop_back and other funcs only work with 

type_list<>

We can't use it with std::tuple<> for example....
We can make our function make work with anytype of template using 

<template<typename...> class LIST>

This represents a template with a list of types. class because everything in CPP is a class

We should do that same for all the functions, follow it up with a cleanup....



We are going to write a ANY function that returns true if the template has
a given predicate in it

any_v<std::is_integral, type_list<int, double, float>>

