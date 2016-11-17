# AutoCompare

Compare member function of CpuMatrix and GpuMatrix with a single line of code.
In autoCompare, it will automatically generate the parameters required to call the member function.
And synchronizes the initial values of the parameters of the two member functions.
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

