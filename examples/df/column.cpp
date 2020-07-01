// Copyright (c) 2020 Tom Kwok
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/numeric/ublas/df/column.hpp>

int main () {
	using namespace boost::numeric::ublas::df;

	std::cout << std::boolalpha; // output booleans as strings

	// initialize from std::initializer_list

	column<int32_t> column_1({
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	});

	std::cout << column_1.at(0) << std::endl;
	std::cout << !column_1.at(0) << std::endl;
	std::cout << ~column_1.at(6) << std::endl;
	std::cout << column_1.at(0) + 10 << std::endl;
	std::cout << column_1.at(0) - 10 << std::endl;
	std::cout << column_1.at(0) * 2 << std::endl;
	std::cout << column_1.at(0) / 2 << std::endl;
	std::cout << column_1.at(0) * column_1.at(0) << std::endl;
	std::cout << std::nullopt * column_1.at(0) * column_1.at(0) << std::endl;
	std::cout << 2 * column_1.front() << std::endl;
	// std::cout << column_1[1] * 2 << std::endl;
	std::cout << column_1[0] << std::endl;
	std::cout << -column_1[1] << std::endl;
	std::cout << column_1[-1] << std::endl; // out of bounds
	std::cout << column_1[2] * column_1[2] << std::endl;
	column_1[0] = 100;
	column_1.reset(8);
	column_1[9] = std::nullopt;
	std::cout << "size=" << column_1.size() << std::endl;
	std::cout << "capacity=" << column_1.capacity() << std::endl;
	std::cout << "max_size=" << column_1.max_size() << std::endl;
	std::cout << "empty=" << column_1.empty() << std::endl;
	std::cout << column_1 << std::endl;
	// std::cout << column_1 + 1 << std::endl;

	// initialize from another column

	column<int32_t> column_2(column_1);
	column_2.reserve(20);
	column_2 = column_2;
	column_2 = +column_2;
	column_2 = -column_2;
	column_2 = 2 * column_2;
	column_2 = column_2 * -1;
	column_2 = column_2 * column_2;
	// std::cout << column_2.begin();
	std::cout << "size=" << column_2.size() << std::endl;
	std::cout << "capacity=" << column_2.capacity() << std::endl;
	std::cout << column_2 << std::endl;

	// initialize from std::vector

	std::vector<int> vector_3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	column<int> column_3(vector_3);
	column_3 = 1;
	column_3 = std::nullopt;
	column_3.shrink_to_fit();
	std::cout << "size=" << column_3.size() << std::endl;
	std::cout << "capacity=" << column_3.capacity() << std::endl;
	std::cout << column_3 << std::endl;

	return 0;
}
