#!/bin/bash
N_VALORES=(1000 2000 4000)
HILOS=(1 2 4 8 12 16)
REPETICIONES=3

echo "==========================================================="
echo "      PRUEBAS AUTOMÁTICAS: MULTIPLICACIÓN MATRIZ-VECTOR    "
echo "==========================================================="

for N in "${N_VALORES[@]}"; do
    echo -e "\n-----------------------------------------------------------"
    echo " TAMAÑO DE MATRIZ N = $N (N x N)"
    echo "-----------------------------------------------------------"

    MIN_TIEMPO=999999
    MEJOR_SALIDA=""
    for ((i=1; i<=REPETICIONES; i++)); do
        SALIDA=$(./matriz_secuencial $N)
        TIEMPO=$(echo $SALIDA | awk '{print $2}')
        if awk "BEGIN {exit !($TIEMPO < $MIN_TIEMPO)}"; then
            MIN_TIEMPO=$TIEMPO
            MEJOR_SALIDA=$SALIDA
        fi
    done
    echo "Secuencial          -> $MEJOR_SALIDA"

    for T in "${HILOS[@]}"; do
        MIN_TIEMPO=999999
        MEJOR_SALIDA=""
        for ((i=1; i<=REPETICIONES; i++)); do
            SALIDA=$(./matriz_pthreads $N $T)
            TIEMPO=$(echo $SALIDA | awk '{print $2}')
            if awk "BEGIN {exit !($TIEMPO < $MIN_TIEMPO)}"; then
                MIN_TIEMPO=$TIEMPO
                MEJOR_SALIDA=$SALIDA
            fi
        done
        echo "Pthreads ($T hilos) -> $MEJOR_SALIDA"
    done
done