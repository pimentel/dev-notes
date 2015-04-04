library("rhdf5")

fname <- "../output.h5"
h5ls(fname)
h5read(fname, "/sup")
