#include "functions.hpp"

#include <cmath>
#include <functional>
#include <iostream>



using function = std::function<double(double)>;

using namespace comp;



double basic_function::compute(double x) const
{
  return foo(x);
}

double basic_function::operator()(double x) const
{
  return compute(x);
}



basic_function::num_derivative::num_derivative(
    const basic_function& basic_foo) : foo{basic_foo.foo}
{}

// formula(2)
double basic_function::num_derivative::compute2(double x, double h) const
{
  return (foo(x + h) - foo(x)) / h;
}

// formula(3)
double basic_function::num_derivative::compute3(double x, double h) const
{
  return (foo(x) - foo(x - h)) / h;
}

// formula(4)
double basic_function::num_derivative::compute4(double x, double h) const
{
  return (foo(x + h) - foo(x - h)) / (2 * h);
}

// formula(5)
double basic_function::num_derivative::compute5(double x, double h) const
{
  return (4 / 3) * (foo(x + h) - foo(x - h)) / (2 * h) -
      (1 / 3) * (foo(x + 2 * h) - foo(x - 2 * h)) / (4 * h);
}

// formula(6)
double basic_function::num_derivative::compute6(double x, double h) const
{
  return (3 / 2) * (foo(x + h) - foo(x - h)) / (2 * h) -
       (3 / 5) * (foo(x + 2 * h) - foo(x - 2 * h)) / (4 * h) + 
       (1 / 10) * (foo(x + 3 * h) - foo(x - 3 * h)) / (6 * h);
}

std::array<double, 5> basic_function::num_derivative::compute(double x,
                                                              double h) const
{
  return { compute2(x, h), compute3(x, h), compute4(x, h),
           compute5(x, h), compute6(x, h) };
}



function_sin_x_x::function_sin_x_x()
    : basic_function( [](double x)
                      {
                        return std::sin(x * x);
                      })
{}

double function_sin_x_x::operator()(double x) const
{
  return compute(x);
}

function function_sin_x_x::analytical_derivative() const
{
  return [](double x)
         {
           return 2 * x * std::cos(x * x);
         };
}



function_cos_sin_x::function_cos_sin_x()
    : basic_function( [](double x)
                      {
                        return std::cos(std::sin(x));
                      })
{}

double function_cos_sin_x::operator()(double x) const
{
  return compute(x);
}

function function_cos_sin_x::analytical_derivative() const
{
  return [](double x)
         {
           return -std::cos(x) * std::sin(std::sin(x));
         };
}



function_exp_sin_cos_x::function_exp_sin_cos_x()
    : basic_function( [](double x)
                      {
                        return std::exp(std::sin(std::cos(x)));
                      })
{}

double function_exp_sin_cos_x::operator()(double x) const
{
  return compute(x);
}

function function_exp_sin_cos_x::analytical_derivative() const
{
  return [](double x)
         {
           return -std::sin(x) * std::cos(std::cos(x)) *
               std::exp(std::sin(std::cos(x)));
         };
}



function_ln_x_plus_3::function_ln_x_plus_3()
    : basic_function( [](double x)
                      {
                        return std::log(x + 3.0);
                      })
{}

double function_ln_x_plus_3::operator()(double x) const
{
  return compute(x);
}

function function_ln_x_plus_3::analytical_derivative() const
{
  return [](double x)
         {
           return 1.0 / (x + 3.0);
         };
}



function_sqrt_x_plus_3::function_sqrt_x_plus_3()
    : basic_function( [](double x)
                      {
                        return std::sqrt(x + 3.0);
                      })
{}

double function_sqrt_x_plus_3::operator()(double x) const
{
  return compute(x);
}

function function_sqrt_x_plus_3::analytical_derivative() const
{
  return [](double x)
         {
           return 1.0 / 2.0 / std::sqrt(x + 3);
         };
}



/*template<typename T, size_t size>
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
}*/

