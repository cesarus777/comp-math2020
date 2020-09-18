#include "compute.hpp"
#include "gnuplot.hpp"

#include <string>
#include <vector>



namespace fs = std::filesystem;



int main(int argc, const char *argv[])
{
  try
  {
    comp::context<comp::config::STEP_LIMIT, comp::function_sin_x_x,
                  comp::function_cos_sin_x, comp::function_exp_sin_cos_x,
                  comp::function_ln_x_plus_3, comp::function_sqrt_x_plus_3>
        ctx{comp::config::x0, std::vector<std::string>{argv, argv + argc}};

    ctx.plot_all();
  }
  catch(const std::exception& e)
  {
    std::cerr << "comp-math2020/lab1: fatal error: " << e.what() << std::endl;
  }
}

