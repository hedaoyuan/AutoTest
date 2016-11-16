#include <iostream>
#include <utility>
#include <tuple>
#include "matrix.h"

// init function
template<typename T> T init(T v);
template<> float init(float v) { v = 0.5; return v; }
template<> CpuMatrix init(CpuMatrix v) { v.data_ = 0.2; return v; }
template<> GpuMatrix init(GpuMatrix v) { v.data_ = 0.2; return v; }

// call any member function
template <typename C, typename R,  typename ...Args>
R call(C& obj, R (C::*f)(Args...), Args&&... args) {
  return (obj.*f)(args...);
}

// automatically initialize the parameter
template <typename C, typename R,  typename ...Args, std::size_t... I>
void callWithInit(C& obj, R (C::*f)(Args...), std::index_sequence<I...>) {
  call(obj, f,
    init(typename std::tuple_element<I, std::tuple<Args...>>::type())...);
}

template <typename C1, typename R1, typename ...Args1,
          typename C2, typename R2, typename ...Args2>
void autoCompare(R1 (C1::*f1)(Args1...), R2 (C2::*f2)(Args2...)) {
  C1 obj1;
  C2 obj2;

  callWithInit(obj1, f1, std::make_index_sequence<sizeof...(Args1)>());
  callWithInit(obj2, f2, std::make_index_sequence<sizeof...(Args2)>());

  std::cout << obj1.data_ << std::endl;
  std::cout << obj2.data_ << std::endl;
}

int main() {
  autoCompare(&CpuMatrix::add, &GpuMatrix::add);
  return 0;
}

