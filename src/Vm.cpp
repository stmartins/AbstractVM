/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vm.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <stmartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 17:59:38 by stmartin          #+#    #+#             */
/*   Updated: 2018/04/17 14:44:39 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Vm.hpp"
#include "IOperand.hpp"
#include "Factory.hpp"

Vm::Vm( void ):
_end(0),
_factory(new Factory),
_exit(0)
{
	(void)_exit;
}

Vm::Vm( Vm const & src )
{
	*this = src;
}

Vm::~Vm()
{
	delete _factory;
}

Vm 			&Vm::operator=( Vm const & rhs )
{
	if (this != &rhs)
		*this = rhs;
	return *this;
}

void	Vm::run()
{
	std::string buf;

    while (!_end)
    {
		try
		{
	    	if (!std::getline(std::cin, buf))
	    		break;
			read_args(buf);
		}
		catch (std::runtime_error & e)
		{
			std::cout << e.what() << std::endl;
		}
		catch (std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	try
	{
		if ((_end && !_exit) || (!_end && !_exit))
			throw std::runtime_error("\033[1;31m error : Program stop before exit !\033[0m");
	}
	catch (std::runtime_error & e)
	{
		std::cout << e.what() << std::endl;
	}
}


void		Vm::run(char *av)
{
	std::ifstream file;
	std::string buf;

	file.open(av);
	try
	{
		if (!file)
			throw std::invalid_argument("\033[1;31m Wrong file name !\033[0m");
		else if (file.is_open() == false)
			throw std::runtime_error("\033[1;31m File open failed !\033[0m");
	}
	catch (std::runtime_error & e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (std::invalid_argument & e)
	{
		std::cout << e.what() << std::endl;
	}
	while (file.eof() == false)
	{
		try
		{
			if (!std::getline(file, buf))
				break;
			read_args(buf);
		}
		catch (std::runtime_error & e)
		{
			std::cout << e.what() << std::endl;
		}
		catch (std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	file.close();
	try
	{
		if ((_end && !_exit) || (!_end && !_exit))
			throw std::runtime_error("\033[1;31m error : Program stop before exit");
	}
	catch (std::runtime_error & e)
	{
		std::cout << e.what() << std::endl;
	}
}

const std::map<std::string, int> 	Vm::createMap()
{
	std::map<std::string, int> m;

	m["Int8"] = 1;
	m["Int16"] = 2;
	m["Int32"] = 3;
	m["Float"] = 4;
	m["Double"] = 5;
	return m;
}

void		Vm::read_args(std::string buf)
{

	if (buf.find(";;") == 0)
		_end = 1;
	else if (!_end && buf.find(";") != std::string::npos && buf.find(";") == 0)
		return ;
	else if (!_exit && buf.find("push") == 0)
	{
		_asmArg = PUSH;
		check_operand(buf, 3);
	}
	else if (!_exit && buf.find("assert") == 0)
	{
		_asmArg = ASSERT;
		check_stack();
		check_operand(buf, 5);
	}
	else if (!_exit && buf.find("dump") == 0 && check_word(buf, 4))
		dump_stack();
	else if (!_exit && buf.find("add") == 0 && check_word(buf, 3))
	{
		check_stack();
		add();
	}
	else if (!_exit && buf.find("sub") == 0 && check_word(buf, 3))
	{
		check_stack();
		sub();
	}
	else if (!_exit && buf.find("mul") == 0 && check_word(buf, 3))
	{
		check_stack();
		mul();
	}
	else if(!_exit && buf.find("div") == 0 && check_word(buf, 3))
	{
		check_stack();
		divi();
	}
	else if (!_exit && buf.find("mod") == 0 && check_word(buf, 3))
	{
		check_stack();
		mod();
	}
	else if (!_exit && buf.find("pop") == 0 && check_word(buf, 3))
	{
		_asmArg = POP;
		check_stack();
		_stack.pop_back();
	}
	else if (!_exit && buf.find("print") == 0 && check_word(buf, 5))
	{
		_asmArg = PRINT;
		check_stack();
		check_for_print();
	}
	else if (!_exit && buf.find("min") == 0 && check_word(buf, 3))
	{
		_asmArg = MIN;
		check_stack();
		min();
	}
	else if (!_exit && buf.find("max") == 0 && check_word(buf, 3))
	{
		_asmArg = MAX;
		check_stack();
		max();
	}
	else if (!_exit && buf.find("avg") == 0 && check_word(buf, 3))
	{
		_asmArg = AVG;
		check_stack();
		avg();
	}
	else if (buf.find("exit") == 0 && check_word(buf, 4))
		_exit = 1;
	else if (!_exit)
		throw std::invalid_argument("\033[1;31mInvalid Argument !\033[0m");
}

bool		Vm::check_word(std::string buf,size_t s)
{
	if ((buf.length() == s || (buf.find(";") == s && buf.find(";") != std::string::npos) || (buf.find(" ") == s && buf.find(" ") != std::string::npos && (buf.find(";") == s + 1 && buf.find(";") != std::string::npos))))
		return 1;
	else if ((_asmArg == PUSH || _asmArg == ASSERT) && ((buf.find(" ", s - 1) != std::string::npos)))
		return 1;

	return 0;
}

void		Vm::check_stack()
{
	size_t s = ((_asmArg == PRINT || _asmArg == ASSERT || _asmArg == POP || _asmArg == MIN || _asmArg == MAX || _asmArg == AVG) ? 1 : 2);

	if (_stack.empty())
		throw std::runtime_error("\033[1;31mStack is empty !\033[0m");
	else if (_stack.size() < s)
		throw std::runtime_error("\033[1;31mStack have less than 2 elements !\033[0m");
}

void		Vm::check_operand(std::string const &buf, size_t start)
{
	size_t pos;

	if ((_asmArg == PUSH || _asmArg == ASSERT) && (pos = buf.find(" ", start)) == start + 1)
		chooseType(buf, pos + 1);
	else if ((_asmArg == PUSH || _asmArg == ASSERT) && pos != start + 1)
		throw std::runtime_error("\033[1;31mWrong instruction format !\033[0m");
}

void		Vm::chooseType(std::string const & buf, size_t start)
{

	if (buf.find("int8", start) == start)
	{
		_type = Int8;
		_value = "Int8";
		start += 4;
	}
	else if (buf.find("int16", start) == start)
	{
		_type = Int16;
		_value = "Int16";
		start += 5;
	}
	else if (buf.find("int32", start) == start)
	{
		_type = Int32;
		_value = "Int32";
		start += 5;
	}
	else if (buf.find("float", start) == start)
	{
		_type = Float;
		_value = "Float";
		start += 5;
	}
	else if (buf.find("double", start) == start)
	{
		_type = Double;
		_value = "Double";
		start += 6;
	}
	else if (buf.find("\0", start) == start)
		throw std::runtime_error("\033[1;31mWrong instruction format !\033[0m");
	check_bracket(buf, start);
}

void		Vm::check_bracket(std::string const & buf, size_t start)
{
	size_t opBr;
	size_t clBr;
	size_t len = 0;
	std::string nb;

	if ((opBr = buf.find("(", start)) == start)
	{
		if ((clBr = buf.find(")", start)) < opBr || buf.find(")", start) == std::string::npos)
			throw BracketException("\033[1;31mNo closing Brackets match !\033[0m");
		else if (!check_word(buf, clBr + 1))
		{
			std::cout << "len " << clBr <<std::endl;
			throw std::runtime_error("\033[1;31mWrong instruction after closing bracket !\033[0m");
		}
		else
		{
				len = (clBr - opBr) - 1;
				nb = buf.substr(opBr + 1, len);
				if (_type == Int8 || _type == Int16 || _type == Int32)
					checkInteger(nb);
				else
					checkDecimal(nb);
				castValue(nb);
		}
	}
}

void		Vm::check_for_print()
{
	std::stringstream	ss;
	int					c;

	std::vector<const IOperand *>::reverse_iterator it = _stack.rbegin();
	if ((*it)->getType() != Int8)
		throw std::runtime_error("\033[1;31mNot a type char\033[0m");
	else
	{
		ss << (*it)->toString();
		ss >> c;
		std::cout << static_cast<char>(c) << std::endl;
	}
}

void		Vm::checkInteger(std::string const &value)
{
	if (value == "+" || value == "-"
|| !std::regex_match(value, std::regex("^[+\\-]?(?:0|[1-9]\\d*)?$")))
		throw LexicalSyntacticException("\033[1;35mSyntactic exception (Value is not an integer)\033[0m");
}

void		Vm::checkDecimal(std::string const &value)
{
	if (value == "+" || value == "-" || value == "+." || value == "-." || value == "."
	|| !std::regex_match(value, std::regex("^[+\\-]?(?:|0|[1-9]\\d*)(?:\\.\\d*)?(?:[eE][+\\-]?\\d+)?$")))
		throw LexicalSyntacticException("\033[1;35mSyntactic exception (Value is not a decimal number)\033[0m");
}

void		Vm::castValue(std::string const & nb)
{
	if (_type == Int8 || _type == Int16 || _type == Int32 || _type == Float || _type == Double)
	{
		_value = nb;
		if (_asmArg == PUSH)
			_stack.push_back(_factory->createOperand(_type, _value));
		else if (_asmArg == ASSERT)
		{
			_stack.push_back(_factory->createOperand(_type, _value));
			std::vector<const IOperand *>::reverse_iterator it = _stack.rbegin();
			std::vector<const IOperand *>::reverse_iterator it2 = _stack.rbegin() + 1;
			if (((*it)->toString() != (*it2)->toString()) || ((*it)->getType() != (*it2)->getType()))
			{
				_stack.pop_back();
				throw std::runtime_error("\033[1;31mAssert not equal\033[0m");
			}
			_stack.pop_back();
		}
	}
}

void		Vm::add()
{
	IOperand const 		*left, *right, *rsl;

	right = *(_stack.rbegin());
	left = *(_stack.rbegin() + 1);
	rsl = *left + *right;
	_stack.pop_back();
	_stack.pop_back();
	_stack.push_back(rsl);
}

void		Vm::sub()
{
	IOperand const 		*left, *right, *rsl;

	right = *(_stack.rbegin());
	left = *(_stack.rbegin() + 1);
	rsl = *left - *right;

	_stack.pop_back();
	_stack.pop_back();
	_stack.push_back(rsl);
}

void		Vm::mul()
{
	IOperand const 		*left, *right, *rsl;

	right = *(_stack.rbegin());
	left = *(_stack.rbegin() + 1);
	rsl = *left * *right;
	_stack.pop_back();
	_stack.pop_back();
	_stack.push_back(rsl);
}

void		Vm::divi()
{
	IOperand const 		*left, *right, *rsl;

	right = *(_stack.rbegin());
	left = *(_stack.rbegin() + 1);
	rsl = *left / *right;

	_stack.pop_back();
	_stack.pop_back();
	_stack.push_back(rsl);
}

void		Vm::mod()
{
	IOperand const 		*left, *right, *rsl;

	right = *(_stack.rbegin());
	left = *(_stack.rbegin() + 1);
	rsl = *left % *right;

	_stack.pop_back();
	_stack.pop_back();
	_stack.push_back(rsl);
}

void		Vm::dump_stack()
{
	if (_stack.size() < 1)
		throw std::runtime_error("\033[1;31mCan't dump, Stack is empty \033[0m");
	else
	{
		for (std::vector<const IOperand *>::reverse_iterator it = _stack.rbegin() ; it != _stack.rend(); ++it)
			std::cout << (*it)->toString() << std::endl;
	}
}

void		Vm::min()
{
	double min = std::numeric_limits<double>::max();
	double rsl;
	for (std::vector<const IOperand *>::reverse_iterator it = _stack.rbegin() ; it != _stack.rend(); ++it)
	{
		if ((rsl = std::stod((*it)->toString())) < min)
			min = rsl;
	}
	std::cout << "\033[1;36mmin : " << min << "\033[0m" << std::endl;
}

void		Vm::max()
{
	double max = -std::numeric_limits<double>::max();
	double rsl;
	for (std::vector<const IOperand *>::reverse_iterator it = _stack.rbegin() ; it != _stack.rend(); ++it)
	{
		if ((rsl = std::stod((*it)->toString())) > max)
			max = rsl;
	}
	std::cout << "\033[1;33mmax : " << max << "\033[0m" << std::endl;
}

void		Vm::avg()
{
	double avg = 0;
	size_t i = 0;
	double rsl = 0;

	for (std::vector<const IOperand *>::reverse_iterator it = _stack.rbegin() ; it != _stack.rend(); ++it)
	{
		rsl += std::stod((*it)->toString());
		i++;
	}
	if (i)
		avg = rsl / i;
	std::cout << "\033[1;35mavg : " << avg << "\033[0m" << std::endl;

}

void		Vm::setVm()
{
	_end = 0;
	_exit = 0;
	_stack.clear();
}
