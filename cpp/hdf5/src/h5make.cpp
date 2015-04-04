#include <iostream>
#include <cstring>
#include <string>

#include "hdf5.h"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: h5kal output.h5" << std::endl;
    return 1;
  }

  std::cout << "Opening file " << argv[1] << std::endl;
  std::string fname(argv[1]);

  char* out_str[] = {"hello", "goodbye", "what's up"};
  hid_t file_id;
  file_id = H5Fcreate(fname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  herr_t status;
  // get the right data type for variable len c strings
  hid_t dtype = H5Tcopy (H5T_C_S1);
  status = H5Tset_size (dtype, H5T_VARIABLE);

  hid_t root;
  root = H5Gopen(file_id, "/", H5P_DEFAULT);

  hsize_t dims[1] = {3};
  hid_t data_space = H5Screate_simple(1, dims, NULL);
  // hid_t att;
  // att = H5Acreate(root, "sup", dtype, data_space, H5P_DEFAULT, H5P_DEFAULT);
  // status = H5Awrite(att, dtype, &out_str);
  hid_t dset = H5Dcreate(root, "sup", dtype, data_space,
      H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  std::cout << "hi!" << std::endl;
  /* status = H5Dwrite(dset, dtype, data_space, file_id, H5P_DEFAULT, out_str); */
  status = H5Dwrite(dset, dtype, data_space, H5S_ALL, H5P_DEFAULT, out_str);

  status = H5Dclose(dset);
  /* status = H5Aclose(att); */
  status = H5Gclose(root);
  status = H5Tclose(dtype);
  status = H5Sclose(data_space);
  status = H5Fclose(file_id);

  return 0;
}
