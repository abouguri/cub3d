/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:03:44 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/03 03:47:51 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int argc, char **argv)
{
	if (argc < 2)
		error_exit(ERR_TOO_FEW_ARGS);
	if (argc > 2)
		error_exit(ERR_TOO_MANY_ARGS);
	parse(argv[1]);
	for (int i = 0; get_cub_data()->textures[i]; i++)
		printf("textures: %s\n", get_cub_data()->textures[i]);
	for (int i = 0; get_cub_data()->colors[i]; i++)
		printf("colors: %s\n", get_cub_data()->colors[i]);
	for (int i = 0; get_cub_data()->map[i]; i++)
		printf("map: %s\n", get_cub_data()->map[i]);
	init();
	return (EXIT_SUCCESS);
}
