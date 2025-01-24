/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:03:44 by abouguri          #+#    #+#             */
/*   Updated: 2025/01/24 16:49:09 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
# include "minilibx-linux/mlx.h"

// screen

# define SCREEN_HEIGHT	1000
# define SCREEN_WIDTH 	1500
# define CELL_SIZE	100
# define TEXTURE_HEIGHT 64
# define TEXTURE_WIDTH  64

// Constants

// #define BUFFER_SIZE 42
# define TEXTURE_COUNT 4
# define RGB_COUNT 2
# define NORTH 'N'
# define SOUTH 'S'
# define EAST 'E'
# define WEST 'W'
# define VALID_MAP_SYMBOLS " 10NSEW"

#include  <string.h>   
// Error Messages
# define ERR_TOO_FEW_ARGS "Error\nToo few arguments. Provide a .cub file.\n"
# define ERR_TOO_MANY_ARGS "Error\nToo many arguments. Only the .cub file is required.\n"
# define ERR_INVALID_EXT "Error\nInvalid file extension. The file must have a .cub extension.\n"
# define ERR_FILE_OPEN "Error\nCannot open the map file.\n"
# define ERR_INVALID_INFO "Error\nInvalid map information in the file.\n"
# define ERR_INVALID_MAP "Error\nInvalid map structure.\n"
# define ERR_TEXTURE_LOAD "Error\nCannot load textures\n"
# define ERR_COLORS_LOAD "Error\nCannot load colors\n"
# define DUPLICATE_TEXTURE "Error\nDuplicate texture identifier: "
# define DUPLICATE_COLOR "Error\nDuplicate color identifier: "
# define UNKNOWN_IDENTIFIER "Error\nUnknown identifier: "
# define BOUNDARY_CELL_NOT_ENCLOSED "Error\nCell (%d, %d) is at the boundary and not enclosed\n"
# define CELL_NOT_ENCLOSED "Error\nCell (%d, %d) is not enclosed\n"
# define SMALL_MAP "Error\nMap is too small\n"
# define INVALID_CHARACTERS "Error\nMap contains invalid characters\n"
# define TRAILING_EMPTY_LINES "Error\nMap contains trailing empty lines\n"
# define SINGLE_PLAYER_NEEDED "Error\nMap must contain exactly one player (found %d)\n"
# define ERR_INIT_MLX "Error\nFailed to initialize MLX"
# define ERR_CREATE_WINDOW "Error\nFailed to create window"
# define ERR_CREATE_IMAGE "Error\nFailed to create image"
# define ERR_GET_IMAGE_ADDR "Error\nFailed to get image address"


// Structs


// 	unsigned int	color;
// }			t_var;

// typedef struct s_cub {
//     char    **textures;
//     char    **colors;
//     char    **map;
//     char    **rgb;
// 	char    **xpm;
//     t_var			var;
// } t_cub;

typedef struct s_var
{
	double			position_x;
	double			position_y;
	double			direction_x;
	double			direction_y;
	double			plane_x;
	double			plane_y;
}				t_var;

typedef struct s_img2
{
	void	*img_ptr;
	int		*data_addr;
	int		bpp;
	int		line_size;
	int		endian;
	int		width;
	int		height;
}				t_img2;

typedef struct s_cub {
	void	*mlx;
	void	*win;
    t_img2  *img2[4];
    char    **textures;
    char    **colors;
    char    **map;
	int		texture[4][TEXTURE_HEIGHT * TEXTURE_WIDTH];
	unsigned long	floor;
	unsigned long	ceilling;
    t_var   var;
} t_cub;


// Function Prototypes
void error_exit(const char *message);
void error_exit_with_cleanup(const char *message, int fd);
t_cub *get_cub_data(void);
void free_array(char ***array);

int validate_file_extension(const char *filename);
int parse(char *file);
int parse_file(int fd);
int parse_map(int fd);
int parse_textures(char *line);
int parse_colors(char *line);
int	validate_full_map(void);
int parse_line(char *line);

int get_next_line(int fd, char **line);

#endif
