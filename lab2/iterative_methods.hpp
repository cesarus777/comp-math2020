#pragma once

#include <comp_assert.hpp>
#include <matrix.hpp>
#include <vector.hpp>

#include <iostream>

namespace comp
{

template<typename T, size_t dim>
auto solve_Seidel(const square_matrix<T, dim>& A, const vector<T, dim>& f,
                  const int precision = 6)
{
  //std::cout << "A:\n" << A << std::endl;
  //std::cout << "A^-1:\n" << A.inversed() << std::endl;
  //std::cout << "f:\n" << f << std::endl;
  //std::cout << "L:\n" << A.getL() << std::endl;
  //std::cout << "D:\n" << A.getD() << std::endl;
  //std::cout << "U:\n" << A.getU() << std::endl;

  //std::cout << "L + D:\n" << A.getL() + A.getD() << std::endl;
  //std::cout << "(L + D)^-1:\n" << (A.getL() + A.getD()).inversed() << std::endl;

  square_matrix<T, dim> L_plus_D_inv(std::move((A.getL()+A.getD()).inversed()));
  square_matrix<T, dim> R = -L_plus_D_inv * A.getU();
  vector<T, dim> F = L_plus_D_inv * f;

  //std::cout << "R:\n" << R << std::endl;
  //std::cout << "F:\n" << F << std::endl;

  vector<T, dim> u = vector<T, dim>::get_random();
  // vector<T, dim> u(static_cast<T>(0));
  vector<T, dim> u_next = R * u + F;

  T epsilon = R.norm1() * std::pow(10.0, -precision);

  while(std::abs((u_next - u).norm3()) > std::abs(epsilon))
  {
    u = u_next;
    u_next = R * u + F;
  }

  return std::move(u_next);
}

} // namespace comp

