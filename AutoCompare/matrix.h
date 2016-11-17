#include <cstdlib>

class Matrix {
public:
  float data_;
};

class CpuMatrix : public Matrix {
public:
  void rand() { data_ = 1.0f / (std::rand() % 10);}
  void add(CpuMatrix B, float c) { data_ = B.data_ + c; }
};

class GpuMatrix : public Matrix {
public:
  void rand() { data_ = 1.0f / (std::rand() % 10);}
  void add(GpuMatrix B, float c) { data_ = B.data_ + c; }
};

