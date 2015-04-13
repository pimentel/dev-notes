library("microbenchmark")

create_class_structure <- function() {
  structure(list(), class = "a_class")
}

create_class_class <- function() {
  x <- list()
  class(x) <- "a_class"
  x
}

microbenchmark(
  create_class_structure(),
  create_class_class(),
  times = 1000L
  )

# Unit: nanoseconds
#                      expr  min   lq     mean median     uq   max neval
#  create_class_structure() 5924 6490 7365.700 6909.0 7404.5 40591  1000
#      create_class_class()  885 1004 1288.752 1103.5 1244.0 12787  1000
