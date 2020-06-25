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

template<typename TElement>
class column_ref;

template<typename TElement>
class column {
	using element_type = TElement;
	using optional_type = std::optional<element_type>;

private:
	std::vector<optional_type> content_;

	inline bool test_index_(size_t index) const {
		// type size_t cannot be negative
		return index < this->content_.size();
	}

	template <typename T=std::function<optional_type (optional_type)> >
	inline auto operator_(T func) const {
		std::vector<optional_type> content;
		for (auto element: this->content_) {
			content.push_back(func(element));
		}
		return column<element_type>(content);
	}
public:
	column() { }

	template <typename T=const std::initializer_list<element_type> >
	column(T list) {
		for (auto element: list) {
			this->content_.emplace_back(optional_type(element));
		}
	}

	~column() {}

	auto operator=(element_type value) {
		std::fill(this->content_.begin(), this->content_.end(), 
			optional_type(value));
		return *this;
	}

	auto operator=(std::nullopt_t value) {
		std::fill(this->content_.begin(), this->content_.end(), 
			value);
		return *this;
	}

	column_ref<element_type> operator[](size_t index) {
		return column_ref<element_type>(*this, index);
	}

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

	auto operator+() const {
		return this->operator_([](auto x) { return +x; });
	}

	auto operator-() const {
		return this->operator_([](auto x) { return -x; });
	}

	auto operator!() const {
		return this->operator_([](auto x) { return !x; });
	}

	auto operator~() const {
		return this->operator_([](auto x) { return ~x; });
	}

	template<typename TInsertion>
	friend std::ostream& operator<<(std::ostream &out, const column<TInsertion> &col);
};

template<typename TInsertion>
std::ostream& operator<<(std::ostream &out, const column<TInsertion> &col) {
	for (auto element: col.content_) {
		out << element << std::endl;
	}
	return out;
}

template<typename TElement>
class column_ref {
	using element_type = TElement;
	using optional_type = std::optional<element_type>;

private:
	column<element_type> &column_;
	size_t index_;

	template <typename T=std::function<optional_type (optional_type)> >
	inline auto operator_(T func) const {
		return func(this->column_.at(this->index_));
	}
public:
	template <typename T=column<element_type> >
	column_ref(T &col, size_t index) : column_(col), index_(index) { }

	~column_ref() {}

	auto has_value() const {
		return this->column_.at(this->index_).has_value();
	}

	auto value() const {
		return this->column_.at(this->index_).value();
	}

	auto operator=(element_type value) {
		this->column_.set(this->index_, value);
		return *this;
	}

	auto operator=(std::nullopt_t) {
		this->column_.reset(this->index_);
		return *this;
	}

	auto operator+() const {
		return this->operator_([](auto x) { return +x; });
	}

	auto operator-() const {
		return this->operator_([](auto x) { return -x; });
	}

	auto operator!() const {
		return this->operator_([](auto x) { return !x; });
	}

	auto operator~() const {
		return this->operator_([](auto x) { return ~x; });
	}

	template<typename TInsertion>
	friend std::ostream& operator<<(std::ostream &out, const column_ref<TInsertion> &ref);
};

template<typename TInsertion>
std::ostream& operator<<(std::ostream &out, const column_ref<TInsertion> &ref) {
	out << ref.column_.at(ref.index_);
	return out;
}


}
}
}
}

#endif
