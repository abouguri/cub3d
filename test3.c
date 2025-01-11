int arg_check(int ac, char **av)
{
  int error;
  
  error = 0;
  if (ac > 3)
	error = TOO_MANY_ARGS;
	if (ac < 2)
	error = TOO_FEW_ARGS;
  if (ac == 2)
    if (ft_strncmp(av[1] + ft_strlen(av[1]) - 4, ".cub", 4) != 0)
      error = INVALID_EXT;
}

// #include "cub3d.h"

# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include <limits.h>

typedef	enum    e_error
{
    TOO_MANY_ARGS,
    TOO_FEW_ARGS,
    INVALID_EXT,
}               t_error;

// texture

# define TEXTURE_HEIGHT 			64
# define TEXTURE_WIDTH 				64

// player struct

typedef struct s_key
{
	int	w;
	int	s;
	int	a;
	int	d;
	int	right;
	int	left;
	int	p;
}				t_key;

// game structs

typedef struct s_point
{
	double			x;
	double			y;
}			t_point;

typedef struct s_state
{
	t_point			position;
	t_point			direction;
	t_point			plane;
}			t_state;

typedef struct s_var
{
	double			position_x;
	double			position_y;
	double			direction_x;
	double			direction_y;
	double			plane_x;
	double			plane_y;
	double			camera_x;
	double			ray_direction_x;
	double			ray_direction_y;
	double			side_distance_x;
	double			side_distance_y;
	double			delta_distance_x;
	double			delta_distance_y;
	double			perspective_wall_distance;
	double			wall_x;
	double			step;
	double			texture_position;
	double			frame_time;
	double			move_speed;
	double			rotate_speed;

	int				map_x;
	int				map_y;
	int				step_x;
	int				step_y;
	int				hit;
	int				side;
	int				line_height;
	int				draw_start;
	int				draw_end;
	int				texture_number;
	int				texture_y;
	int				texture_x;

	unsigned int	color;
}			t_var;

// screen struct

typedef struct s_img
{
	void	*pointer_to_image;
	char	*address;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}				t_img;

// textures struct

typedef struct s_img2
{
	void	*pointer_to_image;
	int		*address;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}				t_img2;

typedef struct s_cub
{
	void			*mlx;
	void			*win;
	t_img			img;
	t_img2			img2[4];
	char			**rgb;
	char			**xpm;
	char			**map;
	unsigned long	floor;
	unsigned long	ceilling;
	t_var			var;
	int				texture[4][TEXTURE_HEIGHT * TEXTURE_WIDTH];
	t_key			key;
}				t_cub;

t_cub	*data(void)
{
	static t_cub	data;

	return (&data);
}

const char	*get_exit(const char *file)
{
	const char	*dot;

	dot = ft_strrchr(file, '.');
	if (!dot || dot == file)
		return ("");
	return (dot + 1);
}

static int	textures(char *line)
{
	char	**tokens;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (1);
	if (ft_array_length(tokens) != 2)
		return (1);
	if (ft_strncmp(tokens[0], "NO", 3) == 0)
		data()->xpm[0] = ft_strdup(tokens[1]);
	else if (ft_strncmp(tokens[0], "EA", 3) == 0)
		data()->xpm[1] = ft_strdup(tokens[1]);
	else if (ft_strncmp(tokens[0], "SO", 3) == 0)
		data()->xpm[2] = ft_strdup(tokens[1]);
	else if (ft_strncmp(tokens[0], "WE", 3) == 0)
		data()->xpm[3] = ft_strdup(tokens[1]);
	else if (ft_strncmp(tokens[0], "C", 2) \
		&& ft_strncmp(tokens[0], "F", 2))
		return (1);
	ft_free(&tokens);
	return (0);
}

static int	rgb(char *line)
{
	char	**tokens;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (1);
	if (ft_array_length(tokens) != 2)
		return (1);
	if (ft_strncmp(tokens[0], "F", 1) == 0)
		data()->rgb[0] = ft_strdup(tokens[1]);
	else if (ft_strncmp(tokens[0], "C", 1) == 0)
		data()->rgb[1] = ft_strdup(tokens[1]);
	ft_free(&tokens);
	return (0);
}

void	ft_free(char ***array)
{
	int	i;

	i = 0;
	if (!*array)
		return ;
	while ((*array)[i])
	{
		free((*array)[i]);
		i++;
	}
	free(*array);
	*array = NULL;
}

static void	ft_exit_parse(char *s, int fd)
{
	if (fd < 0)
		close(fd);
	write(2, s, ft_strlen(s));
	ft_free(&data()->xpm);
	ft_free(&data()->rgb);
	ft_free(&data()->map);
	exit(1);
}

int	ft_array_length(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		i++;
	}
	return (i);
}

int	parse_info(int fd)
{
	char	*line;
	int		ret;

	data()->xpm = ft_calloc(sizeof(char *), 5);
	if (!data()->xpm)
		return (1);
	data()->rgb = ft_calloc(sizeof(char *), 3);
	if (!data()->rgb)
		return (1);
	while (ft_array_length(data()->xpm) != 4 \
		|| ft_array_length(data()->rgb) != 2)
	{
		ret = get_next_line(fd, &line);
		if (ret == -1)
			return (1);
		if (ft_strlen(line) == 0)
			;
		else if (textures(line) == 1 || rgb(line) == 1)
		{
			free(line);
			return (1);
		}
		free(line);
		if (ret == 0)
			break ;
	}
	return (0);
}

int	parse(char *file)
{
	int	fd;

	if (ft_strncmp(get_exit(file), "cub", 4) != 0)
		ft_exit_parse("Error\nInvalid extension\n", -1);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		ft_exit_parse("Error\nCannot open the map\n", fd);
	if (parse_info(fd) == 1)
		ft_exit_parse("Error\nInvalid information\n", fd);
	if (parse_map(fd) == 1)
		ft_exit_parse("Error\nInvalid map\n", fd);
	close(fd);
	return (0);
}


int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "Error\nMap not provided properly\n", 32);
		return (1);
	}
	parse(argv[1]);
	return (0);
}