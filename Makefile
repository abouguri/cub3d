NAME := cub3d

CC := gcc -fsanitize=address

CFLAGS := -D #BUFFER_SIZE=100

FILES := main.c

LIBRARY := -Lminilibx-linux -lmlx -L/usr/lib/x86_64-linux-gnu -lXext -lX11 -lm
MINILIBX := minilibx-linux/

all:
	make -C $(MINILIBX)
	$(CC) $(CFLAGS) $(FILES) $(LIBRARY) -o $(NAME)

clean:
	make clean -C $(MINILIBX)

fclean: clean
	make fclean -C $(MINILIBX)
	rm -rf $(NAME)

re: fclean all
