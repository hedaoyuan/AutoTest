#include <iostream>
#include <utility>
#include <tuple>
#include <cassert>
#include "matrix.h"

namespace detail {

// construct a argument
template<typename T> T construct(int height, int width);
template<> float construct(int height, int width) { return 0.0; }
template<> CpuMatrix construct(int height, int width) {
  CpuMatrix a(height, width);
  return a;
}
template<> GpuMatrix construct(int height, int width) {
  GpuMatrix a(height, width);
  return a;
}

// init a argument
template<typename T> void init(T& v);
template<> void init(float& v) { v = 0.5; }
template<> void init(CpuMatrix& v) { v.rand(); }
template<> void init(GpuMatrix& v) { v.rand(); }

// init a tuple which contains a set of arguments.
template<std::size_t I = 0, typename... Args>
inline typename std::enable_if<I == sizeof...(Args), void>::type
initTuple(std::tuple<Args...>& t){}

template<std::size_t I = 0, typename... Args>
inline typename std::enable_if<I < sizeof...(Args), void>::type
initTuple(std::tuple<Args...>& t) {
  init(std::get<I>(t));
  initTuple<I + 1>(t);
}

// copy a argument, copy src to dest
template<typename T1, typename T2> void copy(T1& dest, T2& src);
template<> void copy(float& dest, float& src) { dest = src; }
template<> void copy(GpuMatrix& dest, CpuMatrix& src) {
  dest.data_ = src.data_;
}

// copy a tuple, copy src to dest
template<std::size_t I = 0, typename... Args1, typename... Args2>
inline typename std::enable_if<I == sizeof...(Args1), void>::type
copyTuple(std::tuple<Args1...>& dest, std::tuple<Args2...>& src) {}

template<std::size_t I = 0, typename... Args1, typename... Args2>
inline typename std::enable_if<I < sizeof...(Args1), void>::type
copyTuple(std::tuple<Args1...>& dest, std::tuple<Args2...>& src) {
  copy(std::get<I>(dest), std::get<I>(src));
  copyTuple<I + 1>(dest, src);
}

// Check
template<typename Matrix1, typename Matrix2>
void checkEqual(Matrix1 t1, Matrix2 t2);

template<>
void checkEqual(CpuMatrix t1, GpuMatrix t2) {
  assert(t1.height_ == t2.height_);
  assert(t1.width_ == t2.width_);

  assert(t1.data_ == t2.data_);

  std::cout << "RUN PASSED "
            << "[height = " << t1.height_
            << " width = " << t1.width_
            << "]"
            << std::endl;
}

// call member function
template <typename C, typename R, typename ...FArgs, typename ...Args>
R call(C& obj, R (C::*f)(FArgs...), Args&&... args) {
  return (obj.*f)(args...);
}

template <std::size_t... I,
          typename C1, typename R1, typename ...Args1,
          typename C2, typename R2, typename ...Args2>
void autoCompare(R1 (C1::*f1)(Args1...), R2 (C2::*f2)(Args2...)) {
  for (auto height : {1, 5}) {
    for (auto width : {1, 5}) {
      C1 obj1(height, width);
      C2 obj2(height, width);

      auto tuple1 = std::make_tuple(
        construct<typename std::decay<
          typename std::tuple_element<I, std::tuple<Args1...>>::type>::type>(
          height, width)...);

      auto tuple2 = std::make_tuple(
        construct<typename std::decay<
          typename std::tuple_element<I, std::tuple<Args2...>>::type>::type>(
          height, width)...);

      initTuple(tuple1);
      copyTuple(tuple2, tuple1);

      call(obj1, f1, std::get<I>(tuple1)...);
      call(obj2, f2, std::get<I>(tuple2)...);

      checkEqual(obj1, obj2);
    }
  }
}

}

#if (__cplusplus >= 201402L)

template <std::size_t... I,
          typename C1, typename R1, typename ...Args1,
          typename C2, typename R2, typename ...Args2>
void autoCompare(R1 (C1::*f1)(Args1...), R2 (C2::*f2)(Args2...),
                 std::index_sequence<I...>) {
  detail::autoCompare<I...>(f1, f2);
}

template <typename C1, typename R1, typename ...Args1,
          typename C2, typename R2, typename ...Args2>
void autoCompare(R1 (C1::*f1)(Args1...), R2 (C2::*f2)(Args2...)) {
  static_assert(sizeof...(Args1) == sizeof...(Args2),
    "size of parameter packs are not equal");

  autoCompare(f1, f2, std::make_index_sequence<sizeof...(Args1)>());
}
#else

template <std::size_t... I,
         typename C1, typename R1, typename ...Args1,
         typename C2, typename R2, typename ...Args2>
void autoCompare(R1 (C1::*f1)(Args1...), R2 (C2::*f2)(Args2...)) {
  static_assert(sizeof...(I) == sizeof...(Args1),
    "size of parameter packs are not equal");
  static_assert(sizeof...(I) == sizeof...(Args2),
    "size of parameter packs are not equal");

  detail::autoCompare<I...>(f1, f2);
}

#endif

int main() {
#if (__cplusplus >= 201402L)
  // c++14
  autoCompare(&CpuMatrix::add, &GpuMatrix::add);
  autoCompare(&CpuMatrix::assign, &GpuMatrix::assign);
  autoCompare(&CpuMatrix::neg, &GpuMatrix::neg);
#else
  // c++11
  autoCompare<0, 1>(&CpuMatrix::add, &GpuMatrix::add);
  autoCompare<0>(&CpuMatrix::assign, &GpuMatrix::assign);
  autoCompare(&CpuMatrix::neg, &GpuMatrix::neg);
#endif

  return 0;
}

