# AutoCompare

Compare member function of CpuMatrix and GpuMatrix with a single line of code.
`g++ -O3 -std=c++14 auto_compare.cpp`
```
  autoCompare(&CpuMatrix::add, &GpuMatrix::add);
```

