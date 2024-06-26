#!/bin/bash

# Number of times to run the program
NUM_RUNS=10

# Path to your executable
PROGRAM_PATH="/home/tliuth/Documents/ED2/tp01-ED2/pesquisa"

# Static arguments
METODO=1
QUANTIDADE=1000000
SITUACAO=1

# Function to generate a random number for the changing argument
generate_random_chave() {
  echo $(((RANDOM*30) % QUANTIDADE))  # Change 10000 to the range you need
}

# Run the program multiple times

for ((i=0; i<NUM_RUNS; i++)); do
  CHAVE=$(generate_random_chave)
  echo "Running $PROGRAM_PATH with arguments: $METODO $QUANTIDADE $SITUACAO $CHAVE"
  $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
done


