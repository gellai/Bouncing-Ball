#
#   Simple Bouncing Ball game.
#   Written and tested under Linux.
#   'ncurses' library is required.
#
#   January 2019
#   
#   Compile:
#   --------
#	$ make build
#
#   Run:
#   ----
#	$ ./bouncingball
#
#   Clean:
#   ------
#	$ make clean
#

CC = gcc
LIBFLAG = -lncurses
EXEC = bouncingball

# .PHONY is to tell Makefile that 'build' is a command and not a file
.PHONY: build
build: 
	@$(CC) -o $(EXEC) bouncingball.c $(LIBFLAG)
	@echo All done!

.PHONY: clean
clean:
	@rm -f $(EXEC)
	@echo Clean done!