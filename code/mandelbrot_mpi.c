#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <mpi.h>

#define NODE_0 0

/* Computes the Mandelbrot set on [x1, x2] x [y1, y2]
 * at a m x n resolution. The value of picture[i, j] is
 * the number of iterations before bailout. */
void mandelbrot(int m, int n, double x1, double x2, double y1, double y2,
                int max_iter, int *picture, int size, int rank)
{
    for (int i = rank; i < m; i += size)
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

    int n_rows = 0;
    for (int i = rank; i < m; i += size)
        n_rows++;

    int *local_picture = malloc(m * n * sizeof(int));

    double start_time, end_time;
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    mandelbrot(m, n, x1, x2, y1, y2, max_iter, local_picture, size, rank);

    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    double execution_time = end_time - start_time;

    double max_execution_time;
    MPI_Reduce(&execution_time, &max_execution_time, 1, MPI_DOUBLE,
               MPI_MAX, 0, MPI_COMM_WORLD);

    int local_count = 0;
    for (int i = rank; i < m; i += size)
        for (int j = 0; j < n; j++)
            if (local_picture[i * n + j] == max_iter)
                local_count++;

    int total_count;
    MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == NODE_0)
    {
        double p = (double)total_count / (m * n);
        fprintf(stderr, "%lf\n", p);
    }

    double local_flops = 8 * n_rows * n;
    for (int i = rank; i < m; i += size)
        for (int j = 0; j < n; j++)
            local_flops += 10 * local_picture[i * n + j];

    double total_flops;
    MPI_Reduce(&local_flops, &total_flops, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == NODE_0)
        printf("%lf\n", total_flops / 1e9 / max_execution_time);

    free(local_picture);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
