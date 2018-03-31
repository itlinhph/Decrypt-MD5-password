#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <malloc.h>
// #include <mpi.h>

char *passwordToMd5(char *password, int length) {
    char *md5 = (char*)malloc(33);
    unsigned char digest[16];
    
    MD5((unsigned char *)password, length, (unsigned char *)&digest);

    int i;
    for (i = 0; i < 16; ++i) {
        sprintf(&md5[i * 2], "%02x", (unsigned int)digest[i]);
    }

    return md5;
}

    int main(int argc, char **argv) {

        char character[26] = 
        {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'
        ,'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't'
        , 'u', 'v','w', 'x', 'y', 'z' } ;
        
        char* password = "linh" ;
        char *md5 = passwordToMd5(password, strlen(password));
        printf("%s\n", md5);
        free(md5);
        return 0;
    }