#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <malloc.h>
#include <mpi.h>

int find_pass (int number, char *md5_input) {

    int is_find = 0 ; 
    char character[36] = {'0', 'n', 'a', 'o', 'h', 'i', 'u', 'g', 't', 'c', 'e', 'd', 'm', 'y', 'l', 'r', 'b', 'v', 's', 'k', 'p', 'x', 'q', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'f', 'j', 'w', 'z'};
    char *password = (char *)malloc(sizeof(char) * 20);
    int index_char[20];

    int i = 0, j, k=0 ;
    unsigned int temp;
    while (number != 0) {
        temp = number % 36;
        index_char[i] = temp;
        i++;
        number = number / 36;
    }

    for (j = i - 1; j >= 0; j--, k++)
        password[k] = character[index_char[j]];
    password[i] = '\0';

    char *md5 = (char *)malloc(33);
    unsigned char digest[16];

    MD5((unsigned char *)password, strlen(password), (unsigned char *)&digest);

    for (i = 0; i < 16; ++i)
        sprintf(&md5[i * 2], "%02x", (unsigned int)digest[i]);

    if (strcmp(md5_input, md5) == 0) {
        printf("YOUR PASSWORD: %s\n", password);
        is_find = 1; 
    }
    free(password);
    free(md5);
    return is_find ;
}

int main() {

    char *md5_input = (char *) malloc (sizeof(char)*33) ;
    int rank, ntasks;
    unsigned int i, j ;

    MPI_Status status ;
    int is_find = 0 , sum_is_find = 0 ; 

    MPI_Init(NULL, NULL) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks) ;

    unsigned int start, end;
    int count, remain ; 
    double start_time, end_time ;
    unsigned int current = 0 ; 

    if (rank == 0) {
        count = 1000 / ntasks;
        printf("\n-------MD5 DECRYPT BY LINHPHAN------\n\n ");
        printf("YOUR MD5 CODE IS: ");
        scanf("%s", md5_input);
        for (j = 1; j < ntasks; j++)
            MPI_Send(md5_input, 33, MPI_CHAR, j, 102, MPI_COMM_WORLD);
    }
    else
        MPI_Recv(md5_input, 33, MPI_CHAR, 0, 102, MPI_COMM_WORLD, &status);

    while(!is_find) {

        count = 1000/ ntasks ; 
        start = current + rank*count ;
        end = start + count ;

        for(i = start; i<end; i++)
            is_find += find_pass(i, md5_input) ;

        current += 1000 ;
        
        MPI_Reduce(&is_find, &sum_is_find, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD ) ;
        is_find = sum_is_find ;

        if (rank == 0) {
            for (j = 1; j < ntasks; j++)
                MPI_Send(&is_find, 1, MPI_INT, j, 101, MPI_COMM_WORLD);
        }
        else
            MPI_Recv(&is_find, 1, MPI_INT, 0, 101, MPI_COMM_WORLD, &status);
    }
    free(md5_input) ;
    MPI_Finalize() ;

    return 0;
}
