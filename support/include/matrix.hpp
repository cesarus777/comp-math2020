#pragma once

#include <vector.hpp>

#include <algorithm>
#include <complex>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <valarray>



namespace comp
{

template<typename T, size_t dim>
class square_matrix;

template<typename T, size_t rows = 1, size_t cols = 1>
class matrix
{
protected:
  std::valarray<T> data;

public:
  matrix(const T& val = 0) : data(val, rows * cols)
  {
    if constexpr(rows == 0 || cols == 0)
      throw std::exception();

    if(data.size() != rows * cols)
      throw std::exception{};
  }

  matrix(const std::valarray<T>& other_data) : data(other_data)
  {
    if constexpr(rows == 0 || cols == 0)
      throw std::exception{};

    if(data.size() != rows * cols)
      throw std::exception{};
  }

  matrix(std::valarray<T>&& other_data) : data(std::move(other_data))
  {
    if constexpr (rows == 0 || cols == 0)
      throw std::exception{};

    if(data.size() != rows * cols)
      throw std::exception{};
  }

  matrix(const matrix& other) : data(other.data) {}

  matrix& operator=(const matrix& other)
  {
    data = other.data;
    if((data.size() != rows * cols) || (data.size() != other.data.size()) || (data != other.data).min())
      throw std::exception{};

    return *this;
  }

  matrix(matrix&& other) : data(std::move(other.data)) {}

  matrix& operator=(matrix&& other)
  {
    data = std::move(other.data);
    if(data.size() != rows * cols)
      throw std::exception{};

    return *this;
  }

  auto row(size_t i)
  {
    return data[ std::slice(i * cols, cols, 1) ];
  }

  auto col(size_t j)
  {
    return data[ std::slice(j, rows, cols) ];
  }

  auto crow(size_t i) const
  {
    return data[ std::slice(i * cols, cols, 1) ];
  }

  auto ccol(size_t j) const
  {
    return data[ std::slice(j, rows, cols) ];
  }

  auto get_row(size_t i) const
  {
    std::valarray<T> r = data[ std::slice(i * cols, cols, 1) ];
    return std::move(matrix<T, 1, cols>{r});
  }

  auto get_col(size_t j) const
  {
    std::valarray<T> c = data[ std::slice(j, rows, cols) ];
    return std::move(vector<T, rows>{c});
  }

  const std::valarray<T>& get_raw_data() const
  {
    return data;
  }

  T& operator()(size_t r, size_t c)
  {
    return data[r * cols + c];
  }

  const T& operator()(size_t r, size_t c) const
  {
    return data[r * cols + c];
  }

  T sum() const { return data.sum(); }

  auto begin() { return std::begin(data); }
  auto end()   { return std::end(data); }
  const auto begin() const { return std::begin(data); }
  const auto end()   const { return std::end(data); }

  auto operator[](std::slice slice)
  {
    return data[slice];
  }

  const auto operator[](std::slice slice) const
  {
    return data[slice];
  }

  auto operator+(const matrix<T, rows, cols>& other) const
  {
    if constexpr(rows == cols)
    {
      square_matrix<T, rows> res(data + other.data);
      return std::move(res);
    }
    else
    {
      matrix<T, rows, cols> res(data + other.data);
      return std::move(res);
    }
  }

  auto operator-() const
  {
    if constexpr(rows == cols)
    {
      square_matrix<T, rows> res(-data);
      return std::move(res);
    }
    else
    {
      matrix<T, rows, cols> res(-data);
      return std::move(res);
    }
  }

  auto operator-(const matrix<T, rows, cols>& other) const
  {
    if constexpr(rows == cols)
    {
      square_matrix<T, rows> res(data - other.data);
      return std::move(res);
    }
    else
    {
      matrix<T, rows, cols> res(data - other.data);
      return std::move(res);
    }
  }

  auto operator*(const T& koeff) const
  {
    return std::move(matrix<T, rows, cols>{data * koeff});
  }

  auto operator/(const T& koeff) const
  {
    return std::move(matrix<T, rows, cols>{data / koeff});
  }

  auto operator*(const vector<T, rows>& v) const
  {
    vector<T, rows> res{};

    for(size_t i = 0; i < rows; ++i)
    {
      res[i] = (crow(i) * v.get_raw_data()).sum();
    }

    return std::move(res);
  }

  template<size_t other_rows, size_t other_cols>
  auto operator*(const matrix<T, other_rows, other_cols>& other) const
  {
    if constexpr(cols != other_rows)
      throw std::exception();

    matrix<T, rows, other_cols> res{};
    for(size_t i = 0; i < rows; ++i)
    {
      for(size_t k = 0; k < cols; ++k)
      {
        for(size_t j = 0; j < other_cols; ++j)
        {
          res(i, j) += (*this)(i, k) * other(k, j);
        }
      }
    }

    if constexpr(rows == other_cols)
      return std::move(square_matrix<T, rows>{std::move(res.data)});
    else
      return std::move(res);
  }

  auto transposed()
  {
    matrix<T, cols, rows> transposed_mtx{};

    for(size_t i = 0; i < rows; ++i)
      for(size_t j = 0; j < cols; ++j)
        transposed_mtx(j, i) = (*this)(i, j);

    return std::move(transposed_mtx);
  }

  auto conj() const
  {
    matrix<T, rows, cols> conj_mtx(data);
    if constexpr(std::is_same_v<T, std::complex<float>>  ||
                 std::is_same_v<T, std::complex<double>> ||
                 std::is_same_v<T, std::complex<long double>>)
      conj_mtx.data.apply(std::conj);

    return std::move(conj_mtx.transposed());
  }
}; //class matrix



template<typename T, size_t rows, size_t cols>
std::ostream& operator<<(std::ostream& os, const matrix<T, rows, cols>& mtx)
{
  size_t column = 0;
  for(const auto& elem : mtx)
  {
    os << std::setw(4) << std::internal;
    if(column)
      os << " ";
    else
      os << "|";

    os << elem;

    if(++column % cols == 0)
    {
      column = 0;
      os << "|" << std::endl;
    }
  }
  return os;
}



template<typename T, size_t rows, size_t cols>
matrix<T, rows, cols> operator*(T val, const matrix<T, rows, cols>& mtx)
{
  return std::move(mtx * val);
}



template<typename T, size_t dim = 1>
class square_matrix : public matrix<T, dim, dim>
{
protected:
  using matrix<T, dim, dim>::data;

public:
  using matrix<T, dim, dim>::row;
  using matrix<T, dim, dim>::col;
  using matrix<T, dim, dim>::crow;
  using matrix<T, dim, dim>::ccol;
  using matrix<T, dim, dim>::begin;
  using matrix<T, dim, dim>::end;
  //using matrix<T, dim, dim>::cbegin;
  //using matrix<T, dim, dim>::cend;
  using matrix<T, dim, dim>::operator[];
  using matrix<T, dim, dim>::operator();
  using matrix<T, dim, dim>::operator+;
  using matrix<T, dim, dim>::operator*;
  using matrix<T, dim, dim>::operator/;
  using matrix<T, dim, dim>::transposed;
  using matrix<T, dim, dim>::conj;

  square_matrix(const T& val = 0) : matrix<T, dim, dim>(val)
  {
    if constexpr(dim == 0)
      throw std::exception();

    if(data.size() != dim * dim)
      throw std::exception{};
  }

  square_matrix(const std::valarray<T>& other_data)
      : matrix<T, dim, dim>(other_data)
  {
    if constexpr(dim == 0)
      throw std::exception();

    if(data.size() != dim * dim)
      throw std::exception{};
  }

  square_matrix(std::valarray<T>&& other_data)
      : matrix<T, dim, dim>(std::move(other_data))
  {
    if constexpr(dim == 0)
      throw std::exception();

    if(data.size() != dim * dim)
      throw std::exception{};
  }

  square_matrix(const square_matrix& other) : matrix<T, dim, dim>(other.data)
  {
    if constexpr(dim == 0)
      throw std::exception();

    if(data.size() != dim * dim)
      throw std::exception{};
  }

  square_matrix(square_matrix&& other)
      : matrix<T, dim, dim>(std::move(other.data))
  {
    if constexpr(dim == 0)
      throw std::exception();

    if(data.size() != dim * dim)
      throw std::exception{};
  }

  static auto identity_matrix()
  {
    square_matrix<T, dim> E(static_cast<T>(0));
    E[std::slice(0, dim, dim + 1)] = static_cast<T>(1);
    return std::move(E);
  }

  auto minor(size_t i, size_t j) const
  {
    if(dim < 2)
      throw std::exception{};

    std::valarray<bool> mask(true, dim * dim);
    size_t index = 0;
    std::for_each(std::begin(mask), std::end(mask),
                  [i, j, &index](bool& elem)
                  {
                    if(((index / dim) == i) || ((index % dim) == j))
                      elem = false;

                    ++index;
                  });
    return std::move(square_matrix<T, dim - 1>{data[mask]});
  }

  T det() const
  {
    T res = 0;

    if constexpr (dim == 1)
    {
      res = data[0];
    }
    else if(dim == 2)
    {
      res = data[0] * data[3] - data[1] * data[2];
    }
    else
    {
      square_matrix<T, dim> mtx4det(data);
      res = mtx4det(0, 0);
      for(size_t i = 1; i < dim; ++i)
      {
        for(size_t j = i; j < dim; ++j)
        {
          T koeff = -mtx4det(j, i - 1) / mtx4det(i - 1, i - 1);
          mtx4det.row(j) += koeff * mtx4det.crow(i - 1);
        }

        res *= mtx4det(i, i);
      }

      // std::cout << "\nTriag mtx:\n" << mtx4det << std::endl;
    }

      return res;
  }

  auto inversed() const
  {
#ifdef BAD_INVERSED
    square_matrix<T, dim> inversed_mtx{};

    for(size_t i = 0; i < dim; ++i)
    {
      for(size_t j = 0; j < dim; ++j)
      {
        if((i + j) % 2 == 0)
          inversed_mtx(i, j) = minor(j, i).det();
        else
          inversed_mtx(i, j) = -minor(j, i).det();
      }
    }

    inversed_mtx.data /= det();

    return std::move(inversed_mtx);
#else
    square_matrix<T, dim> E_to_inv(std::move(identity_matrix()));
    //std::cout << "---\nE:\n" << E_to_inv << std::endl;
    square_matrix<T, dim> this_to_E(*this);
    //std::cout << "this:\n" << this_to_E << std::endl;
    for(size_t i = 1; i < dim; ++i)
    {
      for(size_t j = i; j < dim; ++j)
      {
        T koeff = -this_to_E(j, i - 1) / this_to_E(i - 1, i - 1);
        this_to_E.row(j) += koeff * this_to_E.crow(i - 1);
        E_to_inv.row(j) += koeff * E_to_inv.crow(i - 1);
      }
    }

    //std::cout << "E_to_inv(partly):\n" << E_to_inv << std::endl;
    //std::cout << "this_to_E(partly):\n" << this_to_E << std::endl;

    for(size_t i = dim; i > 0; --i)
    {
      T diag_koeff = this_to_E(i - 1, i - 1);
      E_to_inv.row(i - 1) = E_to_inv.crow(i - 1) / diag_koeff;
      this_to_E.row(i - 1) = this_to_E.crow(i - 1) / diag_koeff;
      //this_to_E(i - 1, i - 1) = static_cast<T>(1);
      
      //std::cout << "i = " << i - 1 << std::endl;
      //std::cout << "E_to_inv(partly):\n" << E_to_inv << std::endl;
      //std::cout << "this_to_E(partly):\n" << this_to_E << std::endl;

      for(size_t j = i; j < dim; ++j)
      {
        T koeff = this_to_E(i - 1, j);
        E_to_inv.row(i - 1) -= E_to_inv.crow(j) * koeff;
        this_to_E.row(i - 1) -= this_to_E.crow(j) * koeff;
        //this_to_E(i - 1, j) = static_cast<T>(0);
        //std::cout << "i = " << i - 1 << ", j = " << j << std::endl;
        //std::cout << "E_to_inv(partly):\n" << E_to_inv << std::endl;
        //std::cout << "this_to_E(partly):\n" << this_to_E << std::endl;
      }
    }

    //std::cout << "E_to_inv:\n" << E_to_inv << std::endl;
    //std::cout << "this_to_E:\n" << this_to_E << std::endl;
    //std::cout << "this * this^-1:\n" << *this * E_to_inv << std::endl;
    //std::cout << "---" << std::endl;

    return std::move(E_to_inv);
#endif
  }

  T nearest_eigenvalue(T value = static_cast<T>(1)) const
  {
#ifdef RAYLEI_EIGENVALUE
    T value_entry = value;
    auto x0 = vector<T, dim>::get_random();
    auto E = std::move(identity_matrix());
    auto x1 = (*this - value * E).inversed() * x0;
    x1 /= x1.norm1();
    value = x1 * (*this * x1) / (x1 * x1);
    T error = value;

    do
    {
      T prev = value;
      x0 = x1;
      x1 = (*this - value * E).inversed() * x0;
      x1 /= x1.norm1();
      value = x1 * (*this * x1) / (x1 * x1);
      error = std::abs((value - prev) / (value + prev));
    }
    while(error > std::numeric_limits<T>::epsilon());

    std::cout << "nearest eigenvalue to " << value_entry << " is " << value << std::endl;

    return value;
#else
    auto x0 = vector<T, dim>::get_random();
    auto iteration_mtx = std::move(*this - value * identity_matrix());
    auto x1 = iteration_mtx * x0;
    x1 /= x1.norm1();
    T error = (x1 - x0).norm1();

    while(std::abs(error) > std::numeric_limits<T>::epsilon())
    {
      x0 = x1;
      x1 = iteration_mtx * x0;
      x1 /= x1.norm1();
      error = (x1 - x0).norm1();
    }

    T eigenvalue = (x1 * (*this * x1)) / (x1 * x1);

    std::cout << "nearest eigenvalue to " << value << " is " << eigenvalue << std::endl;

    return eigenvalue;
#endif
  }

  T max_eigenvalue() const
  {
    auto x0 = vector<T, dim>::get_random();
    auto x1 = *this * x0;
    T res = (x1 * x0) / (x0 * x0);
    T error = res;

    do
    {
      T prev = res;
      x0 = x1;
      x1 = *this * x0;
      res = (x1 * x0) / (x0 * x0);
      error = std::abs((res - prev) / 2.0 / (res + prev));
    }
    while(error > std::numeric_limits<T>::epsilon());

    if constexpr(std::is_floating_point<T>::value)
    {
      if(std::isnan(res) || std::isinf(res))
      {
      T near_value = -std::abs(norm1());
      size_t count = 1001;
      T step = -2 * near_value / (count - 1);

      while(--count)
      {
        res = nearest_eigenvalue(near_value);
        near_value += step;
      }
      }
    }

    return res;
  }

  T min_eigenvalue() const
  {
    return static_cast<T>(1) / inversed().max_eigenvalue();
  }

  T norm1() const
  {
    T res;
    for(size_t i = 0; i < dim; ++i)
    {
      std::valarray<T> cur_col = ccol(i);
      cur_col.apply(std::abs);
      T preres = cur_col.sum();

      if(i == 0 || res < preres)
        res = preres;
    }

    return res;
  }

  T norm2() const
  {
    T res = static_cast<T>(0);
    for(size_t j = 0; j < dim; ++j)
    {
      std::valarray<T> cur_row = crow(j);
      cur_row.apply(std::abs);
      T preres = cur_row.sum();

      if(j == 0 || std::abs(res) < std::abs(preres))
        res = preres;
    }

    return res;
  }

  T norm3() const
  {
    square_matrix<T, dim> mtx_mult(*this * conj());
    T max_lambda = mtx_mult.max_eigenvalue();
    return std::sqrt(max_lambda);
  }

  T condition_number(size_t norm) const
  {
    if(norm == 1)
    {
      return norm1() * inversed().norm1();
    }
    else if(norm == 2)
    {
      return norm2() * inversed().norm2();
    }
    else if(norm == 3)
    {
      return norm3() * inversed().norm3();
    }
    else
    {
      throw std::exception{};
    }
  }

  square_matrix<T, dim> getL() const
  {
    square_matrix<T, dim> L(static_cast<T>(0));
    for(size_t i = 0; i < dim; ++i)
    {
      for(size_t j = 0; j < dim; ++j)
      {
        if(i > j)
          L(i, j) = data[i * dim + j];
      }
    }

    return std::move(L);
  }

  square_matrix<T, dim> getD() const
  {
    square_matrix<T, dim> D(static_cast<T>(0));
    for(size_t i = 0; i < dim; ++i)
    {
      for(size_t j = 0; j < dim; ++j)
      {
        if(i == j)
          D(i, j) = data[i * dim + j];
      }
    }

    return std::move(D);
  }

  square_matrix<T, dim> getU() const
  {
    square_matrix<T, dim> U(static_cast<T>(0));
    for(size_t i = 0; i < dim; ++i)
    {
      for(size_t j = 0; j < dim; ++j)
      {
        if(i < j)
          U(i, j) = data[i * dim + j];
      }
    }

    return std::move(U);
  }
}; //class square_matrix

};

