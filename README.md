# Utility

## AutoCompare
```
  class CpuMatrix {
    void add(const CpuMatrix& B, float c);
  };
  class GpuMatrix {
    void add(const GpuMatrix& B, float c);
  };
```
Compare member function of CpuMatrix and GpuMatrix with a single line of code.
```
  autoCompare(&CpuMatrix::add, &GpuMatrix::add);
```

