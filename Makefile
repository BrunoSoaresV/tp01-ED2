all: main.o sequencial.o
	@gcc main.o sequencial.o -o pesquisa
main.o: main.c
	@gcc main.c -c -Wall
sequencial.o: sequencial.c
	@gcc sequencial.c -c -Wall
run:
	@./pesquisa