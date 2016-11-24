# AutoCompare

Compare member function of CpuMatrix and GpuMatrix with a single line of code.
In autoCompare, it will automatically generate the arguments required to call the member function.
And synchronizes the initial values of the arguments of the two member functions.
Finally, verify that the results of the two member functions are the same.

```
#if (__cplusplus >= 201402L)
  // if c++14
  autoCompare(&CpuMatrix::add, &GpuMatrix::add);
#else
  // if c++11
  autoCompare<0, 1>(&CpuMatrix::add, &GpuMatrix::add);
#endif
```

## Code logic
The main code logic is in the detail::autoCompare function.
1. Constructs a set of arguments based on the argument types of member function passed in, and stored in a `std::tuple`.
2. Initializes all the elements in the tuple, need to wrap an init function for all possible types.
3. Constructs a generic function template to call back the incoming member function.



