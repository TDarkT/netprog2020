#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct PAK {
    char header[5];
    unsigned short length;
    char data[1024];
};

struct PAK decodePak(char *buff) {
    struct PAK *package;
    memcpy(package->header, buff, 5);
    memcpy(&(package->length), buff+6, 2);
    memcpy(package->data, buff+8,1024);
    return *package;
}

