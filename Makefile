# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apintus <apintus@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/23 17:16:05 by apintus           #+#    #+#              #
#    Updated: 2024/03/11 17:34:49 by apintus          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#########################################################
## ARGUMENTS

NAME = pipex
ARCHIVE = pipex.a
CC = cc
RM = rm -f
CFLAGS = -Wall -Werror -Wextra -g
MAKE_LIB = ar -rcs
BONUS_NAME = pipex_bonus
BONUS_ARCHIVE = pipex_bonus.a

##########################################################
## SOURCES

SRCS = srcs/main.c\
		srcs/utils.c\
		srcs/pipex.c\
		srcs/exec.c\

OBJS = $(SRCS:.c=.o)

# LIBFT
LIBFT_DIR = libft
LIBFT_MAKE_COMMAND = make -s -C $(LIBFT_DIR)
LIBFT_PATH = $(LIBFT_DIR)/libft.a

# BONUS

SRCS_BONUS = bonus/main_bonus.c\
		bonus/utils_bonus.c\
		bonus/pipex_bonus.c\
		bonus/exec_bonus.c\
		bonus/here_doc_bonus.c\

OBJS_BONUS = $(SRCS_BONUS:.c=.o)

##########################################################
## RULES

all : $(NAME) $(LIBFT_PATH)

$(NAME) : $(ARCHIVE) $(LIBFT_PATH)
	$(CC) $(ARCHIVE) $(LIBFT_PATH) -o $(NAME)

$(ARCHIVE) : $(OBJS)
	$(MAKE_LIB) $(ARCHIVE) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_PATH) :
	$(LIBFT_MAKE_COMMAND)

clean :
	$(RM) $(OBJS) $(ARCHIVE)
	$(RM) $(OBJS_BONUS) $(BONUS_ARCHIVE)
	$(LIBFT_MAKE_COMMAND) clean

fclean : clean
	$(RM) $(NAME)
	$(RM) $(BONUS_NAME)
	$(LIBFT_MAKE_COMMAND) fclean

re : fclean all

bonus : $(BONUS_NAME) $(LIBFT_PATH)

$(BONUS_NAME) : $(BONUS_ARCHIVE) $(LIBFT_PATH)
	$(CC) $(BONUS_ARCHIVE) $(LIBFT_PATH) -o $(BONUS_NAME)

$(BONUS_ARCHIVE) : $(OBJS_BONUS)
	$(MAKE_LIB) $(BONUS_ARCHIVE) $(OBJS_BONUS)

.PHONY : all clean fclean re bonus
