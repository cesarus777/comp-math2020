#pragma once

#include <matrix.hpp>
#include <tmp_fstream.hpp>
#include <vector.hpp>

#include "direct_methods.hpp"
#include "iterative_methods.hpp"

#include <iostream>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>



namespace fs = std::filesystem;



namespace comp
{

template<typename T, size_t dim>
class context
{
private:
  fs::path output_dir = "./output/";
  bool be_quiet = false;
  bool dump_data = false;
  bool logging = false;

  std::vector<std::pair<std::string, T>> values;
  std::vector<std::pair<std::string, vector<T, dim>>> solutions;
  std::vector<std::pair<std::string, vector<T, dim>>> residuals;

public:
  context(const std::vector<std::string>& args)
  {
    for(auto arg = args.cbegin(); ++arg != args.cend();)
    {
      if(*arg == "-v" || *arg == "--verbose")
      {
        logging = true;
      }
      else if(*arg == "--dump-data")
      {
        dump_data = true;
      }
      else if(*arg == "-q")
      {
        be_quiet = true;
      }
      else if(*arg == "-d")
      {
        if(++arg == args.cend() || *arg == "")
        {
          std::cerr << "error: " << "Output directory can't be empty."
                    << " Outputting in default directory: " << output_dir
                    << std::endl;
        }

        fs::path odir{*arg};

        if(fs::is_directory(odir) == false)
        {
          std::cerr << "error: " << odir << " is not a directory."
                    << " Outputting in default directory: " << output_dir
                    << std::endl;
        }
        else
        {
          output_dir = std::move(odir / "");
        }
      }
      else
      {
        std::cerr << "error: " << "Unknown command line argument: " << *arg
                  << std::endl;
      }
    }
  }

  void compute(const comp::square_matrix<T, dim>& A, const comp::vector<T, dim>& f)
  {
    values.clear();
    solutions.clear();
    residuals.clear();

    T min_eigenvalue = A.min_eigenvalue();
    T max_eigenvalue = A.max_eigenvalue();
    T condition_number = A.condition_number(2);
    values.push_back({ "Min eigenvalue", min_eigenvalue, });
    values.push_back({ "Max eigenvalue", max_eigenvalue, });
    values.push_back({ "Condition number", condition_number, });

    auto  Gauss_solution = solve_Gauss(A, f);
    auto Seidel_solution = solve_Seidel(A, f);
    solutions.push_back({ "Gauss",   Gauss_solution, });
    solutions.push_back({ "Seidel", Seidel_solution, });

    auto  Gauss_residual = A *  Gauss_solution - f;
    auto Seidel_residual = A * Seidel_solution - f;
    residuals.push_back({ "Gauss",   Gauss_residual, });
    residuals.push_back({ "Seidel", Seidel_residual, });
  }

  void dump_output() const
  {
    support::tmp_fstream tmpfile;

    if(values.size())
    {
      for(const auto& value : values)
      {
        tmpfile << value.first << ": " << value.second << "\n";
      }
    }

    if(solutions.size())
    {
      for(const auto& solution : solutions)
      {
        tmpfile << "Solution using " << solution.first << " method:\n"
                << solution.second << "\n";
      }
    }

    if(residuals.size())
    {
      for(const auto& residual : residuals)
      {
        tmpfile << "Residual of " << residual.first << " method:\n"
                << residual.second << "\n";
      }
    }

    if(be_quiet == false)
    {
      tmpfile.dump_to(std::cout);
    }

    if(dump_data == true)
    {
      auto dump_name = output_dir / "lab2_report.txt";
      std::clog << "Dumping calculations to " << dump_name << "\n";
      tmpfile.dump_to(dump_name);
    }
  }
}; // class context

} // namespace comp

