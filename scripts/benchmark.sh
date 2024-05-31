#!/bin/bash

CODE_FILE="code/mandelbrot_load_bal.c"
RESULTS_CSV_FILE="mandelbrot.csv"

CNCZ_DIR="/home/bzvolenszki/parallel_programs"
CNCZ_BENCHMARK="bench_mpi.sh"
CNCZ_COLLECTOR="collect_mpi_load_bal.sh"

N_RUNS=10
NODES=8
MANDELBROT_OPTIONS="5000 5000 100 -2 2 -2 2 2>/dev/null"

GET_NJOBS="squeue -u bzvolenszki -h -t pending,running -r | wc -l"

# Connect to VPN
nmcli c up "Science VPNsec"

# Copy code to CNCZ
scp "$CODE_FILE" "cncz:$CNCZ_DIR/src/mpi/mandelbrot.c"

# Run benchmark on CNCZ
SSH_CMDS=$(cat << EOF
set -e

source .bashrc
cd "$CNCZ_DIR"

rm -f results/*
rm -f mandelbrot_*

echo "Running benchmark..."
./$CNCZ_BENCHMARK bin/mandelbrot_mpi $N_RUNS results mandelbrot $NODES $MANDELBROT_OPTIONS

echo "Waiting for jobs to finish..."
n_jobs=\$($GET_NJOBS)
while [ \$n_jobs -gt 0 ]; do
    echo "\$n_jobs jobs still running..."
    sleep 5
    n_jobs=\$($GET_NJOBS)
done

echo "Collecting results..."
./$CNCZ_COLLECTOR results mandelbrot $NODES
EOF
)

ssh cncz "$SSH_CMDS"

# Copy results from CNCZ
scp "cncz:$CNCZ_DIR/results/mandelbrot.csv" "./$RESULTS_CSV_FILE"

# Disconnect from VPN
nmcli c down "Science VPNsec"
