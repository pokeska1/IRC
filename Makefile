CC		= clang++ -std=c++98

FLAG	= -g #-Wall -Wextra -Werror

OBJC =		$(SRCS:.cpp=.o)

NAME	= ircserv

SRCS	=	main.cpp \
 			Server.cpp \
			User.cpp \
			Channel.cpp


RM		= rm -f

all:		$(NAME)

$(NAME) : $(OBJC)
			$(CC) $(FLAG) $(OBJC) -o $(NAME)

%.o:	%.cpp #Server.hpp
			$(CC) -c $(FLAG) $< -o $@

clean:
			$(RM) $(OBJC)

fclean:	clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clan fclean re