/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:49:13 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:54:04 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_line(char *line)
{
	char	**tokens;
	t_cub	*data;

	tokens = ft_split(line, ' ');
	data = get_cub_data();
	if (!tokens || ft_array_length(tokens) != 2)
	{
		free_array(&tokens);
		return (1);
	}
	if (ft_strncmp(tokens[0], "NO", 3) == 0 || ft_strncmp(tokens[0], "SO",
			3) == 0 || ft_strncmp(tokens[0], "WE", 3) == 0
		|| ft_strncmp(tokens[0], "EA", 3) == 0)
	{
		if (parse_texture(data, tokens[0], tokens[1], tokens) == 1)
			return (1);
	}
	else if (ft_strncmp(tokens[0], "F", 2) == 0 || ft_strncmp(tokens[0], "C",
			2) == 0)
	{
		if (parse_color(data, tokens[0], tokens[1], tokens) == 1)
			return (1);
	}
	else
		return (map_info_error(tokens, UNKNOWN_IDENTIFIER), 1);
	free_array(&tokens);
	return (0);
}
