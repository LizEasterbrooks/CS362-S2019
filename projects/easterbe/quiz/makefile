# These two resources were essential for understanding rules for execution and phony targets:
# https://stackoverflow.com/questions/15566405/run-executable-from-makefile
# https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html

default: run

run: prog
	./prog

prog:
	gcc -Wall -std=c99 -coverage -o prog testme.c
		
clean:
	rm prog

cleanall: 
	rm prog 

.PHONY: default run