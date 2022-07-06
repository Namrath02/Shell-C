OBJS	= main.o commands.o parse.o signal_handler.o pipes.o jobs.o fg_bg.o
SOURCE	= main.c commands.c parse.c signal_handler.c pipes.c jobs.c fg_bg.c
HEADER	= headerfiles.h
OUT	= a.out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

commands.o: commands.c
	$(CC) $(FLAGS) commands.c 

parse.o: parse.c
	$(CC) $(FLAGS) parse.c 

signal_handler.o: signal_handler.c
	$(CC) $(FLAGS) signal_handler.c

pipes.o: pipes.c
	$(CC) $(FLAGS) pipes.c

jobs.o: jobs.c
	$(CC) $(FLAGS) jobs.c

fg_bg.o: fg_bg.c
	$(CC) $(FLAGS) fg_bg.c
	
clean:
	rm -f $(OBJS) $(OUT)