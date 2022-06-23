NAME = ft_irc


SRCS =	src/main.cpp \
 		src/other/utils.cpp \
	    src/irc/ClientRequestParse.cpp \
	    src/irc/ClientRequestAnalyse.cpp \
	    src/irc/ClientResponse.cpp \
	    src/irc/ClientRequestRead.cpp \
	    src/irc/ClientRequestChans.cpp \
	    src/irc/Chan.cpp \

HDRS =  src/server/Webserv.hpp\
 		src/main.hpp \
 		src/server/PortServer.hpp \
 		src/irc/Request.hpp \
 		src/irc/Response.hpp \
 		src/irc/Client.hpp \
 		src/irc/Chan.hpp \

OBJS = $(SRCS:.cpp=.o)

FLAGS = -O3 -g -Wall -Wextra -Werror -std=c++98

COMP = g++

%.o: %.cpp		$(HDRS)
				@$(COMP) $(FLAGS) -c $< -o $@

all:			$(NAME)

$(NAME):		$(OBJS)
				@$(COMP) $(FLAGS) $(OBJS) -o $@

clean:
				@rm -f $(OBJS)

fclean:			clean
				@rm -f $(NAME)

re:				fclean all


.PHONY:			all clean fclean re
