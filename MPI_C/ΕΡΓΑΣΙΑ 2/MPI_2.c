#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int N, local_N, i, j;
    int isDiagonallyDominant, minRow, minCol;
    double *A, *local_A, *B; 
    double *global_B = NULL;
    double diagonalElement, sum, maxDiagonalElement, globalMaxDiagonalElement, minElement;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("Please give the size of array: \n");
		scanf("%d" , &N);
    }

    // Broadcasting the size to all processes
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Checking if the number of processors size is a divisor of N
    if (N % size != 0)
    {
        if (rank == 0)
        {
            printf("The number of processors must divide the size of array.\n");
        }
		MPI_Finalize();
        return 1;
    }

    local_N = N / size;
    A = (double *)malloc(local_N * N * sizeof(double));
    local_A = (double *)malloc(local_N * N * sizeof(double));
    B = (double *)malloc(local_N * N * sizeof(double));

    if (rank == 0)
    {
        // Processor 0 reads the matrix A
        int cnt=0;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                printf("Give the No.%d number:  ",cnt);
                scanf("%lf", &A[i * N + j]);
                printf("\n");
                cnt++;
            }
        }
    }

    // Broadcasting matrix A to all processes
    MPI_Bcast(A, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter the matrix A to all processes
    MPI_Scatter(A, local_N * N, MPI_DOUBLE, local_A, local_N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    isDiagonallyDominant = 1;
    for (i = 0; i < local_N; i++)
    {
		diagonalElement = (local_A[i * N + i]);
		if (diagonalElement < 0 )
		{
			diagonalElement = diagonalElement * (-1) ;
		}
          sum = 0.0;
        for (j = 0; j < N; j++)
        {
            if (j != i)
            {
                
		  sum += (local_A[i * N + j]);
		  if (sum < 0 )
		    {
		      sum = sum * (-1) ;
		    }
            }
        }
		
        if (diagonalElement <= sum)
        {
            isDiagonallyDominant = 0;
            break;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&isDiagonallyDominant, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (isDiagonallyDominant == 1)
    {
        maxDiagonalElement = 0.0;
        for (i = 0; i < N; i++)
        {
          for (j=0; j < N; j++)
          {
           if (i == j)
           {
             if (local_A[i,j] > maxDiagonalElement)
             {
              maxDiagonalElement = local_A[i,j];
             }
            }
          }
        }

        MPI_Allreduce(&maxDiagonalElement, &globalMaxDiagonalElement, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

        for (i = 0; i < local_N; i++)
        {
            for (j = 0; j < N; j++)
            {
                B[i * N + j] = globalMaxDiagonalElement - fabs(local_A[i * N + j]);
            }
        }

        global_B = (double *)malloc(N * N * sizeof(double));
        MPI_Gather(B, local_N * N, MPI_DOUBLE, global_B, local_N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		
        if (rank == 0)
        {
			

            minElement = global_B[0];
            minRow = 0;
            minCol = 0;
            for (i = 0; i < N; i++)
            {
                for (j = 0; j < N; j++)
                {
                    if (global_B[i * N + j] < minElement)
                    {
                        minElement = global_B[i * N + j];
                        minRow = i;
                        minCol = j;
                    }
                }
            }

            printf("YES! Matrix A is strictly diagonally dominant.\n");
            printf("The maximum absolute element on the diagonal is: %lf\n", globalMaxDiagonalElement);
            printf("Matrix B:\n");
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                    printf("%lf\t", global_B[i * N + j]);
                printf("\n");
            }
            printf("The minimum element of matrix B is: %lf\n", minElement);
            printf("It is located at position (%d, %d)\n", minRow, minCol);
        }

        
    }else
	{
        if (rank == 0)
		{
			printf("NO! Matrix A is NOT strictly diagonally dominant.\n");
		}
	}
	
    free(global_B);
    free(A);
    free(local_A);
    free(B);
	for(i=0; i<rank; i++){
		MPI_Finalize();
	}

    return 0;
}

