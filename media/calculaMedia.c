#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For access()

#define MAX_LINE_LENGTH 256

typedef struct {
    long chave;
    long leituras;
    long escritas;
    long transf_t;
    long comparacoes;
    double tempo_total;
} Record;

void calculaMedia(const char* filePath, FILE* outputFile) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filePath);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    // Variables to store sums
    long sumLeituras = 0;
    long sumEscritas = 0;
    long sumTransfT = 0;
    long sumComparacoes = 0;
    double sumTempoTotal = 0.0;

    // Read header line
    fgets(line, MAX_LINE_LENGTH, file);

    // Read data lines
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        Record rec;
        sscanf(line, "%ld | %ld | %ld | %ld | %ld | %lf",
               &rec.chave,
               &rec.leituras,
               &rec.escritas,
               &rec.transf_t,
               &rec.comparacoes,
               &rec.tempo_total);

        sumLeituras += rec.leituras;
        sumEscritas += rec.escritas;
        sumTransfT += rec.transf_t;
        sumComparacoes += rec.comparacoes;
        sumTempoTotal += rec.tempo_total;
        count++;
    }

    if (count > 0) {
        fprintf(outputFile, "Médias para %s:\n", filePath);
        fprintf(outputFile, "Leituras: %.2f\n", (double)sumLeituras / count);
        fprintf(outputFile, "Escritas: %.2f\n", (double)sumEscritas / count);
        fprintf(outputFile, "Transf T: %.2f\n", (double)sumTransfT / count);
        fprintf(outputFile, "Comparações: %.2f\n", (double)sumComparacoes / count);
        fprintf(outputFile, "Tempo_Total: %.6f\n\n", sumTempoTotal / count);
        printf("Médias calculadas e salvas no arquivo de saída.\n");
    } else {
        printf("Nenhum dado encontrado no arquivo %s.\n", filePath);
    }

    fclose(file);
}


int main() {
    FILE* outputFile = fopen("averages.txt", "a");
    if (outputFile == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        return 1;
    }
    const char* filePath = "/home/tliuth/Documents/ED2/tp01-ED2/contagens/arvB/UmMilhao/O_Busca";
    calculaMedia(filePath, outputFile);
    return 0;
}