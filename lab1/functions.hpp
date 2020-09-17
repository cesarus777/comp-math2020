#pragma once

#include <cmath>
#include <functional>
#include <iostream>



namespace comp
{

class basic_function
{
protected:
  using function = std::function<double(double)>;

  function foo;

public:
  explicit basic_function(const function& other_foo) : foo(other_foo) {}

  double compute(double x) const;

  double operator()(double x) const;

  virtual function analytical_derivative() const = 0;

  class num_derivative;

}; // class basic_function



class basic_function::num_derivative
{
private:
  function foo;

public:
  explicit num_derivative(const basic_function& basic_foo);

  double compute2(double x, double h) const; // formula (2)

  double compute3(double x, double h) const; // formula (3)

  double compute4(double x, double h) const; // formula (4)

  double compute5(double x, double h) const; // formula (5)

  double compute6(double x, double h) const; // formula (6)

  std::array<double, 5> compute(double x, double h) const;

}; // class numerical_derivative



class function_sin_x_x final : public basic_function
{
public:
  explicit function_sin_x_x();

  double operator()(double x) const;

  virtual function analytical_derivative() const;

}; // class function_sin_x_x



class function_cos_sin_x final : public basic_function
{
public:
  explicit function_cos_sin_x();

  double operator()(double x) const;

  virtual function analytical_derivative() const;

}; // class function_cos_sin_x



class function_exp_sin_cos_x final : public basic_function
{
public:
  explicit function_exp_sin_cos_x();

  double operator()(double x) const;

  virtual function analytical_derivative() const;

}; // class function_exp_sin_cos_x



class function_ln_x_plus_3 final : public basic_function
{
public:
  explicit function_ln_x_plus_3();

  double operator()(double x) const;

  virtual function analytical_derivative() const;

}; // class function_ln_x_plus_3



class function_sqrt_x_plus_3 final : public basic_function
{
public:
  explicit function_sqrt_x_plus_3();

  double operator()(double x) const;

  virtual function analytical_derivative() const;

}; // class function_sqrt_x_plus_3

}; // namespace comp



template<typename T, size_t size>
std::ostream& operator<<(std::ostream& os, const std::array<T, size>& arr)
{
  os << "[";
  for(size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      os << ',';

    os << ' ' << arr[i];
  }
  os << " ]";
  return os;
}

