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
