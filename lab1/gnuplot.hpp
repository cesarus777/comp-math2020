#pragma once

#include <cstdio>
#include <filesystem>
#include <string_view>
#include <utility>
#include <vector>



namespace comp::support
{

class gnuplot
{
private:
  FILE *pipe = NULL;
  bool log;

public:
  explicit gnuplot(bool logging = false);
  explicit gnuplot(std::string_view args, bool logging = false);

  gnuplot(const gnuplot& other) = delete;
  gnuplot& operator=(const gnuplot& other) = delete;

  gnuplot(gnuplot&& other);
  gnuplot& operator=(gnuplot&& other);

  ~gnuplot();

  void exec_command(std::string_view command);

  void operator()(const std::string& command);

  template<typename T>
  gnuplot& operator<<(const T& item)
  {
    std::stringstream ss{};
    ss << item;
    exec_command(ss.str());

    return *this;
  }

}; // class gnuplot

}; // namespace support

