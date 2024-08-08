#!/bin/bash

# Number of times to run the program
NUM_RUNS=10

# Path to your executable
PROGRAM_PATH="/home/tliuth/Documents/ED2/tp01-ED2/pesquisa"

# Static arguments
METODO=2
QUANTIDADE=100
SITUACAO=1
ECHO_MESSAGE="Running BINARIA with arguments "

# Function to generate a random number for the changing argument
generate_random_chave() {
  echo $(((RANDOM*40) % QUANTIDADE))  # Change 100000 to the range you need
}

# Run the program multiple times

# for ((i=0; i<NUM_RUNS; i++)); do
#   CHAVE=$(generate_random_chave)
#   echo "$ECHO_MESSAGE $METODO $QUANTIDADE $SITUACAO $CHAVE"
#   $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
# done

# QUANTIDADE=1000
# for ((i=0; i<NUM_RUNS; i++)); do
#   CHAVE=$(generate_random_chave)
#   echo "$ECHO_MESSAGE $METODO $QUANTIDADE $SITUACAO $CHAVE"
#   $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
# done

# QUANTIDADE=10000
# for ((i=0; i<NUM_RUNS; i++)); do
#   CHAVE=$(generate_random_chave)
#   echo "$ECHO_MESSAGE $METODO $QUANTIDADE $SITUACAO $CHAVE"
#   $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
# done


# SITUACAO=2
# QUANTIDADE=100
# for ((i=0; i<NUM_RUNS; i++)); do
#   CHAVE=$(generate_random_chave)
#   echo "$ECHO_MESSAGE $METODO $QUANTIDADE $SITUACAO $CHAVE"
#   $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
# done

# QUANTIDADE=1000
# for ((i=0; i<NUM_RUNS; i++)); do
#   CHAVE=$(generate_random_chave)
#   echo "$ECHO_MESSAGE $METODO $QUANTIDADE $SITUACAO $CHAVE"
#   $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
# done

# QUANTIDADE=10000
# for ((i=0; i<NUM_RUNS; i++)); do
#   CHAVE=$(generate_random_chave)
#   echo "$ECHO_MESSAGE $METODO $QUANTIDADE $SITUACAO $CHAVE"
#   $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
# done


SITUACAO=3
# QUANTIDADE=100
# for ((i=0; i<NUM_RUNS; i++)); do
#   CHAVE=$(generate_random_chave)
#   echo "$ECHO_MESSAGE $METODO $QUANTIDADE $SITUACAO $CHAVE"
#   $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
# done

# QUANTIDADE=1000
# for ((i=0; i<NUM_RUNS; i++)); do
#   CHAVE=$(generate_random_chave)
#   echo "$ECHO_MESSAGE $METODO $QUANTIDADE $SITUACAO $CHAVE"
#   $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
# done

# QUANTIDADE=10000
# for ((i=0; i<NUM_RUNS; i++)); do
#   CHAVE=$(generate_random_chave)
#   echo "$ECHO_MESSAGE $METODO $QUANTIDADE $SITUACAO $CHAVE"
#   $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
# done

QUANTIDADE=100000
for ((i=0; i<NUM_RUNS; i++)); do
  CHAVE=$(generate_random_chave)
  echo "Running $PROGRAM_PATH with arguments: $METODO $QUANTIDADE $SITUACAO $CHAVE"
  $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
done

QUANTIDADE=1000000
for ((i=0; i<NUM_RUNS; i++)); do
  CHAVE=$(generate_random_chave)
  echo "Running $PROGRAM_PATH with arguments: $METODO $QUANTIDADE $SITUACAO $CHAVE"
  $PROGRAM_PATH $METODO $QUANTIDADE $SITUACAO $CHAVE
done
