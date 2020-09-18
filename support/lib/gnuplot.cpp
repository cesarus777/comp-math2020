#include "gnuplot.hpp"

#include <cerrno>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>



using namespace comp::support;



gnuplot::gnuplot(bool logging) : log{logging}
{
  *this = std::move(gnuplot{"", logging});
}



gnuplot::gnuplot(std::string_view args, bool logging) : log{logging}
{
  std::string config{"gnuplot"};

  if(!args.empty())
    config.append(" ").append(args);

  pipe = popen(config.data(), "w");

  if(log)
    std::clog << config << std::endl;

  if(!pipe)
    throw std::runtime_error{"Can't open gnuplot pipe"};
}



gnuplot::gnuplot(gnuplot&& other)
{
  log = other.log;
  pipe = other.pipe;
  other.pipe = NULL;
}



gnuplot& gnuplot::operator=(gnuplot&& other)
{
  log = other.log;
  pipe = other.pipe;
  other.pipe = NULL;

  return *this;
}



gnuplot::~gnuplot()
{
  if(pipe)
  {
    fprintf(pipe, "exit\n");

    if(pclose(pipe) < 0)
      std::cerr << std::error_code{errno, std::generic_category()}.message()
                << std::endl;
  }
}



void gnuplot::operator()(const std::string& command)
{
  exec_command(command + "\n");
}



void gnuplot::exec_command(std::string_view command)
{
  int n = fprintf(pipe, "%s", command.data());

  if(log)
    std::clog << command;

  if(n != command.length())
    throw std::runtime_error{"Can't write command to plot"};

  if(fflush(pipe))
    throw std::system_error{std::error_code{errno, std::generic_category()}};
}

