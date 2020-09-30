#pragma once

#include "functions.hpp"
#include "gnuplot.hpp"
#include "plot_data.hpp"
#include "tmp_fstream.hpp"

#include <array>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <tuple>



namespace comp::config
{
  enum { STEP_LIMIT = 21 };
  constexpr static double x0 = 5.0;
}; // namespace comp::config



namespace comp
{

template<typename T>
static comp::basic_function& get_func(T& container, size_t i)
{
  switch(i)
  {
    case 0: return std::get<0>(container);
    case 1: return std::get<1>(container);
    case 2: return std::get<2>(container);
    case 3: return std::get<3>(container);
    case 4: return std::get<4>(container);
    default: throw std::runtime_error("Can't get element with too big index!");
  }
}

template<typename T>
static const comp::basic_function& get_func(const T& container, size_t i)
{
  switch(i)
  {
    case 0: return std::get<0>(container);
    case 1: return std::get<1>(container);
    case 2: return std::get<2>(container);
    case 3: return std::get<3>(container);
    case 4: return std::get<4>(container);
    default: throw std::runtime_error("Can't get element with too big index!");
  }
}



template<int max_precision, class ... Funcs>
class context
{
private:
  enum output_type_t
  {
    WINDOW,
    PNG,
    JPG,
  };

  double x0;
  std::tuple<Funcs...> functions;
  output_type_t output_type = WINDOW;
  fs::path output_dir = "./output/";
  size_t graph_width = 1600;
  size_t graph_height = 900;
  bool be_quiet = false;
  bool dump_data = false;
  bool logging = false;
  bool linear = true;

public:
  explicit context(double x, const std::vector<std::string>& args)
      : x0{x}, functions{ Funcs{}... }
  {
    for(auto arg = args.cbegin(); ++arg != args.cend();)
    {
      if(*arg == "-o")
      {
        arg++;

        if(*arg == "window")
          output_type = WINDOW;
        else if(*arg == "png")
          output_type = PNG;
        else if(*arg == "jpg")
          output_type = JPG;
        else
          throw std::runtime_error{"Bad output type!"};
      }
      else if(*arg == "-size")
      {
        arg++;

        std::stringstream ss{*arg};
        ss >> graph_width;
        ss.ignore(1);
        ss >> graph_height;

        if(ss.fail())
          throw std::runtime_error{"Bad plot size!"};
      }
      else if(*arg == "-v" || *arg == "--verbose")
      {
        logging = true;
      }
      else if(*arg == "--nonlinear")
      {
        linear = false;
      }
      else if(*arg == "--dump-data")
      {
        dump_data = true;
      }
      else if(*arg == "-q")
      {
        be_quiet = true;
      }
      else if(*arg == "-od")
      {
        if(++arg == args.cend() || *arg == "")
        {
          std::cerr << "error: " <<  "Output directory can't be empty."
                    << " Outputting in default directory: " << output_dir
                    << std::endl;
          continue;
        }

        fs::path odir{*arg};

        if(fs::is_directory(odir) == false)
        {
          std::cerr << "error: " <<  odir << " is not a directory."
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



  const char *output() const
  {
    switch(output_type)
    {
      case WINDOW: return "window";
      case PNG: return "png";
      case JPG: return "jpg";
      default:
        throw std::runtime_error{"Unknown output type"};
    };
  }



  inline size_t xsize() const noexcept { return graph_width; }

  inline size_t ysize() const noexcept { return graph_height; }

  inline bool dump() const noexcept { return dump_data; }

  inline bool log() const noexcept { return logging; }

  inline bool nonlinear() const noexcept { return !linear; }



  auto&& error(const size_t index, const int precision) const
  {
    double analytical = get_func(functions, index).analytical_derivative()(x0);

    auto numerical = basic_function::num_derivative{get_func(functions, index)}.
                                                  compute(x0, exp2(-precision));

    for(auto& item : numerical)
    {
      item = std::abs(analytical - item);
    }

    return std::move(numerical);
  }



  auto&& compute_errors() const
  {
    constexpr size_t n = sizeof...(Funcs);

    std::array<plot_data<double, 5, max_precision>, n> res;

    for(size_t i = 0; i < n; ++i)
    {
      for(int j = 0; j < max_precision; ++j)
      {
        res[i][j] = error(i, j);
      }
    }

    return std::move(res);
  }



  void plot(const plot_data<double, 5, config::STEP_LIMIT>& data,
            std::string_view data_name_in) const
  {
    support::gnuplot plot{logging};
    support::tmp_ofstream data_file;
    std::ostringstream data_name;
    data_name << output_dir.c_str() << data_name_in;

    if(linear == false)
    {
      data_name << "_nonlinear";
      plot << "set nonlinear y via log10(y) inverse 10**y\n";
    }

    if(logging)
      std::clog << "tmpfile name : " << data_file.name() << std::endl;

    data_file << data;
    plot << "filename='" << data_file.name().c_str() << "'\n";
    plot << "ofilename='" << data_name.str() << "'\n";

    plot << "xrange_lim=" << config::STEP_LIMIT << "\n";
    plot << "xsize=" << graph_width << "\n";
    plot << "ysize=" << graph_height << "\n";

    if((output_type != WINDOW) || (be_quiet == false))
    {
      plot << "load 'plot_to_" << output() << ".gnu'" << "\n";

      // waiting gnuplot for reading from tmpfile
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if(dump_data)
      std::ofstream{data_name.str() + ".txt"} << data;

    if(logging)
    {
      std::cout << "press any key to continue..." << std::endl;
      std::cin.clear();
      std::cin.ignore(1);
    }
  }

  void plot_all(std::string_view data_name_template = "function_") const
  {
    auto data = compute_errors();

    size_t n = 1;

    std::string data_name{};

    for(const auto& data_set : data)
    {
      data_name.erase();
      data_name += data_name_template.data();
      data_name += std::to_string(n++);

      if(logging)
      {
        std::clog << data_name << " data set:" << std::endl;
        std::clog << data_set << std::endl;
      }

      plot(data_set, data_name);
    }
  }
}; // class context

}; // namespace comp

