#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define FIRST 0

int main(int argc, char **argv)
{
    int rank, size, choice, i;
    int *X;
    int n;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    do
    {
        // Μόνο ο πρώτος επεξεργαστής λαμβάνει είσοδο από το χρήστη
        if (rank == FIRST)
        {
            printf("\nMenu:\n");
            printf("1. Υπολογισμός\n");
            printf("2. Έξοδος\n");
            printf("Επιλέξτε λειτουργία: ");
            scanf("%d", &choice);

            // Αποστολή της επιλογής σε όλες τις διεργασίες
            for (int dest = 1; dest < size; dest++) {
                MPI_Send(&choice, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
        } else {
            // Κάθε άλλη διεργασία λαμβάνει την επιλογή από τον πρώτο
            MPI_Recv(&choice, 1, MPI_INT, FIRST, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Ελέγχουμε αν ο χρήστης επέλεξε να εκτελέσει το πρόγραμμα
        if (choice == 2)
            break;

        // Εκτέλεση του υπόλοιπου κώδικα μόνο αν ο χρήστης επέλεξε τη λειτουργία 1
        if (choice == 1)
        {
            if (rank == FIRST)
            {
                printf("Δώστε το μήκος του διανύσματος Χ: ");
                scanf("%d", &n);

                // Αρχικοποίηση του διανύσματος X στον κεντρικό επεξεργαστή
                X = (int *)malloc(n * sizeof(int));
                printf("Δώστε τα στοιχεία του διανύσματος Χ:\n");
                for (i = 0; i < n; i++)
                {
                    printf("X[%d]: ", i);
                    scanf("%d", &X[i]);
                }
                printf("Το διάνυσμα X είναι:\n");
                for (i = 0; i < n; i++)
                {
                    printf("%d ", X[i]);
                }
                printf("\n");

                // Αποστολή του μήκους του διανύσματος X σε όλους τους επεξεργαστές
                for (int dest = 1; dest < size; dest++)
                {
                    MPI_Send(&n, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
                }

                // Αποστολή των στοιχείων του διανύσματος X σε όλους τους επεξεργαστές
                for (int dest = 1; dest < size; dest++)
                {
                    MPI_Send(X, n, MPI_INT, dest, 0, MPI_COMM_WORLD);
                }
            } else {
                // Κάθε επεξεργαστής λαμβάνει το μήκος του διανύσματος X
                MPI_Recv(&n, 1, MPI_INT, FIRST, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // Κάθε επεξεργαστής λαμβάνει το διάνυσμα X
                int *local = (int *)malloc(n * sizeof(int));
                MPI_Recv(local, n, MPI_INT, FIRST, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // Οι υπολογισμοί σε κάθε επεξεργαστή

                // α) Υπολογισμός μέσης τιμής
                double sum = 0, mo;
                for (i = 0; i < n; i++)
                {
                    sum = sum + local[i];
                }

                mo = sum / n;
                MPI_Send(&mo, 1, MPI_DOUBLE, FIRST, 0, MPI_COMM_WORLD);

                // β) Υπολογισμός της μέγιστης τιμής
                int lmax = local[0];
                for (i = 0; i < n; i++)
                {
                    if (local[i] > lmax)
                    {
                        lmax = local[i];
                    }
                }
                MPI_Send(&lmax, 1, MPI_INT, FIRST, 0, MPI_COMM_WORLD);

                // γ) Υπολογισμός της διασποράς
                double var = 0;
                for (i = 0; i < n; i++)
                {
                    var += pow(local[i] - mo, 2) ;
                }
                var = var / n;
                MPI_Send(&var, 1, MPI_DOUBLE, FIRST, 0, MPI_COMM_WORLD);


                // δ) Υπολογισμός του νέου διανύσματος Δ
                int *d = (int *)malloc(n * sizeof(int));
                
                // Αρχικοποίηση του διανύσματος d σε μηδενικές τιμές
                for (i = 0; i < n; i++)
                {
                  d[i] = 0;
                }
                
                int ap;
                // Υπολογισμός του νέου διανύσματος Δ
                for (i = 0; i < n; i++)
                {
                  ap = local[i] - lmax;
                  //ap = fabs(local[i] - lmax);
                  if (ap < 0)
                  {
                    ap = ap * (-1);
                  }
                  d[i] = ap * ap;
                }
                // Τώρα αποστέλλω το νέο διάνυσμα στον κεντρικό επεξεργαστή
                MPI_Send(d, n, MPI_INT, FIRST, 0, MPI_COMM_WORLD);

                free(local);
                free(d);
            }

            if (rank == FIRST)
            {
                // Τώρα θα συλλέξω όλα τα αποτελέσματα από τους επεξεργαστές
                int gmax;
                int *gd, i; 
                double lmo, lvar;
                int lmax;
                
               
                
                gd = (int *)malloc(n * sizeof(int));

                for (int source = 1; source < size; source++)
                {
                    MPI_Recv(&lmo, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Recv(&lmax, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Recv(&lvar, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Recv(gd, n, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					gmax = lmax;
               
                }
                
               
                
                
                double mo = lmo;
                double v = lvar;

                printf("\nΑΠΟΤΕΛΕΣΜΑΤΑ\n");
                printf("1) Μέση τιμή: %g\n", mo);
                printf("2) Μέγιστη τιμή: %d\n", gmax);
                printf("3) Διασπορά: %g\n", v);
                printf("4) Νέο Διάνυσμα:\n");
                for (i = 0; i < n; i++)
                {
                    printf("%d ", gd[i]);
                }
                printf("\n");

                free(gd);
                
            }

            // Αποστολή σήματος ολοκλήρωσης από τις υπόλοιπες διεργασίες
            if (rank != FIRST)
            {
                MPI_Send(&i, 0, MPI_INT, FIRST, 0, MPI_COMM_WORLD);
            }
        }

    }
   
  while (1);

    if (rank == FIRST)
    {
        free(X);
    }

    MPI_Finalize();
    return 0;
}
