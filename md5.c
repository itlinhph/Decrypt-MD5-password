#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <mpi.h>

int main()
{
    int rank, ntasks ;
    int length;
    unsigned long i, j ;

    MPI_Status status ;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    unsigned char digest[MD5_DIGEST_LENGTH];
    char password[] = "linh";
    // if(rank == 0) {
    //     printf("Nhap ma md5: ") ;
    //     // scanf
    // }

    MD5((unsigned char *)&password, strlen(password), (unsigned char *)&digest);

    char md5String[33];
    // for(i=0; i<strlen(digest); i++ ) {
    //     printf("%d", digest[i]) ;
    // }
    printf("\n");
    for (int i = 0; i < 16; i++)
        sprintf(&md5String[i * 2], "%02x", (unsigned int)digest[i]);

    printf("%s\n", md5String);
    printf("\n%d", strlen(md5String)) ;

    MPI_Finalize() ;
    return 0;
}