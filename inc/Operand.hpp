/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <stmartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/07 15:23:19 by stmartin          #+#    #+#             */
/*   Updated: 2018/04/13 18:11:07 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERAND_HPP
#define OPERAND_HPP

#include <iostream>
#include <stack>
#include <sstream>
#include "IOperand.hpp"
#include "Factory.hpp"


template <typename T>

class Operand : public IOperand {

public:

	Operand();

	Operand(std::string const &value, eOperandType type, /*const Factory* factory,*/ T const & nb):
	_type(type),
	// _factory(factory),
	_nb(nb),
	_str(value)
	{
		// (void)_factory;
		// std::cout << "on creer operand " << value << " a une valeur type de " << _type << " nb = " << nb << std::endl;
	}

	Operand( Operand const & src);

	~Operand( void )
	{ }

	Operand &        operator=( Operand const & rhs );


	virtual int getPrecision( void ) const // Precision of the type of the instance
	{
		int precision = 0;

		if (_type == Float)
			precision = 2;
		else if (_type == Double)
			precision = 7;
		return precision;
	}

	virtual eOperandType getType( void ) const // Type of the instance
	{
		return _type;
	}

	virtual IOperand const * operator+( IOperand const & rhs ) const // Sum
	{
		double left = this->_nb;
		double right = std::stod(rhs.toString());
		std::stringstream	ss;

		if (this->_type == Int8)
		{
			if (left + right > std::numeric_limits<char>::max() || left + right < std::numeric_limits<char>::min())
				throw std::runtime_error("Int8 overflow !");
			else
			{
				ss << left + right;
				return new Operand<short>(ss.str() , Int8, left + right);
			}
		}
		// std::cout << "lft " << left << " rgt " << right << std::endl;
		return (this);
	}

	// virtual IOperand const * operator-( IOperand const & rhs ) const; // Difference
	// virtual IOperand const * operator*( IOperand const & rhs ) const; // Product
	// virtual IOperand const * operator/( IOperand const & rhs ) const; // Quotient
	// virtual IOperand const * operator%( IOperand const & rhs ) const; // Modulo

	virtual std::string const & toString( void ) const // String representation of the instance
	{
		return _str;
	}

private:

	eOperandType		_type;
	// const Factory		*_factory;
	T					_nb;
	std::string			_str;
};

#endif
