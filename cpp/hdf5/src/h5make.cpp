#include <iostream>
#include <cstring>
#include <string>
#include <vector>

#include "hdf5.h"

std::unique_ptr<const char*> string_to_cptr(const std::vector<std::string>& v) {
  std::unique_ptr<const char*> ret( new const char*[v.size()] );
  for (size_t i = 0; i < v.size(); ++i) {
    ret.get()[i] = v[i].c_str();
  }

  return std::move(ret);
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: h5kal output.h5" << std::endl;
    return 1;
  }

  std::cout << "Opening file " << argv[1] << std::endl;
  std::string fname(argv[1]);


  std::vector<std::string> str_vec;

  const auto NSTR = 10000;
  for (auto i = 0; i < NSTR; ++i) {
    std::string cur_str = std::to_string( i );
    str_vec.push_back( cur_str );
  }

  std::unique_ptr<const char*> out_ptr = string_to_cptr(str_vec);
  char* out_str[] = {"hello", "goodbye", "what's up"};
  hid_t file_id;
  file_id = H5Fcreate(fname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  herr_t status;
  // get the right data type for variable len c strings
  hid_t dtype = H5Tcopy (H5T_C_S1);
  status = H5Tset_size (dtype, H5T_VARIABLE);

  hid_t root;
  root = H5Gopen(file_id, "/", H5P_DEFAULT);

  hsize_t dims[1] = {NSTR};

  hid_t p_id = H5Pcreate(H5P_DATASET_CREATE);
  status = H5Pset_chunk(p_id, 1, dims);
  status = H5Pset_deflate(p_id, 6);

  hid_t data_space = H5Screate_simple(1, dims, NULL);

  hid_t dset = H5Dcreate(root, "sup", dtype, data_space,
      H5P_DEFAULT, p_id, H5P_DEFAULT);
  // hid_t dset = H5Dcreate(root, "sup", dtype, data_space,
  //     H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  std::cout << "hi!" << std::endl;

  /* status = H5Dwrite(dset, dtype, data_space, H5S_ALL, H5P_DEFAULT, out_str); */
  status = H5Dwrite(dset, dtype, data_space, H5S_ALL, H5P_DEFAULT, out_ptr.get());

  status = H5Dclose(dset);
  /* status = H5Aclose(att); */
  status = H5Gclose(root);
  status = H5Tclose(dtype);
  status = H5Sclose(data_space);
  status = H5Fclose(file_id);

  return 0;
}
