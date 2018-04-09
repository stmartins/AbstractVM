/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vm.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <stmartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 17:59:38 by stmartin          #+#    #+#             */
/*   Updated: 2018/04/09 19:23:40 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Vm.hpp"

Vm::Vm( void )
{ }

Vm::Vm( Vm const & src )
{
	*this = src;
}

Vm::~Vm()
{ }

Vm &Vm::operator=( Vm const & rhs )
{
	if (this != &rhs)
		*this = rhs;
	return *this;
}

void	Vm::run()
{
	std::string action;
	int out = 0;

    while (!out)
    {
    	if (!std::getline(std::cin, action))
    		break;
		else if (action == "ok")
			std::cout << "ca marche" << std::endl;
	}
}


void	Vm::run(char *av)
{
	std::ifstream file;

		if (!file)
			throw std::invalid_argument("File empty !");
		else
		{
			std::string s;
			file.open(av);
			if (file.fail())
				throw std::runtime_error("File open failed !");
			while (file >> s)
				std::cout << s << std::endl;
			file.close();
		}
}
