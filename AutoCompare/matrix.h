class Matrix {
public:
  float data_;
};

class CpuMatrix : public Matrix {
public:
  void add(CpuMatrix B, float c) { data_ = B.data_ + c; }
};

class GpuMatrix : public Matrix {
public:
  void add(GpuMatrix B, float c) { data_ = B.data_ + c; }
};
