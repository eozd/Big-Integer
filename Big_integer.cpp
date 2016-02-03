#include <algorithm>
#include <stdexcept>
#include <stack>
#include <deque>
#include <sstream>
#include "Big_integer.hpp"

Big_integer::container_type to_number(const std::string& number);

Big_integer::Big_integer()
	: m_digits()
{
}

Big_integer::Big_integer(const std::string& number)
	: m_digits()
{
	this->insert_front(number);
}

Big_integer::Big_integer(long long number)
	: m_digits()
{
	this->insert_front(number);
}

void Big_integer::insert_back(const std::string& number)
{
	auto digits = to_number(number);
	for (const auto i : digits) {
		this->m_digits.push_back(i);
	}
}

void Big_integer::insert_back(long long number)
{
	std::stack<int> s;
	do {
		s.push(number % 10);
		number /= 10;
	} while (number != 0);
	while (!s.empty()) {
		int i = s.top();
		s.pop();
		this->m_digits.push_back(i);
	}
}

void Big_integer::insert_front(const std::string& number)
{
	auto digits = to_number(number);
	for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
		this->m_digits.push_front(*it);
	}
}

void Big_integer::insert_front(long long number)
{
	do {
		this->m_digits.push_front(number % 10);
		number /= 10;
	} while (number != 0);
}

int& Big_integer::operator[](int index)
{
	return this->m_digits[index];
}

const int& Big_integer::operator[](int index) const
{
	return this->m_digits[index];
}

int& Big_integer::at(int index)
{
	return this->m_digits.at(index);
}

const int& Big_integer::at(int index) const
{
	return this->m_digits.at(index);
}

Big_integer Big_integer::subint(size_t index, size_t length) const
{
	if (index == this->size())
		return Big_integer();
	else if (index > this->size())
		throw std::out_of_range("Big_integer::subint: Given index is greater"
								" than size");
	Big_integer res;
	for (size_t i = 0; i < length; ++i) {
		if (index + i == this->size())
			break;
		res.insert_back(this->m_digits[index + i]);
	}
	return res;
}

Big_integer::iterator Big_integer::begin()
{
	return this->m_digits.begin();
}
Big_integer::const_iterator Big_integer::begin() const
{
	return this->m_digits.begin();
}

Big_integer::iterator Big_integer::end()
{
	return this->m_digits.end();
}
Big_integer::const_iterator Big_integer::end() const
{
	return this->m_digits.end();
}

size_t Big_integer::size() const
{
	return this->m_digits.size();
}

Big_integer::iterator Big_integer::erase(const_iterator pos)
{
	return this->m_digits.erase(pos);
}

Big_integer::iterator Big_integer::erase(const_iterator first,
										 const_iterator last)
{
	return this->m_digits.erase(first, last);
}

const Big_integer operator+(const Big_integer& a, const Big_integer& b)
{
	Big_integer result;
	const auto& shorter = a.size() < b.size() ? a : b;
	const auto& longer = a.size() < b.size() ? b : a;
	int carry_out = 0;
	int size_diff = longer.size() - shorter.size();
	for (size_t i = longer.size(); i > 0; --i) {
		int long_index = i - 1;
		int short_index = long_index - size_diff;
		int sum = 0;
		if (short_index < 0) {
			sum = longer[long_index] + carry_out;
		} else {
			sum = shorter[short_index] + longer[long_index]
											+ carry_out;
		}
		carry_out = sum / 10;
		if (long_index != 0)
			result.insert_front(sum % 10);
		else {
			do {
				result.insert_front(sum % 10);
				sum /= 10;
			} while (sum != 0);
		}
	}
	result.rm_leading_zeroes();
	return result;
}

const Big_integer operator-(const Big_integer& a, const Big_integer& b)
{
	if (a == b)
		return Big_integer();
	/**
	  * We are going to assume that a > b
	  * a < b case needs negative number implementation
	  * TODO: Implement negative Big_integers
	  */
	Big_integer result;
	auto a_copy = a;
	size_t size_diff = a_copy.size() - b.size();
	for (size_t i = a_copy.size(); i > 0; --i) {
		int long_index = i - 1;
		int short_index = long_index - size_diff;
		int diff = 0;
		if (short_index >= 0) {
			if (a_copy[long_index] >= b[short_index])
				diff = a_copy[long_index] - b[short_index];
			else {
				//ask for carry until you can get it
				int iter = long_index - 1; //left of long_index
				while (a_copy[iter] == 0)
					--iter; //get to the first nonzero digit
				--a_copy[iter];
				++iter;
				while (iter != long_index) {
					a_copy[iter] += 9;
					++iter;
				}
				diff = a_copy[long_index] + 10 - b[short_index];
			} //diff is ready
		} else {
			diff = a_copy[long_index];
		}
		result.insert_front(diff);
	}
	result.rm_leading_zeroes();
	return result;
}

const Big_integer operator*(const Big_integer& a, const Big_integer& b)
{
	Big_integer result;
	const auto& shorter = a.size() < b.size() ? a : b;
	const auto& longer = a.size() < b.size() ? b : a;
	for (size_t i = shorter.size(); i > 0; --i) {
		int short_index = i - 1;
		Big_integer inter_result;
		int carry_out = 0;
		for (size_t j = longer.size(); j > 0; --j) {
			int long_index = j - 1;
			int product = shorter[short_index] * longer[long_index]
						+ carry_out;
			carry_out = product / 10;
			if (long_index != 0)
				inter_result.insert_front(product % 10);
			else {
				do {
					inter_result.insert_front(product % 10);
					product /= 10;
				} while (product != 0);
			}
		}
		for (size_t k = shorter.size() - i; k > 0; --k) {
			inter_result.insert_back(0);
		}
		result += inter_result;
	}
	result.rm_leading_zeroes();
	return result;
}

const Big_integer operator/(const Big_integer& a, const Big_integer& b)
{
	if (b == Big_integer() || b == Big_integer{0})
		throw std::invalid_argument("Division by 0 is prohibited");
	Big_integer result;
	Big_integer	dividend = a.subint(0, b.size());
	for (size_t iter = b.size(); iter <= a.size(); ++iter) {
		if (dividend < b) {
			if (iter == a.size())
				break;
			dividend.insert_back(a[iter]);
			dividend.rm_leading_zeroes();
			if (dividend < b) {
				result.insert_back(0);
			}
		} else {
			Big_integer partial_product{1};
			int quotient = 0;
			do {
				++quotient;
				partial_product = b * Big_integer{quotient};
			} while (dividend - partial_product >= b);
			result.insert_back(quotient);
			dividend -= partial_product;
			--iter;
		}
	}
	result.rm_leading_zeroes();
	return result;
}

void operator+=(Big_integer& a, const Big_integer& b)
{
	a = a + b;
}

void operator-=(Big_integer& a, const Big_integer& b)
{
	a = a - b;
}

void operator*=(Big_integer& a, const Big_integer& b)
{
	a = a * b;
}

void operator/=(Big_integer& a, const Big_integer& b)
{
	a = a / b;
}

bool operator==(const Big_integer& a, const Big_integer& b)
{
	if (a.size() != b.size())
		return false;
	for (size_t i = 0; i < a.size(); ++i) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}

bool operator!=(const Big_integer& a, const Big_integer& b)
{
	return !(a == b);
}

bool operator<(const Big_integer& a, const Big_integer& b)
{
	if (a.size() < b.size())
		return true;
	else if (b.size() < a.size())
		return false;
	for (size_t i = 0; i < a.size(); ++i) {
		if (a[i] < b[i])
			return true;
		else if (a[i] > b[i])
			return false;
	}
	return false;
}

bool operator<=(const Big_integer& a, const Big_integer& b)
{
	return (a < b) || (a == b);
}

bool operator>(const Big_integer& a, const Big_integer& b)
{
	return !(a <= b);
}

bool operator>=(const Big_integer& a, const Big_integer& b)
{
	return !(a < b);
}

std::ostream& operator<<(std::ostream& os, Big_integer& b)
{
	for (size_t i = 0; i < b.size(); ++i) {
		os << b[i];
	}
	return os;
}

void Big_integer::rm_leading_zeroes()
{
	auto iter = std::find_if(this->m_digits.begin(), this->m_digits.end(),
			[] (const int a) { return a != 0; });
	if (iter == this->m_digits.end())
		this->m_digits.erase(this->m_digits.begin(), iter - 1);
	else
		this->m_digits.erase(this->m_digits.begin(), iter);
}

Big_integer::container_type to_number(const std::string& number)
{
	std::stringstream ss(number);
	Big_integer::container_type result;
	char c;
	while (ss >> c) {
		if (c < '0' || c > '9')
			throw std::invalid_argument("String must contain only digits");
		int i = c - '0'; //char to digit conversion. Check ASCII table
		result.push_back(i);
	}
	return result;
}
