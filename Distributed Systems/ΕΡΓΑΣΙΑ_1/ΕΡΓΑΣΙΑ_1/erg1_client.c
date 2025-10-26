#include "erg1.h"
#include <stdio.h>
#include <stdlib.h>

void perform_computation(CLIENT *clnt, int choice, float a, int *array, int length) {
    int_vector vec;
    vec.array = array;
    vec.length = length;

    float_vector fvec;
    fvec.a = a;
    fvec.vector = vec;
    switch (choice) {
        case 1: {
            float *result = average_1(&vec, clnt);
            if (result == NULL) {
                fprintf(stderr, "Error in AVERAGE RPC call\n");
            } else {
                printf("Average: %f\n", *result);
            }
            break;
        }
        case 2: {
            intpair *result = max_min_1(&vec, clnt);
            if (result == NULL) {
                fprintf(stderr, "Error in MAX_MIN RPC call\n");
            } else {
                printf("Max: %d, Min: %d\n", result->a, result->b);
            }
            break;
        }
        case 3: {
            floatvec *result = multiply_1(&fvec, clnt);
            if (result == NULL) {
                fprintf(stderr, "Error in MULTIPLY RPC call\n");
            } else {
                printf("Product: ");
                for (int i = 0; i < result->length; i++) {
                    printf("%f ", result->array[i]);
                }
                printf("\n");
                free(result->array);
            }
            break;
        }
        default:
            fprintf(stderr, "Invalid choice\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_host>\n", argv[0]);
        exit(1);
    }
    char *server_host = argv[1];
    CLIENT *clnt = clnt_create(server_host, CALC_PROG, CALC_VERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(server_host);
        exit(1);
    }
     int choice;
    float a;
    int n;
    int *array;

    while (1) {
        printf("Enter computation type (1: Average, 2: Max/Min, 3: Multiply, 0: Exit): ");
        scanf("%d", &choice);

        if (choice == 0) break;

         printf("Enter length of vector: ");
        scanf("%d", &n);

        array = (int *) malloc(n * sizeof(int));
        printf("Enter elements of vector: ");
        for (int i = 0; i < n; i++) {
            scanf("%d", &array[i]);
        }

        if (choice == 3) {
            printf("Enter a real number a: ");
            scanf("%f", &a);
        }

        perform_computation(clnt, choice, a, array, n);

        free(array);
    }

    clnt_destroy(clnt);
    return 0;
}


