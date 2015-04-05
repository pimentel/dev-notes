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

// str_vec: a vector of string to be written out
// group_id: a group_id which has already been opened
// dataset_name: the to write out to
// compression_level: the level of compression (6 seems reasonable)
herr_t string_to_hdf5(
    const std::vector<std::string>& str_vec,
    hid_t group_id,
    const std::string& dataset_name,
    uint compression_level = 6) {
  herr_t status;

  hsize_t dims[1] = {str_vec.size()};

  // create the data type
  hid_t datatype_id = H5Tcopy (H5T_C_S1);
  status = H5Tset_size (datatype_id, H5T_VARIABLE);

  // create the propery which allows for compression
  hid_t prop_id = H5Pcreate(H5P_DATASET_CREATE);
  // chunk size is same size as vector
  status = H5Pset_chunk(prop_id, 1, dims);
  status = H5Pset_deflate(prop_id, compression_level);

  // create the data space
  hid_t dataspace_id = H5Screate_simple(1, dims, NULL);

  // create the dataset
  hid_t dataset_id = H5Dcreate(group_id, dataset_name.c_str(), datatype_id,
      dataspace_id, H5P_DEFAULT, prop_id, H5P_DEFAULT);

  // get the ptrs from the string and write out
  auto cstr_ptr = string_to_cptr(str_vec);
  status = H5Dwrite(dataset_id, datatype_id, dataspace_id, H5S_ALL, H5P_DEFAULT,
      cstr_ptr.get());

  H5Dclose(dataset_id);
  H5Pclose(prop_id);
  H5Sclose(dataspace_id);
  H5Tclose(datatype_id);

  return status;
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
  hid_t file_id;
  file_id = H5Fcreate(fname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  herr_t status;
  // get the right data type for variable len c strings

  // hid_t dtype = H5Tcopy (H5T_C_S1);
  // status = H5Tset_size (dtype, H5T_VARIABLE);

  hid_t root;
  root = H5Gopen(file_id, "/", H5P_DEFAULT);

  // hsize_t dims[1] = {NSTR};

  // hid_t p_id = H5Pcreate(H5P_DATASET_CREATE);
  // status = H5Pset_chunk(p_id, 1, dims);
  // status = H5Pset_deflate(p_id, 6);

  // hid_t data_space = H5Screate_simple(1, dims, NULL);

  // hid_t dset = H5Dcreate(root, "sup", dtype, data_space,
  //     H5P_DEFAULT, p_id, H5P_DEFAULT);
  // std::cout << "hi!" << std::endl;

  // status = H5Dwrite(dset, dtype, data_space, H5S_ALL, H5P_DEFAULT, out_ptr.get());

  status = string_to_hdf5(str_vec, root, "sup", 6);

  /* status = H5Dclose(dset); */
  /* status = H5Aclose(att); */
  status = H5Gclose(root);
  /* status = H5Tclose(dtype); */
  /* status = H5Sclose(data_space); */
  status = H5Fclose(file_id);

  return 0;
}
