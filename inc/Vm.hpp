/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vm.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <stmartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 17:57:06 by stmartin          #+#    #+#             */
/*   Updated: 2018/04/09 19:20:34 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_HPP
# define VM_HPP

#include <iostream>
#include <fstream>
#include <string>

class Vm {

public:

	Vm( void );
	Vm( Vm const & src );
	virtual ~Vm();

	Vm &operator=( Vm const & rhs );

	void	run();
	void	run(char *av);

};


#endif
