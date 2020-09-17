#pragma once
#include "gnuplot.hpp"

#include <array>



template<typename T, size_t n_cols, size_t n_points>
using data_type = std::array<std::array<T, n_cols>, n_points>;



template<typename T, size_t n_cols, size_t n_points>
class plot_data
{
private:
  using data_type = std::array<std::array<T, n_cols>, n_points>;
  data_type data;

public:
  plot_data() : data{}
  {}

  plot_data(const plot_data& other) : data{other.data}
  {}

  plot_data& operator=(const plot_data& other)
  {
    data = other.data;
  }

  plot_data(plot_data&& other) : data{std::move(other.data)}
  {}

  plot_data& operator=(plot_data&& other)
  {
    data = std::move(other.data);
  }

  ~plot_data()
  {}

  auto begin() const
  {
    return data.begin();
  }

  auto end() const
  {
    return data.end();
  }

  std::array<T, n_cols>& operator[](size_t i)
  {
    return data[i];
  }
}; // class plot_data



template<typename T, size_t n_cols, size_t n_points>
std::ostream& operator<<(std::ostream& os, const plot_data<T, n_cols, n_points>& data)
{
  int row_index = 0;

  os << row_index++;

  for(size_t col = 0; col < n_cols; col++)
    os << " formula(" << col + 2 << ")";

  os << "\n";

  for(const auto& row : data)
  {
    os << row_index++;

    for(const auto& elem : row)
      os << " " << elem;

    os << "\n";
  }

  //os << "EOF\n";

  return os;
}

