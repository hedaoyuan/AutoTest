#include <vector>
#include <cstdlib>

class Matrix {
public:
  Matrix(int height, int width) : height_(height), width_(width) {}
  float data_;
  int height_;
  int width_;
};

class CpuMatrix : public Matrix {
public:
  CpuMatrix(int height, int width) : Matrix(height, width) {}
  void rand() { data_ = 1.0f / (std::rand() % 10);}
  void add(const CpuMatrix& B, float c) { data_ = B.data_ + c; }
  void assign(const CpuMatrix& B) { data_ = B.data_; }
  void neg() { data_ = - data_; };
};

class GpuMatrix : public Matrix {
public:
  GpuMatrix(int height, int width) : Matrix(height, width) {}
  void rand() { data_ = 1.0f / (std::rand() % 10);}
  void add(const GpuMatrix& B, float c) { data_ = B.data_ + c; }
  void assign(const GpuMatrix& B) { data_ = B.data_; }
  void neg() { data_ = - data_; };
};

