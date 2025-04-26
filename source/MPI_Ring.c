#define _CRT_SECURE_NO_WARNINGS
#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    int rank, size;
    char send_msg[100], recv_msg[100];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    sprintf(send_msg, "Hello from process %d", rank);

    // send to next and receive from previous
    MPI_Sendrecv(
        send_msg, strlen(send_msg) + 1, MPI_CHAR, next, 0,
        recv_msg, 100, MPI_CHAR, prev, 0,
        MPI_COMM_WORLD, &status
    );

    printf("Process %d received message: '%s'\n", rank, recv_msg);

    MPI_Finalize();
    return 0;
}
