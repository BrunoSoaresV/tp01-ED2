all: main.o sequencial.o binaria.o arvoreB.o
	@gcc main.o sequencial.o binaria.o arvoreB.o -o pesquisa -g 
main.o: main.c
	@gcc main.c -c -Wall
sequencial.o: sequencial.c
	@gcc sequencial.c -c -Wall
binaria.o: binaria.c
	@gcc binaria.c -c -Wall
arvoreB.o: arvoreB.c
	@gcc arvoreB.c -c -Wall
run:
	@./pesquisa
