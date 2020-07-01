// Copyright (c) 2020 Tom Kwok
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file column.hpp Definition for the column class for data frame

#ifndef BOOST_UBLAS_DF_COLUMN_HPP
#define BOOST_UBLAS_DF_COLUMN_HPP

#include <initializer_list>
#include <vector>
#include <optional>
#include <functional>
#include <algorithm>

#include <iostream>

#include "optional_operator.hpp"

namespace boost {
namespace numeric {
namespace ublas {
namespace df {

// Column reference declaration for use in definition of column class

template<typename TElement>
class column_ref;

// Column class

template<typename TElement>
class column {
	using element_type = TElement;
	using optional_type = std::optional<element_type>;

private:
	// Store of column content

	std::vector<optional_type> content_;

	// Helper function for index bounds checking

	inline bool test_index_(size_t index) const {
		// note type size_t cannot be negative, no need to test
		return index < this->content_.size();
	}

	// Helper function for unary operations on one column and binary operations with one column only

	template <typename TOut=element_type>
	inline auto operator_(std::function<std::optional<TOut> (optional_type)> func) const {
		std::vector<std::optional<TOut> > content;
		content.reserve(this->size());
		for (const auto& element: this->content_) {
			content.push_back(func(element));
		}
		return column<TOut>(content);
	}

	// Helper function for binary operations with two columns
	//  Note the returned column has the same size as that of the larger column

	template <typename TOut=element_type>
	inline auto operator_(std::function<std::optional<TOut> (optional_type, optional_type)> func, const column<element_type> &rhs) const {
		size_t size = std::max(this->size(), rhs.size());
		std::vector<std::optional<TOut> > content;
		content.reserve(size);
		for (int i = 0; i < size; ++i) {
			content.push_back(func(this->at(i), rhs.at(i)));
		}
		return column<TOut>(content);
	}

public:
	// Constructor
	//  Note constructor takes as parameter a std::initializer_list 
	//  or any data structure on which a range-based for loop can operate 

	column() { }

	template <typename T=const std::initializer_list<element_type> >
	column(T list) {
		// reserve memory to avoid unnecessary allocation and deallocation
		this->content_.reserve(list.size());
		for (const auto& element: list) {
			this->content_.emplace_back(optional_type(element));
		}
	}

	// Destructor
	//  Note destructor destructs member variables

	~column() {}

	// Operator overloading function for broadcasted assignment

	auto operator=(element_type value) {
		std::fill(this->content_.begin(), this->content_.end(), optional_type(value));
		return *this; // reference, not copy
	}

	auto operator=(std::nullopt_t) {
		std::fill(this->content_.begin(), this->content_.end(), std::nullopt);
		return *this; // reference, not copy
	}

	// Operator overloading function for subscript referencing

	column_ref<element_type> operator[](size_t index) {
		return column_ref<element_type>(*this, index);
	}

	// Class methods (wrapper of std::vector class methods)
	//  Note `emplace_back()` (as for `std::vector`) is not implemented 
	//   since structure supplied is to be wrapped in `std::optional`
	//  Note `get_allocator()` and `swap()` are not implemented

	auto at(size_t index) const {
		if (this->test_index_(index)) {
			return this->content_[index];
		} else {
			// returns std::nullopt for out of bounds access; does not throw an exception
			return optional_type(std::nullopt);
		}
	}

	auto front() const {
		return this->content_.front();
	}

	auto back() const {
		return this->content_.back();
	}

	bool empty() const {
		return this->content_.empty();
	}

	size_t size() const {
		return this->content_.size();
	}

	size_t max_size() const {
		return this->content_.max_size();
	}

	size_t capacity() const {
		return this->content_.capacity();
	}

	void reserve(size_t new_cap) {
		this->content_.reserve(new_cap);
	}

	void shrink_to_fit() {
		this->content_.shrink_to_fit();
	}

	void clear() {
		this->content_.clear();
	}

	// Helper functions for operations on indexed element

	bool set(size_t index, element_type value) {
		bool test = this->test_index_(index);
		if (test) {
			this->content_[index] = optional_type(value);
		}
		return test;
	}

	bool set(size_t index, std::nullopt_t) {
		return this->reset(index);
	}

	bool reset(size_t index) {
		bool test = this->test_index_(index);
		if (test) {
			this->content_[index].reset();
		}
		return test;
	}

	// Operator overloading function for unary operators
	//  Implemented operators: + - ! ~

	auto operator+() const {
		return this->operator_<element_type>([](auto x) { return +x; });
	}

	auto operator-() const {
		return this->operator_<element_type>([](auto x) { return -x; });
	}

	auto operator~() const {
		return this->operator_<element_type>([](auto x) { return ~x; });
	}

	auto operator!() const {
		return this->operator_<bool>([](auto x) { return !x; });
	}

	// Operator overloading function for binary operators
	//  Implemented operators:
	//    Arithmetic: + - * / %

	//  Cases for templated typename T:
	//   (1) column<T> `op` column<T>
	//   (2) column<T> `op` T
	//   (3) column<T> `op` std::optional<T>
	//   (4) T `op` column<T>
	//   (5) std::optional<T> `op` column<T>
	//   (6) column<T> `op` std::nullopt_t
	//   (7) std::nullopt_t `op` column<T>

	// operator*

	template<typename T>
	friend auto operator*(const column<T> &self, const column<T> &rhs) {
		return self.template operator_<T>([](auto x, auto y) { return x * y; }, rhs);
	}

	template<typename T>
	friend auto operator*(const column<T> &self, const T &value) {
		return self.template operator_<T>([value](auto x) { return x * value; });
	}

	template<typename T>
	friend auto operator*(const column<T> &self, const std::optional<T> &value) {
		return self.template operator_<T>([value](auto x) { return x * value; });
	}

	template<typename T>
	friend auto operator*(const T &value, const column<T> &self) {
		return self.template operator_<T>([value](auto x) { return value * x; });
	}

	template<typename T>
	friend auto operator*(const std::optional<T> &value, const column<T> &self) {
		return self.template operator_<T>([value](auto x) { return value * x; });
	}

	template<typename T>
	friend auto operator*(const column<T> &self, const std::nullopt_t &) {
		return self.template operator_<T>([](auto) { return std::nullopt; });
	}

	template<typename T>
	friend auto operator*(const std::nullopt_t &, const column<T> &self) {
		return self.template operator_<T>([](auto) { return std::nullopt; });
	}

	// Friend declaration for output stream operator to access content_

	template<typename T>
	friend std::ostream& operator<<(std::ostream &out, const column<T> &col);
};

// Operator overloading function for output stream
//  Implemented operators: <<

template<typename T>
std::ostream& operator<<(std::ostream &out, const column<T> &col) {
	for (const auto& element: col.content_) {
		out << element << std::endl;
	}
	return out;
}


// Column reference class
//  An intermediate class containing a column reference to (col) and an index (i) to provide 
//   (1) assignment operation short-hand (example: col[i] = value;)
//   (2) other operation short-hand (example: auto sum = col[i] + col[i+1];)

template<typename TElement>
class column_ref {
	using element_type = TElement;
	using optional_type = std::optional<element_type>;

private:
	// Store of column reference and index

	column<element_type> &column_;
	size_t index_;

	// Helper function for unary operations on one column reference and binary operations with one column reference only

	template <typename TOut=element_type>
	inline auto operator_(std::function<std::optional<TOut> (optional_type)> func) const {
		return func(this->column_.at(this->index_));
	}

	// Helper function for binary operations with two column references

	template <typename TOut=element_type>
	inline auto operator_(std::function<std::optional<TOut> (optional_type, optional_type)> func, const column_ref<element_type> &rhs) const {
		return func(this->column_.at(this->index_), rhs.column_.at(rhs.index_));
	}
public:
	// Constructor

	column_ref(column<element_type> &col, size_t index) : column_(col), index_(index) { }

	// Destructor
	//  Note destructor destructs member variables

	~column_ref() {}

	// Class methods (wrapper of std::optional class methods)

	auto has_value() const {
		return this->column_.at(this->index_).has_value();
	}

	auto value() const {
		return this->column_.at(this->index_).value();
	}

	// Operator overloading function for assignment

	auto operator=(element_type value) {
		this->column_.set(this->index_, value);
		return *this;
	}

	auto operator=(std::nullopt_t) {
		this->column_.reset(this->index_);
		return *this;
	}

	// Operator overloading function for unary operators
	//  Implemented operators: + - ! ~

	auto operator+() const {
		return this->operator_<element_type>([](auto x) { return +x; });
	}

	auto operator-() const {
		return this->operator_<element_type>([](auto x) { return -x; });
	}

	auto operator~() const {
		return this->operator_<element_type>([](auto x) { return ~x; });
	}

	auto operator!() const {
		return this->operator_<bool>([](auto x) { return !x; });
	}

	// Operator overloading function for binary operators
	//  Implemented operators: 

	//  Cases for templated typename T:
	//   (1) column_ref<T> `op` column_ref<T>
	//   (2) column_ref<T> `op` T
	//   (3) column_ref<T> `op` std::optional<T>
	//   (4) column_ref<T> `op` std::nullopt_t
	//   (5) T `op` column_ref<T>
	//   (6) std::optional<T> `op` column_ref<T>
	//   (7) std::nullopt_t `op` column_ref<T>

	// operator*

	template<typename T>
	friend auto operator*(const column_ref<T> &self, const column_ref<T> &rhs) {
		return self.template operator_<T>([](auto x, auto y) { return x * y; }, rhs);
	}

	template<typename T>
	friend auto operator*(const column_ref<T> &self, const T &value) {
		return self.template operator_<T>([value](auto x) { return x * value; });
	}

	template<typename T>
	friend auto operator*(const column_ref<T> &self, const std::optional<T> &value) {
		return self.template operator_<T>([value](auto x) { return x * value; });
	}

	template<typename T>
	friend auto operator*(const T &value, const column_ref<T> &self) {
		return self.template operator_<T>([value](auto x) { return value * x; });
	}

	template<typename T>
	friend auto operator*(const std::optional<T> &value, const column_ref<T> &self) {
		return self.template operator_<T>([value](auto x) { return value * x; });
	}

	template<typename T>
	friend auto operator*(const column_ref<T> &self, const std::nullopt_t &) {
		return self.template operator_<T>([](auto) { return std::nullopt; });
	}

	template<typename T>
	friend auto operator*(const std::nullopt_t &, const column_ref<T> &self) {
		return self.template operator_<T>([](auto) { return std::nullopt; });
	}

	// Friend declaration for output stream operator to access content_

	template<typename T>
	friend std::ostream& operator<<(std::ostream &out, const column_ref<T> &ref);
};

// Operator overloading function for output stream
//  Implemented operators: <<

template<typename T>
std::ostream& operator<<(std::ostream &out, const column_ref<T> &ref) {
	out << ref.column_.at(ref.index_);
	return out;
}


}
}
}
}

#endif
