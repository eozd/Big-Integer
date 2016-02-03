#pragma once
#include <string>
#include <deque>

class Big_integer {
public:
	typedef std::deque<int> container_type;
	typedef typename container_type::iterator iterator;
	typedef typename container_type::const_iterator const_iterator;
public:
	Big_integer();
	Big_integer(const std::string& number);
	Big_integer(long long number);
	Big_integer(const Big_integer&) = default;
	Big_integer(Big_integer&&) = default;
	Big_integer& operator=(const Big_integer&) = default;
	Big_integer& operator=(Big_integer&&) = default;
	~Big_integer() = default;

	/**
	  * Should be used if the number that is going to be inserted
	  * is greater than std::numeric_limits<long long>::max()
	  */
	void insert_back(const std::string& number);
	void insert_front(const std::string& number);

	/**
	  * If the number tha tis going to be inserted fits into the bounds
	  * of long long type, these methods should be used. 
	  * Inserting numbers this way gives better performance
	  */
	void insert_back(long long number);
	void insert_front(long long number);

	/**
	  * No bounds checking
	  */
	int& operator[](int index);
	const int& operator[](int index) const;

	/**
	  * Bounds checking.
	  */
	int& at(int index);
	const int& at(int index) const;

	/**
	  * Behaves as std::string::substr
	  */
	Big_integer subint(size_t index, size_t length) const;

	iterator begin();
	const_iterator begin() const;

	iterator end();
	const_iterator end() const;

	size_t size() const;

	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);

	/**
	  * 00742000 ==> 742000
	  */
	void rm_leading_zeroes();

private:
	container_type m_digits;
};

std::ostream& operator<<(std::ostream& os, Big_integer& b);

const Big_integer operator+(const Big_integer& a, const Big_integer& b);
const Big_integer operator-(const Big_integer& a, const Big_integer& b);
const Big_integer operator*(const Big_integer& a, const Big_integer& b);
const Big_integer operator/(const Big_integer& a, const Big_integer& b);

void operator+=(Big_integer& a, const Big_integer& b);
void operator-=(Big_integer& a, const Big_integer& b);
void operator*=(Big_integer& a, const Big_integer& b);
void operator/=(Big_integer& a, const Big_integer& b);

//equality operators
bool operator==(const Big_integer& a, const Big_integer& b);
bool operator!=(const Big_integer& a, const Big_integer& b);

//ordering operators
bool operator<(const Big_integer& a, const Big_integer& b);
bool operator<=(const Big_integer& a, const Big_integer& b);
bool operator>(const Big_integer& a, const Big_integer& b);
bool operator>=(const Big_integer& a, const Big_integer& b);
