/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:48:57 by abouguri          #+#    #+#             */
/*   Updated: 2025/01/06 11:44:42 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int arg_check(int ac, char **av)
{
    int error;

    error = 0;
    if (ac > 3)
		error = TOO_MANY_ARGS;
	if (ac < 2)
		error = TOO_FEW_ARGS;
    
}