/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:43:50 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/02 20:44:27 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	validate_file_extension(const char *filename)
{
	const char	*dot = strrchr(filename, '.');

	return (dot && strcmp(dot + 1, "cub") == 0);
}

int	parse(char *file)
{
	int	fd;

	if (!validate_file_extension(file))
		error_exit(ERR_INVALID_EXT);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		error_exit(ERR_FILE_OPEN);
	if (parse_file(fd) == 1)
		error_exit_fd_cleanup(ERR_INVALID_INFO, fd);
	if (parse_map(fd) == 1)
		error_exit_fd_cleanup(ERR_INVALID_MAP, fd);
	if (validate_full_map() == 1)
		error_exit_cleanup(ERR_INVALID_MAP);
	close(fd);
	return (0);
}
