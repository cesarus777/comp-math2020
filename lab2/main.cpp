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

  comp::context<double, MTX::SIZE> ctx(std::vector<std::string>(argv, argv + argc));
  ctx.compute(A, f);
  ctx.dump_output();
}

