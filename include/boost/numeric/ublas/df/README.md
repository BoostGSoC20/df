# Boost.uBLAS Data Frame (df) Refactored

Boost uBLAS data frame is a header-only library implementation of data frames, a two-dimensional matrix-like data structure, inspired by data frames in Python Pandas and R.

This project is authored by Tom Kwok and funded by Google Summer of Code 2020.

## Implementation

The source code of the implementation of data frame is placed in the `include/boost/numeric/ublas/df/` directory, structured as follows:
* `column.hpp` defines a column, similar to Series in Pandas.
* `frame.hpp` defines a frame, similar to DataFrame in Pandas.
* `column_algorithm.hpp` defines algorithms on a column, including transform, statistical aggregate operations and string filter operations.
* `frame_algorithm.hpp` defines algorithms on a frame, including frame merging and wrapper operations for applying algorithms on a column on the constituent columns of a frame.

The implementation is defined in the `boost::numeric::ublas::df` namespace. 

A column is stored as a `std::vector` of `std::optional` elements of templated type `element_type`.

Every element is contained in a `std::optional` class. Null type is indicated by `std::nullopt`, and is printed `Null` (similar to `None` in Python).

No separate data structure is defined for a view of a frame or a column for simplicity. A filtering or merging operation produces a new frame or column data structure. Nonetheless, a frame or a column is mutable using append, drop and element access operations. 

A library from modern C++ libraries (C++11/14/17/20) is preferred over older implementations in Boost libraries for when such a choice is available. For example, `std::optional` is used instead of `boost::optional`.

The implementation requires support for C++17 or newer to compile due to the use of `std::optional` class template since C++17.

## Examples

Examples of usage of data frame is placed in the `examples/df/` directory.

## Road-map

* Design of data frame data structures in README documentation
* C++17 implementation of `column` class
* C++17 implementation of `frame` class
* Expression template implementation of column and frame (which enables lazy evaluation with broadcasting)

