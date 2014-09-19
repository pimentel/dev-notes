---
title: data.table notes
layout: default
output: bookdown::html_chapter
---

# data.table notes

# Introduction

This document is a set of notes for the `data.table` R package.

First, load the package:


```r
library(data.table)
```

Also load other packages that we use in this page:


```r
library(dplyr)
```

A `data.table` extends from a `data.frame` with some interesting features and
some syntactical sugar to make many common operations fast and easily
expressible in simple syntax.

<a name="setting-keys"></a>
# Making a table and setting keys

You can make a `data.table` from a `data.frame`. The basic syntax is also quite
similar. If you're going to do joins, or fast lookup statements, you should set
a **key**. The key serves as an index for fast lookups. It does not need to be
unique, and it can be more than one column.

You can set the key on creation or afterwards using the `setkey()` function:


```r
set.seed(42)

# Setting a key on creation
A <- data.table(data.frame(id = 10:1, aGroup = c(rep('a', 3), rep('b', 3), 'c',
            rep('d', 3)), val = runif(10, 1, 2)), key = "id")

# Setting a key after creation
B <- data.table(id = c(21:23, 4:8, 29:30), bGroup = c(rep('w', 3),
        rep('x', 3), 'y', rep('d', 3)), val = runif(10, 5, 6))
setkey(B, id)

head(A, 3)
```

```
##    id aGroup   val
## 1:  1      d 1.705
## 2:  2      d 1.657
## 3:  3      d 1.135
```

```r
head(B, 3)
```

```
##    id bGroup   val
## 1:  4      x 5.255
## 2:  5      x 5.462
## 3:  6      x 5.940
```

You'll notice that both methods have the same effect. `data.table` sorts by the
key (a one time cost) so that lookups can be fast.

# Lookups (a.k.a. SELECT)

TODO... 

# Joins

Joins take the rows from two tables and attempt to "join" them based on
a common identifier, often called a **key**. See [Setting keys](#setting-keys)
for more info on setting keys. See
[StackOverflow](http://stackoverflow.com/questions/406294/left-join-and-left-outer-join-in-sql-server)
for more information on joins.

Joins are done with the syntax `X[Y]`. This will take the rows from `Y`, find
the matching ones in `X`, then output them


```r
A[B]
```

```
##     id aGroup   val bGroup val.1
##  1:  4      c 1.737      x 5.255
##  2:  5      b 1.519      x 5.462
##  3:  6      b 1.642      x 5.940
##  4:  7      b 1.830      y 5.978
##  5:  8      a 1.286      d 5.117
##  6: 21     NA    NA      w 5.458
##  7: 22     NA    NA      w 5.719
##  8: 23     NA    NA      w 5.935
##  9: 29     NA    NA      d 5.475
## 10: 30     NA    NA      d 5.560
```

Notice that it used ALL the rows of `B`. This is often known as a *left join*
or *outer join*. In `data.table` speak this is called *inhereted scope*. The
reason being is that all the columns are available to both `A` and `B`.

## Inner join
To do an inner join, that is the intersection of keys between `A` and `B`, set
the value `nomatch = 0`.


```r
A[B,,nomatch = 0]
```

```
##    id aGroup   val bGroup val.1
## 1:  4      c 1.737      x 5.255
## 2:  5      b 1.519      x 5.462
## 3:  6      b 1.642      x 5.940
## 4:  7      b 1.830      y 5.978
## 5:  8      a 1.286      d 5.117
```

Note that for inner joins, the order doesn't matter as it produces the
intersection:


```r
B[A,,nomatch = 0]
```

```
##    id bGroup   val aGroup val.1
## 1:  4      x 5.255      c 1.737
## 2:  5      x 5.462      b 1.519
## 3:  6      x 5.940      b 1.642
## 4:  7      y 5.978      b 1.830
## 5:  8      d 5.117      a 1.286
```

### Other packages

Using **dplyr**:


```r
inner_join(A, B, by = 'id')
```

```
##   id aGroup val.x bGroup val.y
## 1  4      c 1.737      x 5.255
## 2  5      b 1.519      x 5.462
## 3  6      b 1.642      x 5.940
## 4  7      b 1.830      y 5.978
## 5  8      a 1.286      d 5.117
```

Using **base**:


```r
merge(data.frame(A), data.frame(B), by = 'id')
```

```
##   id aGroup val.x bGroup val.y
## 1  4      c 1.737      x 5.255
## 2  5      b 1.519      x 5.462
## 3  6      b 1.642      x 5.940
## 4  7      b 1.830      y 5.978
## 5  8      a 1.286      d 5.117
```

Note that there is an S3 function `merge.data.table` that will produce the same
output with the same syntax:



```r
all.equal( merge(data.frame(A), data.frame(B), by = 'id'),
    data.frame(merge(A, B, by = 'id') ))
```

```
## [1] TRUE
```


## Left/right outer join

An outer join takes all the rows of the *outer* table and reports these. This
is the default behavior for data.table. While this is often called an outer
join, the syntax is a bit odd at first different for `data.table`, but after
you understand the philosophy it becomes a bit more clear.

Outer joins in `data.table` work by considering the table `Y` in the expression
`X[Y]` as the "outer" table. That is, you will always include the rows from
table `Y`. The rows that don't match the key in `Y` will show up as `NA`,
hence, the syntax `nomatch = NA`.


```r
A[B,,nomatch = NA]
```

```
##     id aGroup   val bGroup val.1
##  1:  4      c 1.737      x 5.255
##  2:  5      b 1.519      x 5.462
##  3:  6      b 1.642      x 5.940
##  4:  7      b 1.830      y 5.978
##  5:  8      a 1.286      d 5.117
##  6: 21     NA    NA      w 5.458
##  7: 22     NA    NA      w 5.719
##  8: 23     NA    NA      w 5.935
##  9: 29     NA    NA      d 5.475
## 10: 30     NA    NA      d 5.560
```
Again, note that outer joins are default behavior, so `nomatch = NA` is redundant.

### Other packages

Using **dplyr**,


```r
left_join(B, A, by = 'id')
```

```
##    id bGroup val.x aGroup val.y
## 1   4      x 5.255      c 1.737
## 2   5      x 5.462      b 1.519
## 3   6      x 5.940      b 1.642
## 4   7      y 5.978      b 1.830
## 5   8      d 5.117      a 1.286
## 6  21      w 5.458   <NA>    NA
## 7  22      w 5.719   <NA>    NA
## 8  23      w 5.935   <NA>    NA
## 9  29      d 5.475   <NA>    NA
## 10 30      d 5.560   <NA>    NA
```
Using **base**,


```r
merge(data.frame(A), data.frame(B), by = 'id', all.y = T)
```

```
##    id aGroup val.x bGroup val.y
## 1   4      c 1.737      x 5.255
## 2   5      b 1.519      x 5.462
## 3   6      b 1.642      x 5.940
## 4   7      b 1.830      y 5.978
## 5   8      a 1.286      d 5.117
## 6  21   <NA>    NA      w 5.458
## 7  22   <NA>    NA      w 5.719
## 8  23   <NA>    NA      w 5.935
## 9  29   <NA>    NA      d 5.475
## 10 30   <NA>    NA      d 5.560
```

```r
merge(data.frame(B), data.frame(A), by = 'id', all.x = T)
```

```
##    id bGroup val.x aGroup val.y
## 1   4      x 5.255      c 1.737
## 2   5      x 5.462      b 1.519
## 3   6      x 5.940      b 1.642
## 4   7      y 5.978      b 1.830
## 5   8      d 5.117      a 1.286
## 6  21      w 5.458   <NA>    NA
## 7  22      w 5.719   <NA>    NA
## 8  23      w 5.935   <NA>    NA
## 9  29      d 5.475   <NA>    NA
## 10 30      d 5.560   <NA>    NA
```

# Chaining expressions


```r
A[B][val < 2.7,]
```

```
##    id aGroup   val bGroup val.1
## 1:  4      c 1.737      x 5.255
## 2:  5      b 1.519      x 5.462
## 3:  6      b 1.642      x 5.940
## 4:  7      b 1.830      y 5.978
## 5:  8      a 1.286      d 5.117
```
