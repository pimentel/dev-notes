#include <cstring>
#include <iostream>
#include <string>
#include <vector>

std::unique_ptr<const char*> string_to_cptr(const std::vector<std::string>& v) {
  std::unique_ptr<const char*> ret( new const char*[v.size()] );
  for (size_t i = 0; i < v.size(); ++i) {
    ret.get()[i] = v[i].c_str();
  }

  return std::move(ret);
}


int main(int argc, char *argv[])
{
  std::vector<std::string> str_vec;

  const auto NSTR = 1000;
  for (auto i = 0; i < NSTR; ++i) {
    std::string cur_str = std::to_string( i );
    str_vec.push_back( cur_str );
  }

  /* const char** cvec = string_to_cptr(str_vec); */
  std::unique_ptr<const char*> cvec = string_to_cptr(str_vec);
  for (auto i = 0; i < 100; ++i) {
    std::cout << cvec.get()[i] << "\t" << strlen(cvec.get()[i]) << std::endl;
  }
  std::cout << std::endl;


  /* delete [] cvec; */
  return 0;
}
