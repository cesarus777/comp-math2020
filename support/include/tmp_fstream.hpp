#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <fstream>
#include <stdlib.h>



namespace fs = std::filesystem;



namespace comp::support
{

class tmp_ifstream
{
protected:
  fs::path filename;
  std::ifstream ifs;
public:
  explicit tmp_ifstream()
  {
    char tmpname[] = "/tmp/XXXXXX"; 
    int fd = mkstemp(tmpname);
    if(fd < 0)
      throw std::runtime_error{"Can't create temporary file"};

    filename = fs::read_symlink(fs::path{"/proc/self/fd"} / std::to_string(fd));

    ifs.open(filename);
    if(ifs.is_open() == false)
      throw std::runtime_error{ std::string{"Can;t open ifstream from "} + 
                                filename.c_str() };
  }

  tmp_ifstream(const tmp_ifstream& other) = delete;

  tmp_ifstream(tmp_ifstream&& other) 
      : filename { std::move(other.filename) },
        ifs      { std::move(other.ifs) }
  {}

  template<typename T>
  tmp_ifstream& operator>>(T& elem)
  {
    ifs >> elem;
    if(ifs.fail())
      throw std::runtime_error{"tmp_ifstream fail!"};

    ifs.flush();

    return *this;
  }

  ~tmp_ifstream()
  {
    ifs.close();
    std::remove(filename.c_str());
  }

  inline const fs::path& name() const { return filename; }

}; // class tmp_ifstream



class tmp_ofstream
{
protected:
  fs::path filename;
  std::ofstream ofs;
public:
  explicit tmp_ofstream()
  {
    char tmpname[] = "/tmp/XXXXXX"; 
    int fd = mkstemp(tmpname);
    if(fd < 0)
      throw std::runtime_error{"Can't create temporary file"};

    filename = fs::read_symlink(fs::path{"/proc/self/fd"} / std::to_string(fd));

    ofs.open(filename);
    if(ofs.is_open() == false)
      throw std::runtime_error{ std::string{"Can't open ofstream from "} + 
                                filename.c_str() };
  }

  tmp_ofstream(const tmp_ofstream& other) = delete;

  tmp_ofstream(tmp_ofstream&& other) 
      : filename { std::move(other.filename) },
        ofs      { std::move(other.ofs) }
  {}

  template<typename T>
  tmp_ofstream& operator<<(T& elem)
  {
    ofs << elem;
    if(ofs.fail())
      throw std::runtime_error{"tmp_ofstream fail!"};

    ofs.flush();

    return *this;
  }

  ~tmp_ofstream()
  {
    ofs.close();
    std::remove(filename.c_str());
  }

  inline const fs::path& name() const { return filename; }

}; // class tmp_ofstream



class tmp_fstream
{
protected:
  fs::path filename;
  std::fstream iofs;
public:
  explicit tmp_fstream()
  {
    char tmpname[] = "/tmp/XXXXXX"; 
    int fd = mkstemp(tmpname);
    if(fd < 0)
      throw std::runtime_error{"Can't create temporary file"};

    filename = fs::read_symlink(fs::path{"/proc/self/fd"} / std::to_string(fd));

    iofs.open(filename);
    if(iofs.is_open() == false)
      throw std::runtime_error{ std::string{"Can't open fstream from "} + 
                                filename.c_str() };
  }

  tmp_fstream(const tmp_fstream& other) = delete;

  tmp_fstream(tmp_fstream&& other) 
      : filename { std::move(other.filename) },
        iofs     { std::move(other.iofs) }
  {}

  template<typename T>
  tmp_fstream& operator<<(T& elem)
  {
    iofs << elem;
    if(iofs.fail())
      throw std::runtime_error{"tmp_fstream fail!"};

    iofs.flush();

    return *this;
  }

  template<typename T>
  tmp_fstream& operator>>(T& elem)
  {
    iofs >> elem;
    if(iofs.fail())
      throw std::runtime_error{"tmp_fstream fail!"};

    iofs.flush();

    return *this;
  }

  ~tmp_fstream()
  {
    iofs.close();
    std::remove(filename.c_str());
  }

  inline const fs::path& name() const { return filename; }

}; // class tmp_fstream

}; // namespace comp::support

