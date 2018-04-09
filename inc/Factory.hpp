/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <stmartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 01:46:26 by stmartin          #+#    #+#             */
/*   Updated: 2018/04/09 02:09:23 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "IOperand.hpp"
#include <vector>

class Factory {

public:

    Factory( void );
    Factory( std::string const & name );
    Factory( Factory const & src );
    virtual ~Factory();

    Factory &            operator=( Factory const & rhs );

    IOperand const * createOperand( eOperandType type, std::string const & value ) const;

private:

    IOperand const * createInt8( std::string const & value ) const;
    IOperand const * createInt16( std::string const & value ) const;
    IOperand const * createInt32( std::string const & value ) const;
    IOperand const * createFloat( std::string const & value ) const;
    IOperand const * createDouble( std::string const & value ) const;

    std::vector<void (*)()> vectorOfFunctions;

};

#endif