#include <matrix.hpp>
#include <vector.hpp>

#include "compute.hpp"
#include "direct_methods.hpp"
#include "iterative_methods.hpp"

#include <iostream>



enum MTX
{
  SIZE = 100lu,
  DIAG_ELEM = 10lu,
};



int main(int argc, const char *argv[])
{
  comp::square_matrix<double, MTX::SIZE> A;

  for(size_t i = 0; i < MTX::SIZE; ++i)
  {
    for(size_t j = 0; j < MTX::SIZE; ++j)
    {
      if(i == j)
        A(i, j) = MTX::DIAG_ELEM;
      else if(j < i + 2)
        A(i, j) = 1.0 / static_cast<double>(i + 1);
    }
  }

  comp::vector<double, MTX::SIZE> f;

  for(size_t i = 0; i < MTX::SIZE; ++i)
    f[i] = i + 1;

  //comp::context<double, MTX::SIZE> ctx(std::vector<std::string>(argv, argv + argc));
  //ctx.compute(A, f);
  //ctx.dump_output();

  comp::context<double, 3> ctxs(std::vector<std::string>(argv, argv + argc));
  //comp::square_matrix<double, 3> As{{ 1.0, 2.0, 3.0, 3.0, 5.0, 7.0, 1.0, 3.0, 4.0, }};
  comp::square_matrix<double, 3> As{{ 10.0, -1.0, 1.0, -1.0, 10.0, -1.0, -1.0, 1.0, 10.0, }};
  comp::vector<double, 3> fs{ 3.0, 0.0, 1.0, };
  //ctxs.compute(As, fs);
  std::cout << "As:\n" << As << std::endl;
  std::cout << "As^-1:\n" << As.inversed() << std::endl;
  //ctxs.dump_output();

  std::cout << "norm1 = " << As.norm1() << std::endl;
  std::cout << "norm2 = " << As.norm2() << std::endl;
  std::cout << "norm3 = " << As.norm3() << std::endl;

  for(double test = 0.0; test < 10.1; test+=0.1)
    As.nearest_eigenvalue(test);
  //std::cout << std::sqrt((As.conj() * As).max_eigenvalue()) << std::endl;
/*
  comp::context<std::complex<double>, 3> ctxs(std::vector<std::string>(argv, argv + argc));
  comp::square_matrix<std::complex<double>, 3> As{{ 10.0, -1.0, 1.0, -1.0, 10.0, -1.0, -1.0, 1.0, 10.0, }};
  comp::vector<std::complex<double>, 3> fs{ 3.0, 0.0, 1.0, };
  ctxs.compute(As, fs);
  ctxs.dump_output();
  */
}

