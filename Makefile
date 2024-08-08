all: main.o sequencial.o binaria.o arvoreB.o arvoreBestrela.o
	@gcc main.o sequencial.o binaria.o arvoreB.o arvoreBestrela.o -o pesquisa -g 
main.o: main.c
	@gcc main.c -c -Wall
sequencial.o: sequencial.c
	@gcc sequencial.c -c -Wall
binaria.o: binaria.c
	@gcc binaria.c -c -Wall
arvoreB.o: arvoreB.c
	@gcc arvoreB.c -c -Wall
arvoreBestrela.o: arvoreBestrela.c
	@gcc arvoreBestrela.c -c -Wall
run:
	@./pesquisa
