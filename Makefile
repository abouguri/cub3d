NAME := cub3d

CC := gcc #-fsanitize=address
CFLAGS := -DBUFFER_SIZE=100

SRC := main.c $(wildcard minilibx-linux/*.c)

LIBRARY := -Lminilibx-linux -lmlx -L/usr/lib/x86_64-linux-gnu -lXext -lX11 -lm
MINILIBX := minilibx-linux/

all: $(NAME)

$(NAME): $(SRC)
	@make -C $(MINILIBX)
	$(CC) $(CFLAGS) $(SRC) $(LIBRARY) -o $(NAME)

clean:
	@make clean -C $(MINILIBX)

fclean: clean
	@make fclean -C $(MINILIBX)
	rm -rf $(NAME)

re: fclean all
