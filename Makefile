# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/25 13:22:33 by gtoubol           #+#    #+#              #
#    Updated: 2022/11/23 22:02:22 by gtoubol          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL=/bin/bash

# List of all the sources (.cpp)
# -------------------------------------------------------------------------
SRCS =		$(addprefix config/,											\
				configure.cpp												\
				ConfigEntry.cpp												\
				VirtualServer.cpp											\
				ConfigTree.cpp												\
			)

# List of test sources (.cpp)
# -------------------------------------------------------------------------
TEST =			$(addprefix config/,										\
					test_configure.cpp										\
				)

TEST_DIR =	tests
TEST_OBJS =	$(addprefix $(OBJS_DIR)/,$(TEST:.cpp=.o))
DOC_FILE = doxygen.conf

TEST_EXE = $(addprefix $(SRCS_DIR)/,$(TEST:.cpp=.test))

# List of the related directories
# -------------------------------------------------------------------------
OBJS_DIR =	objs
SRCS_DIR =	srcs
HEAD_DIR =	includes
TPP_DIR =	templates

# List of all compilation options
# -------------------------------------------------------------------------
CXX = 		c++
CXXFLAGS =	-Wall -Wextra -Werror --std=c++98

# Description of the final target
# -------------------------------------------------------------------------
NAME =		webserv

# Libraries
# -------------------------------------------------------------------------

# General rules on makefile
# -------------------------------------------------------------------------
OBJS = 		$(addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o))
DEPS =		$(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

INCLUDES =	$(addprefix -I,$(HEAD_DIR))

RM =		rm -f

vpath %.c $(SRCS_DIR)
vpath %.h $(HEAD_DIR)

# Color
# -------------------------------------------------------------------------
_GREY		= \033[30m
_RED		= \033[31m
_GREEN		= \033[32m
_YELLOW		= \033[33m
_BLUE		= \033[34m
_PURPLE		= \033[35m
_CYAN		= \033[36m
_WHITE		= \033[37m
_NO_COLOR	= \033[0m


$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp $(OBJS_DIR)/%.d
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
		echo "\n$(_BLUE)$(dir $@): Create$(_NO_COLOR)"; \
	fi
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

all:		$(NAME)

clean:		dclean
			$(RM) $(OBJS)
			@if [ -d $(OBJS_DIR) ]; then \
				find $(OBJS_DIR) -type d | xargs rmdir -p --ignore-fail-on-non-empty; \
			fi

tclean:
			$(RM) $(TEST_OBJS) $(TEST_OBJS:.o=.test)

fclean:		clean tclean
			$(RM) $(NAME)


doc:
			doxygen $(DOC_FILE)

test:		$(TEST_EXE)
			pytest

$(SRCS_DIR)/%.test:		$(OBJS_DIR)/%.o $(filter-out main.cpp,$(OBJS))
			$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

re:			fclean all

.PHONY:		all clean fclean re doc tests
.SILENT:	test
.NOTPARALLEL: re

# Library rules
# -----------------------------------------------------------------------

# General dependences management
# ------------------------------------------------------------------------
$(OBJS_DIR)/%.d: $(SRCS_DIR)/%.cpp
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
		echo "\n$(_BLUE)$(dir $@): Create$(_NO_COLOR)"; \
	fi
	$(CXX) -MM -MT $(@:.d=.o) $(CXXFLAGS) $(INCLUDES) $< >> $@

dclean:
			$(RM) $(DEPS)

.PHONY:		dclean
.SILENT:    $(DEPS) dclean

-include	$(DEPS)
