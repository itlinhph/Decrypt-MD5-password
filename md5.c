#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <malloc.h>
#include <mpi.h>

#define NUM_CHARACTER 26
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

    
    // char *password = "linh";
    // char *md5 = password_to_md5(password);
    // printf("md5: %s\n", md5);
    // free(md5);
    // char* c= number_to_password(30) ;
    // printf("pass: %s", c) ;

    char *md5_input = "892da3d819056410c05bca7747d22735";
    char *md5_password ;
    int rank, ntasks;
    unsigned int i, j ;

    MPI_Status status ;
    int finded_pass = 0 ;

    MPI_Init(NULL, NULL) ;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks) ;

    int count, start, stop, remain ;
    int  errorcode ;
    int Nloops = 1000 ; // cu tinh duoc 1000 vong lap thi kiem tra xem da tim ra pass chua.
    unsigned int current = 0 ; // dem so pass da do duoc.
    char *current_password ;
    while(!finded_pass) {
        count = Nloops/ ntasks ; 
        start = current + rank*count ;
        stop = start + count ;

        for(i = start; i<stop; i++) {
            current_password = number_to_password(i) ;
            
            md5_password = password_to_md5(current_password) ;
            if(strcmp(md5_input, md5_password) == 0) {
                printf("Findpasssssssssssss: %s in rank %d", current_password, rank) ;
                finded_pass = 1 ;
                for(j=0; j<ntasks; j++) {
                    MPI_Send(&finded_pass, 1, MPI_INT, 0, 102, MPI_COMM_WORLD) ;
                }
                // MPI_Bcast(&finded_pass, 1, MPI_INT, rank, MPI_COMM_WORLD) ;
            }
            free(current_password);
        }

        if(rank==0) { // master tinh so mat khau con lai
            remain = current + count*ntasks ;
            for(i=remain; i<current + Nloops; i++) {
                current_password = number_to_password(i);
                md5_password = password_to_md5(current_password);
                if (strcmp(md5_input, md5_password) == 0) {
                    printf("Findpassssssssssssssssssssssssssss: %s", current_password);
                    finded_pass = 1;
                    MPI_Send(&finded_pass, 1, MPI_INT, 0, 102, MPI_COMM_WORLD);
                }
                free(current_password);
            }
            MPI_Recv(&finded_pass, 1, MPI_INT, MPI_ANY_SOURCE, 102, MPI_COMM_WORLD, &status);
            for (j = 1; j < ntasks; j++) {
                MPI_Send(&finded_pass, 1, MPI_INT, j, 101, MPI_COMM_WORLD);
            }

        }
        current += Nloops ;
        printf("Loops: %d in rank %d \n", current, rank) ;
        MPI_Recv(&finded_pass,1, MPI_INT, 0, 101, MPI_COMM_WORLD, &status) ;
    }
    // free(current_password);
    // free(md5_password) ;

    MPI_Finalize() ;

    return 0;
}