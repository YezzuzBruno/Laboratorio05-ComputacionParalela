#!/bin/bash

N_VALORES=(1000000 10000000 100000000)

HILOS=(1 2 4 8 12 16)

REPETICIONES=3

echo "==========================================================="
echo "          PRUEBAS AUTOMÁTICAS DE CÁLCULO DE PI             "
echo "==========================================================="

for N in "${N_VALORES[@]}"; do
    echo ""
    echo "-----------------------------------------------------------"
    echo " TAMAÑO DEL PROBLEMA N = $N"
    echo "-----------------------------------------------------------"

    MIN_TIEMPO=999999
    MEJOR_SALIDA=""
    for ((i=1; i<=REPETICIONES; i++)); do
        SALIDA=$(./pi_secuencial $N)
        TIEMPO=$(echo $SALIDA | awk '{print $2}')
        
        if (( $(echo "$TIEMPO < $MIN_TIEMPO" | bc -l) )); then
            MIN_TIEMPO=$TIEMPO
            MEJOR_SALIDA=$SALIDA
        fi
    done
    echo "Secuencial          -> $MEJOR_SALIDA"

    for T in "${HILOS[@]}"; do
        MIN_TIEMPO=999999
        MEJOR_SALIDA=""
        for ((i=1; i<=REPETICIONES; i++)); do
            SALIDA=$(./pi_pthreads $N $T)
            TIEMPO=$(echo $SALIDA | awk '{print $2}')
            
            if (( $(echo "$TIEMPO < $MIN_TIEMPO" | bc -l) )); then
                MIN_TIEMPO=$TIEMPO
                MEJOR_SALIDA=$SALIDA
            fi
        done
        echo "Pthreads ($T hilos)\t-> $MEJOR_SALIDA"
    done
done

echo ""
echo "==========================================================="
echo "PRUEBAS TERMINADAS."