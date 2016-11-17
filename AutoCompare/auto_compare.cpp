#include <iostream>
#include <utility>
#include <tuple>
#include "matrix.h"

// init a element
template<typename T> void init(T& v);
template<> void init(float& v) { v = 0.5; }
template<> void init(CpuMatrix& v) { v.rand(); }
template<> void init(GpuMatrix& v) { v.rand(); }

// init a tuple
template<std::size_t I = 0, typename... Args>
inline typename std::enable_if<I == sizeof...(Args), void>::type
initTuple(std::tuple<Args...>& t){}

template<std::size_t I = 0, typename... Args>
inline typename std::enable_if<I < sizeof...(Args), void>::type
initTuple(std::tuple<Args...>& t) {
  init(std::get<I>(t));
  initTuple<I + 1>(t);
}

// copy a element, copy src to dest
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

// call member function
template <typename C, typename R, typename ...FArgs, typename ...Args>
R call(C& obj, R (C::*f)(FArgs...), Args&&... args) {
  return (obj.*f)(args...);
}

template <std::size_t... I,
          typename C1, typename R1, typename ...Args1,
          typename C2, typename R2, typename ...Args2>
void callWithInit(C1& obj1, R1 (C1::*f1)(Args1...),
                  C2& obj2, R2 (C2::*f2)(Args2...)) {
  auto tuple1 = std::make_tuple(
    typename std::remove_reference<
      typename std::tuple_element<I, std::tuple<Args1...>>::type>::type()...);
  auto tuple2 = std::make_tuple(
    typename std::remove_reference<
      typename std::tuple_element<I, std::tuple<Args2...>>::type>::type()...);

  initTuple(tuple1);
  copyTuple(tuple2, tuple1);

  call(obj1, f1, std::get<I>(tuple1)...);
  call(obj2, f2, std::get<I>(tuple2)...);
}

#if (__cplusplus >= 201402L)

template <typename C1, typename R1, typename ...Args1,
          typename C2, typename R2, typename ...Args2,
          std::size_t... I>
void callWithCPP14(C1& obj1, R1 (C1::*f1)(Args1...),
                   C2& obj2, R2 (C2::*f2)(Args2...),
                   std::index_sequence<I...>) {
  callWithInit<I...>(obj1, f1, obj2, f2);
}

template <typename C1, typename R1, typename ...Args1,
          typename C2, typename R2, typename ...Args2>
void autoCompare(R1 (C1::*f1)(Args1...), R2 (C2::*f2)(Args2...)) {
  static_assert(sizeof...(Args1) == sizeof...(Args2),
    "size of parameter packs are not equal");

  C1 obj1;
  C2 obj2;
  callWithCPP14(obj1, f1, obj2, f2, std::make_index_sequence<sizeof...(Args1)>());

  // check
  std::cout << obj1.data_ << std::endl;
  std::cout << obj2.data_ << std::endl;
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

  C1 obj1;
  C2 obj2;
  callWithInit<I...>(obj1, f1, obj2, f2);

  // check
  std::cout << obj1.data_ << std::endl;
  std::cout << obj2.data_ << std::endl;
}

#endif

int main() {
#if (__cplusplus >= 201402L)
  // c++14
  autoCompare(&CpuMatrix::add, &GpuMatrix::add);
#else
  // c++11
  autoCompare<0, 1>(&CpuMatrix::add, &GpuMatrix::add);
#endif

  return 0;
}

