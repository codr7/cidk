#ifndef CIDK_PATH_HPP
#define CIDK_PATH_HPP

#include <experimental/filesystem>

namespace cidk {
  using namespace std;
  namespace fs = experimental::filesystem;
  using Path = fs::path;
}

#endif
