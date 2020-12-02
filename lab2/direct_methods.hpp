#pragma once

#include <comp_assert.hpp>
#include <matrix.hpp>
#include <vector.hpp>

#include <iostream>

namespace comp
{

template<typename T, size_t dim>
auto solve_Gauss(const square_matrix<T, dim>& A, const vector<T, dim>& f)
{
  COMP_ASSERT(A(0, 0) != static_cast<T>(0));

  // std::cout << A << std::endl << f << std::endl;

  if constexpr (dim == 1)
  {
    vector<T, dim> res{f[0] / A(0, 0)};
    // std::cout << "preres:" << std::endl << res << std::endl;
    // std::cout << "res:" << std::endl << res << std::endl;
    return std::move(res);
  }
  else
  {
    square_matrix<T, dim - 1> A1 = A.minor(0, 0);
    vector<T, dim - 1> f1(f[std::slice(1, dim - 1, 1)]);
    for(size_t i = 0; i < dim - 1; ++i)
    {
      T koeff = A(i + 1, 0) / A(0, 0);
      A1.row(i) -= koeff * A[std::slice(1, dim - 1, 1)];
      f1[i] -= koeff * f[0];
    }

    auto subres = std::move(solve_Gauss(A1, f1));
    vector<T, dim> res{subres, 1};
    res[0] = f[0];

    // std::cout << "preres:" << std::endl << res << std::endl;

    for(size_t i = 1; i < dim; ++i)
      res[0] -= A(0, i) * res[i];
    res[0] /= A(0, 0);

    // std::cout << "res:" << std::endl << res << std::endl;

    return std::move(res);
  }
}

} // namespace comp

