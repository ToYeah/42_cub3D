NAME=cub3D
SRC_DIR= srcs
INC_DIR= includes
ENGINE_DIR= game_engine
VALID_DIR= valid

LIBFT_DIR= libft
LIBFT= $(LIBFT_DIR)/libft.a

MINILIBX_DIR= minilibx-linux
MINILIBX=$(MINILIBX_DIR)/minilibx.a

ENGINE_OBJS=\
angle_utils.c\
event_funcs.c\
free_funcs.c\
load_config.c\
main_loop.c\
malloc_funcs.c\
my_mlx_fuincs.c\
put_sprite.c\
put_wall_tex.c\
ray_casting.c\
save_screenshot.c\
validate_pos_funcs.c\
vector_utils.c

VALID_OBJS=\
cub_flood_fill.c\
init_valid_module.c\
valid_color.c\
valid_cub.c\
valid_map.c\
valid_others.c\
valid_resolution.c\
valid_statuses.c\
valid_texture.c

SRCS =\
main.c\
put_err_msg.c

SRCS+= $(addprefix $(VALID_DIR)/,$(VALID_OBJS))
SRCS+= $(addprefix $(ENGINE_DIR)/,$(ENGINE_OBJS))
OBJS = $(addprefix $(SRC_DIR)/$(),$(SRCS:.c=.o))

CC=gcc
CFLAGS=-Wall -Wextra -Werror
CFLAGS+=-I $(INC_DIR)
CFLAGS+=-I $(MINILIBX_DIR)
CFLAGS+=-I $(LIBFT_DIR)
CFLAGS+=-L $(MINILIBX_DIR)
CFLAGS+=-L $(LIBFT_DIR)
CFLAGS+=-lft -lmlx -lX11 -lXext -lm

all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT_DIR)
	$(MAKE) -C $(MINILIBX_DIR)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	$(MAKE) clean -C $(LIBFT_DIR) 
	$(MAKE) clean -C $(MINILIBX_DIR) 
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR) 
	$(MAKE) clean -C $(MINILIBX_DIR) 
	$(RM) $(NAME)

re: fclean all

bonus: all

.PHONEY	: all clean fclean re
