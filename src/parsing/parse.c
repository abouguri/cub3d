/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:49:13 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 04:16:17 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	validate_tokens(char **tokens)
{
	if (!tokens || ft_array_length(tokens) != 2)
	{
		free_array(&tokens);
		return (0);
	}
	return (1);
}

static int	is_texture_identifier(char *token)
{
	return (ft_strncmp(token, "NO", 3) == 0 || ft_strncmp(token, "SO", 3) == 0
		|| ft_strncmp(token, "WE", 3) == 0 || ft_strncmp(token, "EA", 3) == 0);
}

static int	is_color_identifier(char *token)
{
	return (ft_strncmp(token, "F", 2) == 0 || ft_strncmp(token, "C", 2) == 0);
}

static int	process_identifier(t_cub *data, char **tokens)
{
	if (is_texture_identifier(tokens[0]))
	{
		return (parse_texture(data, tokens[0], tokens[1], tokens));
	}
	else if (is_color_identifier(tokens[0]))
	{
		return (parse_color(data, tokens[0], tokens[1], tokens));
	}
	else
	{
		map_info_error(tokens, UNKNOWN_IDENTIFIER);
		return (1);
	}
}

int	parse_line(char *line)
{
	char	**tokens;
	t_cub	*data;
	int		result;

	tokens = ft_split(line, ' ');
	data = get_cub_data();
	if (!validate_tokens(tokens))
		return (1);
	result = process_identifier(data, tokens);
	if (result == 0)
		free_array(&tokens);
	return (result);
}
