#ifndef IPC_H
#define IPC_H

#define MAX_DATA_SIZE 256

#include <stdint.h>
#include <stdio.h>

typedef struct Package {
    uint16_t header;  
    uint16_t length; 
    uint8_t id;       
    uint8_t checksum; 
    char *data;       
    char data_buffer[MAX_DATA_SIZE]; 
} Package;

void setHeader(Package *self, uint16_t header);
void setLength(Package *self, uint16_t length);
void setId(Package *self, uint8_t id);
void setData(Package *self, const char *data);

void printReadyPackage(Package *self);
int getReadyPackage(Package *self);

int parsePackage(Package *self, const uint8_t *buffer, size_t bufferSize);
int calculateChecksum(const uint8_t *buffer, size_t bufferSize);


#endif // IPC_H