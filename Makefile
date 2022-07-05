NAME = libparsy.so

CC := gcc

ARCHIVER = ar

SRCS = src/parse.c

TESTS_SRCS := $(SRCS)
TESTS_SRCS += tests/parse_tests.c

OBJS = $(SRCS:.c=.o)
TESTS_OBJS = $(TESTS_SRCS:.c=.o)

RM = rm -f

CPPFLAGS += -Wall -Wextra -iquote ./include

CFLAGS += -fPIC -pedantic

LDFLAGS = -shared

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(NAME)

tests_run: $(TESTS_OBJS)
	$(CC) -lcriterion $(TESTS_OBJS) -o unit_tests
	./unit_tests
	$(RM) ./unit_tests
	$(RM) $(TESTS_OBJS)

val_run: CFLAGS += -g3
val_run: $(TESTS_OBJS)
	$(CC) -lcriterion $(TESTS_OBJS) -o unit_tests
	valgrind --trace-children=yes --quiet ./unit_tests
	$(RM) ./unit_tests
	$(RM) $(TESTS_OBJS)

debug: CFLAGS += -g3
debug: re

re: fclean all

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) $(NAME:.so=.a)

install: re
	@cp $(NAME) /usr/lib/$(NAME) 2> /dev/null || \
		printf "try sudo make install\n" && \
		cp include/parsy.h /usr/include/parsy.h 2> /dev/null && \
		printf "Library successfully installed !\n"

static: $(OBJS)
	$(ARCHIVER) rc $(NAME:.so=.a) $(OBJS)

.PHONY: all tests_run val_run debug clean fclean install static