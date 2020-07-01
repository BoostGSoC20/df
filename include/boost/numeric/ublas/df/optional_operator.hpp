// Copyright (c) 2020 Tom Kwok
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file column.hpp Definition for operator overloading on std::optional

#ifndef BOOST_UBLAS_DF_OPTIONAL_OPERATOR_HPP
#define BOOST_UBLAS_DF_OPTIONAL_OPERATOR_HPP

#include <optional>
#include <functional>

namespace boost {
namespace numeric {
namespace ublas {
namespace df {


// Operator overloading function for output stream
//  Implemented operators: <<
//  If variable has value, output value, otherwise output NULLOPT

const std::string NULLOPT = "Null";

template<typename T>
std::ostream& operator<<(std::ostream &out, const std::optional<T> &rhs) {
	if (rhs.has_value()) {
		out << rhs.value();
	} else {
		out << NULLOPT;
	}
	return out;
}

// Helper function for unary operators
//  Cases for templated typename T:
//   (1) `op` std::optional<T> 

template<typename TIn, typename TOut=TIn>
inline std::optional<TOut> operator_(const std::optional<TIn> &rhs, std::function<TOut (TIn)> func) {
	if (rhs.has_value()) {
		return func(rhs.value());
	} else {
		return std::nullopt;
	}
}

// Helper function for binary operators
//  Cases for templated typename T:
//   (1) T `op` std::optional<T>
//   (2) std::optional<T> `op` T
//   (3) std::optional<T> `op` std::optional<T>

template<typename T>
inline std::optional<T> operator_(const T &lhs, const std::optional<T> &rhs, std::function<T (T, T)> func) {
	if (rhs.has_value()) {
		return func(lhs, rhs.value());
	} else {
		return std::nullopt;
	}
}

template<typename T>
inline std::optional<T> operator_(const std::optional<T> &lhs, const T &rhs, std::function<T (T, T)> func) {
	if (lhs.has_value()) {
		return func(lhs.value(), rhs);
	} else {
		return std::nullopt;
	}
}

template<typename T>
inline std::optional<T> operator_(const std::optional<T> &lhs, const std::optional<T> &rhs, std::function<T (T, T)> func) {
	if (lhs.has_value() && rhs.has_value()) {
		return func(lhs.value(), rhs.value());
	} else {
		return std::nullopt;
	}
}

// Operator overloading function for unary operators
//  Implemented operators: + - ! ~

template<typename T>
std::optional<T> operator+(const std::optional<T> &rhs) {
	return operator_<T, T>(rhs, [](T x) { return +x; });
}

template<typename T>
std::optional<T> operator-(const std::optional<T> &rhs) {
	return operator_<T, T>(rhs, std::negate<T>());
}

template<typename T>
std::optional<T> operator~(const std::optional<T> &rhs) {
	return operator_<T, T>(rhs, std::bit_not<T>());
}

template<typename T>
std::optional<bool> operator!(const std::optional<T> &rhs) {
	return operator_<T, bool>(rhs, std::logical_not<T>());
}

// Operator overloading function for binary operators
//  Implemented operators:
//    Arithmetic: + - * / %

//  Cases for templated typename T:
//   (1) T `op` std::optional<T>
//   (2) std::optional<T> `op` T
//   (3) std::optional<T> `op` std::optional<T>
//   (4) std::optional<T> `op` std::nullopt_t
//   (5) std::nullopt_t `op` std::optional<T>

// operator+

template<typename T>
std::optional<T> operator+(const T &lhs, const std::optional<T> &rhs) {
	return operator_<T>(lhs, rhs, std::plus<T>());
}

template<typename T>
std::optional<T> operator+(const std::optional<T> &lhs, const T &rhs) {
	return operator_<T>(lhs, rhs, std::plus<T>());
}

template<typename T>
std::optional<T> operator+(const std::optional<T> &lhs, const std::optional<T> &rhs) {
	return operator_<T>(lhs, rhs, std::plus<T>());
}

template<typename T>
std::optional<T> operator+(const std::optional<T> &, const std::nullopt_t &) {
	return std::nullopt;
}

template<typename T> 
std::optional<T> operator+(const std::nullopt_t &, const std::optional<T> &) {
	return std::nullopt;
}

// operator-

template<typename T>
std::optional<T> operator-(const T &lhs, const std::optional<T> &rhs) {
	return operator_<T>(lhs, rhs, std::minus<T>());
}

template<typename T>
std::optional<T> operator-(const std::optional<T> &lhs, const T &rhs) {
	return operator_<T>(lhs, rhs, std::minus<T>());
}

template<typename T>
std::optional<T> operator-(const std::optional<T> &lhs, const std::optional<T> &rhs) {
	return operator_<T>(lhs, rhs, std::minus<T>());
}

template<typename T>
std::optional<T> operator-(const std::optional<T> &, const std::nullopt_t &) {
	return std::nullopt;
}

template<typename T> 
std::optional<T> operator-(const std::nullopt_t &, const std::optional<T> &) {
	return std::nullopt;
}

// operator*

template<typename T>
std::optional<T> operator*(const T &lhs, const std::optional<T> &rhs) {
	return operator_<T>(lhs, rhs, std::multiplies<T>());
}

template<typename T>
std::optional<T> operator*(const std::optional<T> &lhs, const T &rhs) {
	return operator_<T>(lhs, rhs, std::multiplies<T>());
}

template<typename T>
std::optional<T> operator*(const std::optional<T> &lhs, const std::optional<T> &rhs) {
	return operator_<T>(lhs, rhs, std::multiplies<T>());
}

template<typename T>
std::optional<T> operator*(const std::optional<T> &, const std::nullopt_t &) {
	return std::nullopt;
}

template<typename T> 
std::optional<T> operator*(const std::nullopt_t &, const std::optional<T> &) {
	return std::nullopt;
}

// operator/

template<typename T>
std::optional<T> operator/(const T &lhs, const std::optional<T> &rhs) {
	return operator_<T>(lhs, rhs, std::divides<T>());
}

template<typename T>
std::optional<T> operator/(const std::optional<T> &lhs, const T &rhs) {
	return operator_<T>(lhs, rhs, std::divides<T>());
}

template<typename T>
std::optional<T> operator/(const std::optional<T> &lhs, const std::optional<T> &rhs) {
	return operator_<T>(lhs, rhs, std::divides<T>());
}

template<typename T>
std::optional<T> operator/(const std::optional<T> &, const std::nullopt_t &) {
	return std::nullopt;
}

template<typename T> 
std::optional<T> operator/(const std::nullopt_t &, const std::optional<T> &) {
	return std::nullopt;
}

// operator%

template<typename T>
std::optional<T> operator%(const T &lhs, const std::optional<T> &rhs) {
	return operator_<T>(lhs, rhs, std::modulus<T>());
}

template<typename T>
std::optional<T> operator%(const std::optional<T> &lhs, const T &rhs) {
	return operator_<T>(lhs, rhs, std::modulus<T>());
}

template<typename T>
std::optional<T> operator%(const std::optional<T> &lhs, const std::optional<T> &rhs) {
	return operator_<T>(lhs, rhs, std::modulus<T>());
}

template<typename T>
std::optional<T> operator%(const std::optional<T> &, const std::nullopt_t &) {
	return std::nullopt;
}

template<typename T> 
std::optional<T> operator%(const std::nullopt_t &, const std::optional<T> &) {
	return std::nullopt;
}




}
}
}
}

#endif
