#include "symbol.h"

template<typename T>
string symbol::to_str(const T& t) const
{
	std::ostringstream s;
	s << t;
	return s.str();
}

template<typename T>
T symbol::from_str(const std::string& s) const
{
	std::istringstream is(s);
	T t;
	is >> t;
	return t;
}

operand add_::calculate(const operand &left, const operand &right) const
{
	return operand(to_str<double>(left.get_value() + right.get_value()));
}

operand sub_::calculate(const operand &left, const operand &right) const
{
	return operand(to_str<double>(left.get_value() - right.get_value()));
}

operand mul_::calculate(const operand &left, const operand &right) const
{
	return operand(to_str<double>(left.get_value() * right.get_value()));
}

operand div_::calculate(const operand &left, const operand &right) const
{
	if (right.get_value() == 0)
		throw std::logic_error("Can not be divided by zero");
	return operand(to_str<double>(left.get_value() / right.get_value()));
}

operand mod_::calculate(const operand &left, const operand &right) const
{
	if (right.get_value() == 0)
		throw std::logic_error("Can not be modulod by zero");
	return operand(to_str<double>(fmod(left.get_value(), right.get_value())));
}

operand pow_::calculate(const operand &left, const operand &right) const
{
	return operand(to_str<double>(pow(left.get_value(), right.get_value())));
}

void calculator::clear()
{
	//auto it = _symbols.begin();
	//for (; it != _symbols.end(); ++it) {
	//	delete[] (*it);
	//}
	_symbols.clear();
}

/*
 * This is extra part which convert one line of expression to a vector of symbol
 */
int calculator::expression2symbol(const string &exp)
{
	int ret = 0;
	clear();

	string sym("");
	shared_ptr<symbol> psym = nullptr;
	int dots = 0;

	auto it = exp.begin();
	for (; it != exp.end(); ++it) {
		/* store operands */
		dots = 0;
		while (it != exp.end() && ((*it >= '0' && *it <= '9') || *it == '.')) {
			if (*it == '.') ++dots;
			sym.push_back(*(it++));
		}
		if (dots > 1) {
			ret = -1;
			break;
		}
		if (sym != "") {
			psym = dynamic_pointer_cast<symbol>(make_shared<operand>(operand(sym)));
			_symbols.push_back(psym);
			sym.clear();
		}
		if (it == exp.end()) break;

		/* store operator of power */
		while (it != exp.end() && (*it == 'p' || *it == 'o' || *it == 'w')) {
			sym.push_back(*(it++));
		}
		if (sym == "pow") {
			psym = dynamic_pointer_cast<symbol>(make_shared<pow_>(pow_()));
			_symbols.push_back(psym);
			sym.clear();
		}
		else if (sym != "") {
			ret = -2;
			break;
		}
		if (it == exp.end()) break;

		/* store other operators */
		switch (*it)
		{
		case '+': psym = dynamic_pointer_cast<symbol>(make_shared<add_>(add_())); break;
		case '-': psym = dynamic_pointer_cast<symbol>(make_shared<sub_>(sub_())); break;
		case '*': psym = dynamic_pointer_cast<symbol>(make_shared<mul_>(mul_())); break;
		case '/': psym = dynamic_pointer_cast<symbol>(make_shared<div_>(div_())); break;
		case '%': psym = dynamic_pointer_cast<symbol>(make_shared<mod_>(mod_())); break;
		default:  psym = nullptr; break;
		}

		if (psym != nullptr)
			_symbols.push_back(psym);
		else if (*it != ' ' && *it != '\t') {
			ret = -3;
			break;
		}
	}

	return ret;
}

operand calculator::operator()(const string &expression)
{
	if (0 != expression2symbol(expression))
		throw std::logic_error("wrong expression");

	stack<operand> operands;
	shared_ptr<operand> num;
	shared_ptr<operator_> ope;
	auto it = _symbols.begin();
	for (; it != _symbols.end(); ++it) {
		num = dynamic_pointer_cast<operand>(*it);
		if (num != nullptr) {
			operands.push(*num);
		}
		else {
			ope = dynamic_pointer_cast<operator_>(*it);

			if (operands.empty())
				throw std::logic_error("lack operand");
			operand right = operands.top();
			operands.pop();
			if (operands.empty())
				throw std::logic_error("lack operand");
			operand left = operands.top();
			operands.pop();

			operands.push(ope->calculate(left, right));
		}
	}
	if (operands.size() == 0)
		throw std::logic_error("no operands");
	if (operands.size() != 1)
		throw std::logic_error("too many operands");
	return operands.top();
}

ostream& operator<<(ostream &out, const operand &sym)
{
	out << sym.getter();
	return out;
}