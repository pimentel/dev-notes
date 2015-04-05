find_package(HDF5)

# add_executable(ex h5ex_d_chunk.c)
add_executable(wri h5make.cpp)

if(HDF5_FOUND)
    include_directories( ${HDF5_INCLUDE_DIR} )
    # target_link_libraries( ex ${HDF5_LIBRARIES} )
    target_link_libraries( wri ${HDF5_LIBRARIES} )
else()
    message(FATAL_ERROR "HDF5 not found. Required to output files")
endif()


