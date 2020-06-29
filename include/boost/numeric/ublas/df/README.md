# Boost.uBLAS Data Frame (df) Refactored

Boost uBLAS data frame is a header-only library implementation of data frames, a two-dimensional matrix-like data structure, inspired by data frames in Python Pandas and R.

This project is authored by Tom Kwok and funded by Google Summer of Code 2020.

## Implementation outline

The source code of the implementation of data frame is placed in the `include/boost/numeric/ublas/df/` directory, structured as follows:
* `column.hpp` defines a column, similar to Series in Pandas or data frame column in R.
* `frame.hpp` defines a frame, similar to DataFrame in Pandas or data frame in R.
* `column_algorithm.hpp` defines algorithms on a column, including transform, statistical aggregate operations and string filter operations.
* `frame_algorithm.hpp` defines algorithms on a frame, including frame merging and wrapper operations for applying algorithms on a column on the constituent columns of a frame.

The implementation is defined in the `boost::numeric::ublas::df` namespace. 

A column is stored as a `std::vector` of `std::optional` elements of templated type `element_type`.

No separate data structure is defined for a view of a frame or a column for simplicity. A filtering or merging operation produces a new frame or column data structure. Nonetheless, a frame or a column is mutable using the following operations: element access with `operator[]`.

## Null value and type

Every element is contained in a `std::optional<element_type>` class, short-handed `optional_type`, which manages an optional value of type `element_type`. It can be considered as if having either a value of type `element_type` or a null value `std::nullopt` of null type `std::nullopt_t`. Null is effectively considered as a "first-class citizen".

A null value `std::nullopt` is printed `Null` (similar to `None` object in Python or `NULL` object in R).

As a general rule, any binary operation on a null value and a non-null value (in either order) would return a null value.

Since missing data is common in workflows with data frames, it is worth the extra effort in carefully designing data structures to handle null values. This can facilitate data imputation, which is to replace null values with substituted values inferred from non-null values. Features such as forward or backward fill functions to be implemented in algorithms.

## C++ version requirement

The implementation requires support for C++17 or newer to compile due to the use of `std::optional` class template defined in C++ standard libraries since C++17.

The implementation requires support for C++14 or newer to compile due to the use of initializer lists `std::initializer_list` class template defined in C++ standard libraries since C++14.

## Examples

Examples of usage of data frame is placed in the [`examples/df/`](../../../../../examples/df/) directory.

## Road-map

* Design of data frame data structures in README documentation
* C++17 implementation of `column` class
* C++17 implementation of `frame` class
* Expression template implementation of column and frame (which enables lazy evaluation with broadcasting)

## Programming choices

A library from modern C++ libraries (C++11/14/17/20) is preferred over older implementations in Boost libraries for when such a choice is available. For example, `std::optional` is used instead of `boost::optional`.

Also, note in particular the following programming choices made:

* String for null type: `"Empty"` or `"Null"` (picked)
* Templated type short-hand: `base_type` or `element_type` (picked)
* Naming convention for private class variables: `_column` or `column_` (picked)
* Programming style of loops: `std::for_each` vs `for (auto ...)` loop (picked) 
* Name of function for appending a column or frame to another: `extend()` or `append()` (picked)
