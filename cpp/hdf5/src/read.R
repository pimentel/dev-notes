library("rhdf5")

fname <- "../output.h5"
h5ls(fname)
h5read(fname, "/sup")

fname2 <- "../out.h5"
h5ls(fname2)
sup <- h5read(fname2, "/sup")
dub <- h5read(fname2, "/dub")
aint <- h5read(fname2, "/aint")
aux_aint <- h5read(fname2, "/aux/aint")
