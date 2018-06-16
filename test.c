#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <malloc.h>

char *password_to_md5(char *password)
{
    char *md5 = (char *)malloc(33);
    unsigned char digest[16];
    int length = strlen(password) ;

    MD5((unsigned char *)password, length, (unsigned char *)&digest);

    int i;
    for (i = 0; i < 16; ++i)
    {
        sprintf(&md5[i * 2], "%02x", (unsigned int)digest[i]);
    }

    return md5;
}
int main()
{
    char *password = "linh";

    char *md5_in = "892da3d819056410c05bca7747d22735" ;
    char *md5 = password_to_md5(password);
    if (strcmp(md5_in, md5) == 0){
        printf("Findpass: %s", password);
        
    }
    printf("md5: %s\n", md5);
}
