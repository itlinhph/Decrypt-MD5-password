#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <malloc.h>
// #include <mpi.h>

char md5String[33] ;
char pwd[] = "linh" ;
char *passwordToMD5 (char *password, int length) {

    char *md5String = (char *) malloc (sizeof(char)*32) ;
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char *)&password, length, (unsigned char *)&digest);
    
    for (int i = 0; i < 16; i++)
        sprintf(&md5String[i * 2], "%02x", (unsigned int)digest[i]);
    
    printf("pass: %s, md5: %s\n---\n", password, md5String) ;
    return md5String ;
    
}

void pwdToMD5 (char pwd[], int length) {
    unsigned char digest[MD5_DIGEST_LENGTH];

    printf("pwd: %s", pwd) ;
    MD5((unsigned char *)&pwd, length, (unsigned char *)&digest);
    // char md5String[33];
    for (int i = 0; i < 16; i++)
        sprintf(&md5String[i * 2], "%02x", (unsigned int)digest[i]);
    printf("md5string: %s\n", md5String);
}
int main()
{
    // int rank, ntasks ;
    // int length;
    // unsigned long i, j ;

    // MPI_Status status ;

    // MPI_Init(NULL, NULL);

    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &ntasks);


    // MPI_Finalize() ;

    

    char *md5 ;
    // char pwd[] = "linh" ;
    md5 = passwordToMD5(pwd, strlen(pwd)) ;
    printf("%s\n", md5) ;
    pwdToMD5(pwd , strlen(pwd)) ;
    printf("md5String: %s", md5String) ;
    return 0;
}