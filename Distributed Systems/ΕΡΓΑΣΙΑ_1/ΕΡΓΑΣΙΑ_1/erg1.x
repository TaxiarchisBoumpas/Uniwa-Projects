struct int_vector {
    int *array;
    int length;
};

struct float_vector {
    float a;
    struct int_vector vector;
};

struct intpair {
    int a;
    int b;
};

struct floatvec {
    float *array;
    int length;
};

program CALC_PROG {
    version CALC_VERS {
        float AVERAGE(int_vector) = 1;
        intpair MAX_MIN(int_vector) = 2;
        floatvec MULTIPLY(float_vector) = 3;
    } = 1;
} = 0x20000001;
