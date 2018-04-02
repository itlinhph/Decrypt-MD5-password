#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <malloc.h>
#include <mpi.h>

#define NUM_CHARACTER 26
#define NLOOPS 3000

char *password_to_md5(char *password) {
    char *md5 = (char *)malloc(33);
    unsigned char digest[16];
    int length = strlen(password) ;

    MD5((unsigned char *)password, length, (unsigned char *)&digest);

    int i;
    for (i = 0; i < 16; ++i) {
        sprintf(&md5[i * 2], "%02x", (unsigned int)digest[i]);
    }

    return md5;
}

char *number_to_password(unsigned int num) {

    char character[26] =
        {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    
    char *password = (char *)malloc(sizeof(char) * 20);
    int indexChar[20] ;

    int i = 0, j ;
    unsigned int temp ;
    while(num !=0) {
        temp = num%26 ;
        indexChar[i] = temp;
        i++ ;
        num = num/26 ;
    }
    int k=0 ;
    for(j=i-1 ; j>=0; j--, k++ ) {
        password[k] = character[ indexChar[j]-1] ;
    }
    password[i] = '\0' ;
    // printf("%s", password) ;

    return password;

}

int main() {

    
    char *md5_input = (char *) malloc (sizeof(char)*33) ;
    char *md5_password ;
    char *current_password ;
    // char *password = (char *) malloc (sizeof(char)*20) ;

    int rank, ntasks;
    unsigned int i, j ;

    MPI_Status status ;
    int is_find = 0 ;
    int sum_is_find = 0 ;

    MPI_Init(NULL, NULL) ;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks) ;

    int count, start, stop, remain ;
    int  errorcode ;

    unsigned int current = 0 ; // dem so pass da do duoc.

    if(rank== 0) {

        printf("\n ------- MD5 DECODE BY LINHPHAN -------\n") ;
        printf("YOUR MD5 CODE IS: ") ;
        scanf("%s", md5_input) ;
        for (j = 1; j < ntasks; j++) {
            MPI_Send(md5_input,33, MPI_CHAR, j, 102, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(md5_input, 33, MPI_CHAR, 0, 102, MPI_COMM_WORLD, &status);
    }
    while(!is_find) {

        /* Barrier */
        MPI_Barrier(MPI_COMM_WORLD) ;

        count = NLOOPS/ ntasks ; 
        start = current + rank*count ;
        stop = start + count ;

        for(i = start; i<stop; i++) {
            current_password = number_to_password(i) ;
            
            md5_password = password_to_md5(current_password) ;
            if(strcmp(md5_input, md5_password) == 0) {
                printf("Your password is: %s\n", current_password) ;
                is_find = 1 ;
            }
            free(current_password);
            free(md5_password) ;
        }

        if(rank==0) { // master caculate remaining password.
            remain = current + count*ntasks ;
            for(i=remain; i<current + NLOOPS; i++) {
                current_password = number_to_password(i);
                md5_password = password_to_md5(current_password);
                if (strcmp(md5_input, md5_password) == 0) {
                    printf("Your password is: %s", current_password);
                    is_find = 1;
                }
                free(current_password);
                free(md5_password) ;
            }
            

        }
        current += NLOOPS ;
        // printf("Loops: %d in rank %d \n", current, rank) ;

        MPI_Barrier(MPI_COMM_WORLD) ;
        /* BARRIER */

        MPI_Reduce(&is_find, &sum_is_find, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD ) ;
        is_find = sum_is_find ;
        
        if(rank==0) {
            for (j = 1; j < ntasks; j++) {
                MPI_Send(&is_find, 1, MPI_INT, j, 101, MPI_COMM_WORLD);
            }
        }
        else {
            MPI_Recv(&is_find, 1, MPI_INT, 0, 101, MPI_COMM_WORLD, &status);
        }

    }
    free(md5_input) ;

    MPI_Finalize() ;

    return 0;
}