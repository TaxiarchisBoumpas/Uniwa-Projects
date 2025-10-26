#include "erg1.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>

float *average_1_svc(int_vector *vec, struct svc_req *req) {
    static float result;
    int sum = 0;
    for (int i = 0; i < vec->length; i++) {
        sum += vec->array[i];
    }
    result = (float) sum / vec->length;
    return &result;
}
intpair *max_min_1_svc(int_vector *vec, struct svc_req *req) {
    static intpair result;
    result.a = result.b = vec->array[0];
    for (int i = 1; i < vec->length; i++) {
        if (vec->array[i] > result.a) result.a = vec->array[i];
        if (vec->array[i] < result.b) result.b = vec->array[i];
    }
    return &result;
}
floatvec *multiply_1_svc(float_vector *data, struct svc_req *req) {
    static floatvec result;
    result.length = data->vector.length;
    result.array = (float *) malloc(result.length * sizeof(float));
    for (int i = 0; i < result.length; i++) {
        result.array[i] = data->a * data->vector.array[i];
    }
    return &result;
}
int main() {
    register SVCXPRT *transp;

    pmap_unset(CALC_PROG, CALC_VERS);

    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL) {
        fprintf(stderr, "cannot create udp service.");
        exit(1);
    }
    if (!svc_register(transp, CALC_PROG, CALC_VERS, calc_prog_1, IPPROTO_UDP)) {
        fprintf(stderr, "unable to register (CALC_PROG, CALC_VERS, udp).");
        exit(1);
    }

    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
        fprintf(stderr, "cannot create tcp service.");
        exit(1);
    }
    if (!svc_register(transp, CALC_PROG, CALC_VERS, calc_prog_1, IPPROTO_TCP)) {
        fprintf(stderr, "unable to register (CALC_PROG, CALC_VERS, tcp).");
        exit(1);
    }

    svc_run();
    fprintf(stderr, "svc_run returned");
    exit(1);
}
