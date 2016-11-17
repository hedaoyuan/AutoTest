# AutoCompare

Compare member function of CpuMatrix and GpuMatrix with a single line of code.
In autoCompare, it will automatically generate the parameters required to call the member function.
And synchronizes the initial values of the parameters of the two member functions.
Finally, verify that the results of the two member functions are the same.

`g++ -O3 -std=c++14 auto_compare.cpp`
```
  autoCompare(&CpuMatrix::add, &GpuMatrix::add);
```

