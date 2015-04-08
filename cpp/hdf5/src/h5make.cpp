#include <iostream>
#include <assert.h>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "hdf5.h"

char** vec_to_ptr(const std::vector<std::string>& v) {
  char** ret;
  ret = new char*[v.size()];
  for (size_t i = 0; i < v.size(); ++i) {
    ret[i] = new char[10];
    strcpy(ret[i], v[i].c_str());
    /* ret[i] = v[i].c_str(); */
  }
  return ret;
}

const double* vec_to_ptr(const std::vector<double>& v) {
  return v.data();
}

const int* vec_to_ptr(const std::vector<int>& v) {
  return v.data();
}

hid_t get_datatype_id(const std::vector<std::string>& v) {
  hid_t datatype_id = H5Tcopy (H5T_C_S1);
  /* H5Tset_size (datatype_id, H5T_VARIABLE); */
  herr_t status = H5Tset_size (datatype_id, 10);
  assert( status >= 0 );
  v.size(); // shutup, compiler
  return datatype_id;
}

hid_t get_datatype_id(const std::vector<double>& v) {
  v.size(); // shutup, compiler
  return H5T_NATIVE_DOUBLE;
}

hid_t get_datatype_id(const std::vector<int>& v) {
  v.size(); // shutup, compiler
  return H5T_NATIVE_INT;
}

// str_vec: a vector of string to be written out
// group_id: a group_id which has already been opened
// dataset_name: the to write out to
// release_type: if 'true', release the datatype
// compression_level: the level of compression (6 seems reasonable)
//
// return: the status of H5Dwrite (last H5 operation)
template <typename T>
herr_t data_to_h5(
    const std::vector<T>& str_vec,
    hid_t group_id,
    const std::string& dataset_name,
    bool release_type,
    uint compression_level = 6
    ) {
  herr_t status;

  hsize_t dims[1] = {str_vec.size()};

  // create the propery which allows for compression
  hid_t prop_id = H5Pcreate(H5P_DATASET_CREATE);
  // chunk size is same size as vector
  status = H5Pset_chunk(prop_id, 1, dims);
  assert( status >= 0 );
  status = H5Pset_deflate(prop_id, compression_level);
  assert( status >= 0 );

  // create the data type
  hid_t datatype_id = get_datatype_id(str_vec);

  // create the dataspace
  hid_t dataspace_id = H5Screate_simple(1, dims, NULL);

  // create the dataset
  hid_t dataset_id = H5Dcreate(group_id, dataset_name.c_str(), datatype_id,
      dataspace_id, H5P_DEFAULT, prop_id, H5P_DEFAULT);

  // get the ptrs from the string and write out
  auto ptr = vec_to_ptr(str_vec);
  status = H5Dwrite(dataset_id, datatype_id, H5S_ALL, H5S_ALL, H5P_DEFAULT,
      ptr);
  assert( status >= 0 );

  status = H5Pclose(prop_id);
  assert( status >= 0 );
  status = H5Dclose(dataset_id);
  assert( status >= 0 );
  status = H5Sclose(dataspace_id);
  assert( status >= 0 );
  if (release_type) {
    status = H5Tclose(datatype_id);
    assert( status >= 0 );
    delete [] ptr;
  }

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

  const auto NSTR = 1000;
  for (auto i = 0; i < NSTR; ++i) {
    std::string cur_str = std::to_string( i );
    str_vec.push_back( cur_str );
  }

  // std::unique_ptr<const char*> out_ptr = string_to_cptr(str_vec);
  hid_t file_id;
  file_id = H5Fcreate(fname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  herr_t status;

  hid_t root;
  root = H5Gopen(file_id, "/", H5P_DEFAULT);

  std::vector<double> dbl_vec;
  dbl_vec.reserve(NSTR);
  for (size_t i = 0; i < NSTR; ++i) {
    dbl_vec.push_back( static_cast<double>(i + 0.5) );
  }
  status = data_to_h5(str_vec, root, "stringstuff", true, 6);
  std::cout << "double" << std::endl;
  status = data_to_h5(dbl_vec, root, "dub", false, 6);
  std::vector<int> int_vec;
  int_vec.push_back( 100 );
  status = data_to_h5(int_vec, root, "aint", false, 6);

  hid_t aux_group = H5Gcreate(file_id, "/aux", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  int_vec.push_back( 3 );
  status = data_to_h5(int_vec, aux_group, "aint", false, 6);

  status = H5Gclose(root);
  status = H5Gclose(aux_group);
  status = H5Fclose(file_id);

  return 0;
}
