# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/16 11:21:59 by nrasamim          #+#    #+#              #
#    Updated: 2025/01/27 11:56:05 by nrasamim         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

CC		=	gcc

FLAG = -Wall -Werror -Wextra

DIRLIB	=	./libft/
FILELIB	=	libft.a
NAMELFT	=	$(addprefix $(DIRLIB), $(FILELIB))

SRC_DIR	=	src/
INCLUDE	=	-I ./inc -I ./libft -lreadline

SRCS	=	$(shell find $(SRC_DIR) -name '*.c')
OBJS	=	$(patsubst $(SRC_DIR)/%.c, %.o, $(SRCS))

all : $(NAME)

$(NAME) : $(OBJS)
	make -C $(DIRLIB)
	$(CC) $(FLAG) $(OBJS) $(NAMELFT) $(INCLUDE) -o $(NAME)

clean :
	@ make clean -C $(DIRLIB)

fclean : clean
	@make fclean -C $(DIRLIB)
	@rm -rf $(NAME)

re : fclean all

debug : $(NAME)
	make clean && clear && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=debug -s ./minishell

run : fclean all
	@clear ; ./minishell

.PHONY : all clean fclean re