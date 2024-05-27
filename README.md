# Parallel Computing

## Assignment 2 - MPI

Run this:

1. Copy `mandelbrot_mpi.c` to `src/mpi/mandelbrot.c` to CNCZ in the parallel_programs folder/repo
2. On CNCZ, run `./bench_mpi.sh bin/mandelbrot_mpi N_RUNS results mandelbrot NODES M N MAX_ITER X1 X2 Y1 Y2`
3. Wait for slurm to finish (`squeue --me` to check)
4. Run `./collect_mpi.sh results mandelbrot NODES` to make the csv
5. Copy this csv to your local machine
6. Run `python3 plotter.py` to generate the benchmark graph - *ensure that the csv is named `mandelbrot.csv` (and is in the directory where `plotter.py` is)*

I used N_RUNS=10, NODES=8, M,N=1000, MAX_ITER=100, X1-X2,Y1-Y2=(-2;2). With these parameters, it took about 20-30 secs for slurm to finish. Maybe try with more nodes or larger image size.

For me, by making larger images, the execution time incremented exponentially. For a 5000x5000 image, it took ~30s, for a 20.000x20.000 image it was 6-7 mins **for one run**.
