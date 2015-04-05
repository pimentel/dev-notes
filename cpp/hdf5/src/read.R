library("rhdf5")

fname <- "../output.h5"
h5ls(fname)
h5read(fname, "/sup")

fname2 <- "../output2.h5"
h5read(fname2, "/sup")
