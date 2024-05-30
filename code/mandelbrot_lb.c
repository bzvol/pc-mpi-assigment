#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <mpi.h>

#define NODE_0 0

/* Computes the Mandelbrot set on [x1, x2] x [y1, y2]
 * at a m x n resolution. The value of picture[i, j] is
 * the number of iterations before bailout. */
void mandelbrot(int m, int n, double x1, double x2, double y1, double y2,
                int max_iter, int *picture, int start_row, int end_row)
{
    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double complex c = x1 + (x2 - x1) * j / n +
                               (y1 + (y2 - y1) * i / m) * I;
            double complex z = 0;
            int t = 0;

            while ((cabs(z) <= 2) && t < max_iter)
            {
                z = z * z + c;
                t++;
            }

            picture[i * n + j] = t;
        }
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 8)
    {
        if (rank == NODE_0)
            printf("Usage: %s M N MAX_ITER x1 x2 y1 y2\n"
                   "\tcreates an M x N pixel picture of [x1, x2] x [y1, y2]\n"
                   "\tescaping after MAX_ITER iterations\n",
                   argv[0]);

        MPI_Finalize();
        return EXIT_FAILURE;
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int max_iter = atoi(argv[3]);

    double x1 = atof(argv[4]);
    double x2 = atof(argv[5]);
    double y1 = atof(argv[6]);
    double y2 = atof(argv[7]);

    int *picture = malloc(m * n * sizeof(int));

    int rows_per_process = m / size;
    int remaining_rows = m % size;

    int start_row = rank * rows_per_process;

    if (rank < remaining_rows)
    {
        start_row += rank;
        rows_per_process++;
    }
    else
        start_row += remaining_rows;

    int end_row = start_row + rows_per_process;

    double start_time, end_time;
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    mandelbrot(m, n, x1, x2, y1, y2, max_iter, picture, start_row, end_row);

    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    double execution_time = end_time - start_time;

    double max_execution_time;
    MPI_Reduce(&execution_time, &max_execution_time, 1, MPI_DOUBLE,
               MPI_MAX, 0, MPI_COMM_WORLD);

    /* Compute flops */
    double local_flops = 8 * rows_per_process * n;
    for (int i = start_row; i < end_row; i++)
        for (int j = 0; j < n; j++)
            local_flops += 10 * picture[i * n + j];

    printf("RANK %d FLOPS: %lf\n", rank, local_flops);

    double max_flops, min_flops;
    MPI_Reduce(&local_flops, &max_flops, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_flops, &min_flops, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    // Load balance
    if (rank == NODE_0) {
        printf("MIN_FLOPS: %lf\n", min_flops);
        printf("MAX_FLOPS: %lf\n", max_flops);
        printf("LB: %lf\n", max_flops / min_flops);
    }

    free(picture);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
