#ifndef IPC_PACKET_H
#define IPC_PACKET_H

#define MAX_DATA_SIZE 256

#include <stdint.h>
#include <stdio.h>

// Define the Packet structure
typedef struct Packet
{
    uint16_t header;
    uint16_t length;
    uint8_t id;
    uint8_t checksum;
    char *data;
    char data_buffer[MAX_DATA_SIZE];

    // Methods (function pointers)
    void (*setHeader)(struct Packet *self, uint16_t header);
    void (*setLength)(struct Packet *self, uint16_t length);
    void (*setId)(struct Packet *self, uint8_t id);
    void (*setData)(struct Packet *self, const char *data);
    void (*serialize)(struct Packet *self, uint8_t *buffer, size_t bufferSize);
    void (*printPacket)(struct Packet *self);
} Packet;

// Constructor and Destructor
Packet *newPacket();
void deletePacket(Packet *packet);

// Parsing function
int parsePacket(Packet *self, const uint8_t *buffer, size_t bufferSize);
int calculateChecksum(const uint8_t *buffer, size_t bufferSize);

#endif // IPC_PACKET_H
