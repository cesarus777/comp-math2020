#pragma once

#include <matrix.hpp>

#include <iterator>
#include <valarray>

namespace comp
{

template<typename T, size_t rows, size_t cols> class matrix;

template<typename T, size_t dim>
class vector : public matrix<T, dim, 1>
{
protected:
  using matrix<T, dim, 1>::data;

public:
  using matrix<T, dim, 1>::get_raw_data;
  using matrix<T, dim, 1>::operator*;
  using matrix<T, dim, 1>::operator/;

  vector(const T& val = 0) : matrix<T, dim, 1>(val)
  {
    if(data.size() != dim)
      throw std::exception();
  }

  vector(const T* vals) : matrix<T, dim, 1>(vals)
  {
    if(data.size() != dim)
      throw std::exception();
  }

  template<typename U>
  vector(const std::valarray<U>& other_data) : matrix<T, dim, 1>{other_data}
  {
    if(data.size() != dim)
      throw std::exception();
  }

  template<typename U>
  vector<T, dim>& operator=(const std::valarray<U>& other_data)
  {
    if(other_data.size() != dim)
      throw std::exception();

    data = other_data;
    return *this;
  }

  template<typename U>
  vector(std::valarray<U>&& other_data) : matrix<T, dim, 1>{std::move(other_data)}
  {
    if(data.size() != dim)
      throw std::exception();
  }

  template<typename U>
  vector<T, dim>& operator=(const std::valarray<U>&& other_data)
  {
    if(other_data.size() != dim)
      throw std::exception();

    data = std::move(other_data);
    return *this;
  }

  template<typename U>
  vector(const vector<U, dim>& other) : matrix<T, dim, 1>(other.data)
  {
    if(data.size() != dim)
      throw std::exception();
  }

  template<typename U>
  vector<T, dim>& operator=(const vector<U, dim>& other)
  {
    data = other.data;
    return *this;
  }

  template<typename U, size_t other_dim>
  vector(const vector<U, other_dim>& other, size_t start = 0u)
      : matrix<T, dim, 1>(static_cast<T>(0))
  {
    if constexpr(other_dim > dim)
      throw std::exception();

    data[std::slice(start, other_dim, 1)] = other.matrix<T, other_dim, 1>::get_raw_data();
  }

  template<typename U, size_t other_dim>
  vector<T, dim>& operator=(const vector<U, other_dim>& other)
  {
    if constexpr(other_dim > dim)
      throw std::exception();

    *this = std::move(vector<T, dim>{other});
    return *this;
  }

  template<typename U>
  vector(vector<U, dim>&& other) : matrix<T, dim, 1>{std::move(other.data)}
  {
    if(data.size() != dim)
      throw std::exception();
  }

  template<typename U>
  vector<T, dim>& operator=(const vector<U, dim>&& other)
  {
    data = std::move(other.data);
    return *this;
  }

  vector(std::initializer_list<T> il) : matrix<T, dim, 1>{il}
  {
    if(data.size() != dim)
      throw std::exception();
  }

  vector<T, dim>& operator=(std::initializer_list<T> il)
  {
    if(il.size() != dim)
      throw std::exception();

    data = il;
    return *this;
  }

  auto begin() { return std::begin(data); }
  auto end()   { return std::end(data); }

  const auto cbegin() const { return std::cbegin(data); }
  const auto cend()   const { return std::cend(data); }

  T& operator[](size_t i) { return data[i]; }
  std::slice_array<T> operator[](std::slice slice) { return data[slice]; }

  const T& operator[](size_t i) const { return data[i]; }
  std::valarray<T> operator[](std::slice slice) const { return data[slice]; }

  auto operator*=(const T val)
  {
    data *= val;
    return *this;
  }

  auto operator/=(const T val)
  {
    data *= val;
    return *this;
  }

  auto operator+(const vector<T, dim>& other)
  {
    std::valarray<T> res_data = data + other.get_raw_data();
    return std::move(vector<T,dim>(std::move(res_data)));
  }

  auto operator-(const vector<T, dim>& other)
  {
    std::valarray<T> res_data = data - other.get_raw_data();
    return std::move(vector<T,dim>(std::move(res_data)));
  }

  T operator*(const vector<T, dim>& other)
  {
    T res = 0;
    for(size_t i = 0; i < dim; ++i)
      res += data[i] * other[i];

    return res;
  }

  vector<T, dim>& shift(const size_t count)
  {
    data = data.shift();
    return *this;
  }

  vector<T, dim>& cshift(const size_t count)
  {
    data = data.cshift();
    return *this;
  }

  static auto get_random(unsigned seed = 0u)
  {
    vector<T, dim> v{};
    for(size_t i = 0; i < dim; ++i)
      v[i] = static_cast<T>(i + seed);

    return std::move(v);
  }

  T norm1()
  {
    return data.max();
  }

  T norm2()
  { 
    return std::abs(data).sum();
  }

  T norm3()
  {
    T res = 0;
    for(const auto& elem : data)
      res += std::abs(elem) * std::abs(elem);

    return std::sqrt(res);
  }
}; //class vector

}; // namespace comp

