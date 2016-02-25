#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <memory>

using std::string;
using std::vector;
using std::stack;
using std::ostream;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

class symbol
{
public:
	symbol(const string &str) : _symbol(str) {}
	virtual ~symbol() {}

protected:
	string _symbol;

public:
	virtual string getter() const { return _symbol; };

protected:
	template<typename T> string to_str(const T& t) const;
	template<typename T> T from_str(const string& s) const;
};

class operand : public symbol
{
public:
	operand(const string &str) : symbol(str) {}
	~operand() {}

public:
	double get_value() const { return from_str<double>(_symbol); }
	string getter() const { return _symbol; };
};

class operator_ : public symbol
{
public:
	operator_(const string &str) : symbol(str) {}
	virtual ~operator_() {}

public:
	virtual operand calculate(const operand &left, const operand &right) const = 0;
};

class add_ : public operator_
{
public:
	add_() : operator_("+") {}
	~add_() {}

public:
	operand calculate(const operand &left, const operand &right) const;
	string getter() const { return "+"; };
};

class sub_ : public operator_
{
public:
	sub_() : operator_("-") {}
	~sub_() {}

public:
	operand calculate(const operand &left, const operand &right) const;
	string getter() const { return "-"; };
};

class mul_ : public operator_
{
public:
	mul_() : operator_("*") {}
	~mul_() {}

public:
	operand calculate(const operand &left, const operand &right) const;
	string getter() const { return "*"; };
};

class div_ : public operator_
{
public:
	div_() : operator_("/") {}
	~div_() {}

public:
	operand calculate(const operand &left, const operand &right) const;
	string getter() const { return "/"; };
};

class mod_ : public operator_
{
public:
	mod_() : operator_("%") {}
	~mod_() {}

public:
	operand calculate(const operand &left, const operand &right) const;
	string getter() const { return "%"; };
};

class pow_ : public operator_
{
public:
	pow_() : operator_("pow") {}
	~pow_() {}

public:
	operand calculate(const operand &left, const operand &right) const;
	string getter() const { return "pow"; };
};

class calculator
{
public:
	calculator() {}
	~calculator() { clear(); }

private:
	vector<shared_ptr<symbol>> _symbols;

private:
	void clear();
	int expression2symbol(const string &exp);

public:
	operand operator()(const string &expression);
	friend ostream& operator<<(ostream &out, const operand &sym);
};

#endif // __SYMBOL_H__