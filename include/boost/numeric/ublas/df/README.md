# Boost.uBLAS Data Frame (df) Rewrite

Boost uBLAS data frame is a header-only library implementation of data frames, a two-dimensional matrix-like data structure, inspired by data frames in Python Pandas and R.

This project is authored by Tom Kwok and funded by Google Summer of Code 2020.

## Implementation outline

The source code of the implementation of data frame is placed in the `include/boost/numeric/ublas/df/` directory, structured as follows:
* `optional_operator.hpp` defines operator overloading functions on `std::optional`.
* `column.hpp` defines a column, similar to Series in Pandas or data frame column in R.
* `frame.hpp` defines a frame, similar to DataFrame in Pandas or data frame in R.
* `column_algorithm.hpp` defines algorithms on a column, including transform, statistical aggregate operations and string filter operations.
* `frame_algorithm.hpp` defines algorithms on a frame, including frame merging and wrapper operations for applying algorithms on a column on the constituent columns of a frame.

## Column and frame ontainer

A column class `column` and a frame class `frame` are defined in the `boost::numeric::ublas::df` namespace.

A column `column<element_type>` as a homogeneous container manages a homogeneous `std::vector<element_type>` of `std::optional` elements of templated type `element_type`. (This is a different design compared with a [2019 GSoC implementation](https://github.com/BoostGSoC19/data_frame), in which the column class `data_frame_col` as a heterogeneous container manages a homogeneous `boost::numeric::ublas::vector` of elements.)

## Element type

Note that `element_type` is a user-provided type, which could be any POD type (e.g. `int`, `int64_t`, `double`), any modern C++ class (e.g. `enum class`, `std::variant`, `std::optional`), or any user-defined class.

While it could be redundant to have, for example, `std::optional<T>` as the `element_type` since `optional_type` would be `std::optional<std::optional<T> >`, this possibility is not ruled out for the implementation and should be handled correctly.

## Operations

Mutating operations on a `column` include the following: 
* Class methods (wrapper of `std::vector` class methods): 
    * For element access: `at()` `front()` `back()`
    * For capacity: `empty()` `size()` `max_size()` `capacity()`
* Helper functions for operations on indexed element: `set()` `reset()`

Non-mutating operations on a `column` include the following: 
* Class methods (wrapper of `std::vector` class methods): 
    * For capacity: `reserve()` `shrink_to_fit()` 
    * For modifying: `clear()`
* Operator overloading for unary or binary operations (which return a new instance of `column`):
    * Unary operations: 
        * Arithmetic: `+` `-`
        * Logical: `!`
        * Bitwise: `~`
    * Binary operations:
        * Arithmetic: `+` `-` `*` `/` `%`
        * Comparison: `==` `!=` `>` `<` `>=` `<=`
        * Logical: `&&` `||`
        * Bitwise: `&` `|` `^`

## View

A data structure can be defined for a view of a frame or a column. (To review)

## Null value and type

Every element is contained in a `std::optional<element_type>` class, short-handed `optional_type`, which manages an optional value of type `element_type`. It can be used as if having either a value of type `element_type` or a null value `std::nullopt`. Null is effectively considered as a "first-class citizen".

A null value `std::nullopt` is printed `Null` (similar to `None` object in Python or `NULL` object in R).

As a general rule, any operation on a null value and a non-null value (in either order) would return a null value. Exceptions are comparison operators `==` and `!=` on a null value and a non-null value (in either order), for which a Boolean value is returned.

Since missing data is common in work-flows with data frames, it is worth the extra effort in carefully designing data structures to handle null values. This can facilitate data imputation, which is to replace null values with substituted values inferred from non-null values. Features such as value fill `fillna()`, forward fill `ffill()` and backward fill `bfill()` functions to be implemented in algorithms.

## C++ version requirement

The implementation requires support for C++17 or newer to compile due to the use of `std::optional` class template defined in C++ standard libraries since C++17.

The implementation requires support for C++14 or newer to compile due to the use of initializer lists `std::initializer_list` class template defined in C++ standard libraries since C++14.

## Programming choices

A library from modern C++ libraries (C++11/14/17/20) is preferred over older implementations in Boost libraries for when such a choice is available. For example, `std::optional` is used instead of `boost::optional`.

Also, note in particular the following programming choices made:

* Type name for data structures: `column_t` `frame_t` or `column` `frame` (picked)
* String for null type: `"Empty"` or `"Null"` (picked)
* Templated type short-hand: `base_type` or `element_type` (picked)
* Naming convention for private class variables: `_column` or `column_` (picked)
* Programming style of loops: `std::for_each()` vs `for (const auto&)` loop (picked) 
* Name of function for appending a column or frame to another: `extend()` or `append()` (picked)

## Examples

Examples of usage of data frame is placed in the [`examples/df/`](../../../../../examples/df/) directory.

## To-do: Boost-ification, documentation, benchmarks

* convert `Makefile` in `examples/df/` to a `Jamfile` to use Jam make system
* Sphinx documentation (code comments `\brief` and `\tparam`) with doxygen in `doc/df/`
* write benchmarks for data frame in `benchmarks/df/`

## Road-map

* Design of data structures for data frames in README.md documentation
* C++17 implementation of `column` class
* C++17 implementation of `frame` class
* Expression template implementation of operations on column and frame for lazy evaluation
