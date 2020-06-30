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
		// type size_t cannot be negative, no need to test
		return index < this->content_.size();
	}

	// Helper function for unary operations

	template <typename TOut=element_type>
	inline auto operator_(std::function<std::optional<TOut> (optional_type)> func) const {
		std::vector<std::optional<TOut> > content;
		for (const auto& element: this->content_) {
			content.push_back(func(element));
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
		for (const auto& element: list) {
			this->content_.emplace_back(optional_type(element));
		}
	}

	// Destructor
	//  Note destructor destructs member variables

	~column() {}

	// Operator overloading function for broadcasted assignment

	auto operator=(element_type value) {
		std::fill(this->content_.begin(), this->content_.end(), 
			optional_type(value));
		return *this; // reference, not copy
	}

	auto operator=(std::nullopt_t value) {
		std::fill(this->content_.begin(), this->content_.end(), 
			value);
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

// Operator overloading function for binary operators


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

	// Helper function for unary operations

	template <typename TOut=element_type>
	inline auto operator_(std::function<std::optional<TOut> (optional_type)> func) const {
		return func(this->column_.at(this->index_));
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
