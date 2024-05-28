all: main.o sequencial.o binaria.o
	@gcc main.o sequencial.o binaria.o -o pesquisa
main.o: main.c
	@gcc main.c -c -Wall
sequencial.o: sequencial.c
	@gcc sequencial.c -c -Wall
binaria.o: binaria.c
	@gcc binaria.c -c -Wall
run:
	@./pesquisa
