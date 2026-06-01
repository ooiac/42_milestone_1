# **************************************************************************** #
#                                 ft_printf                                    #
# **************************************************************************** #

NAME        = libftprintf.a

SRCS        = ft_printf.c \
              char_utils.c \
              number_utils.c \
              ptr_utils.c \
              libft_utils.c \
              libft_utils_2.c

OBJS        = $(SRCS:.c=.o)

CC          = cc
CFLAGS      = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
