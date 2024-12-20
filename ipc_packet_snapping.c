#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ipc_packet_snapping.h"

// Method implementations
void setHeader(Packet *self, uint16_t header)
{
    self->header = header;
}

void setLength(Packet *self, uint16_t length)
{
    self->length = length;
}

void setId(Packet *self, uint8_t id)
{
    self->id = id;
}

void setData(Packet *self, const char *data)
{
    free(self->data);                
    self->length = strlen(data) + 1; 
    self->data = malloc(self->length);
    strcpy(self->data, data);

    // Calculate checksum as a simple sum of data bytes
    self->checksum = 0;
    for (size_t i = 0; i < self->length - 1; ++i)
    {
        self->checksum += self->data[i];
    }
}

void serialize(Packet *self, uint8_t *buffer, size_t bufferSize)
{
    if (bufferSize < (5 + self->length))
    {
        fprintf(stderr, "Buffer too small for serialization\n");
        return;
    }

    buffer[0] = (self->header >> 8) & 0xFF; 
    buffer[1] = self->header & 0xFF;        
    buffer[2] = (self->length >> 8) & 0xFF; 
    buffer[3] = self->length & 0xFF;        
    buffer[4] = self->id;                   

    memcpy(buffer + 5, self->data, self->length);  
    buffer[5 + self->length - 1] = self->checksum; 
}

void printPacket(Packet *self)
{
    printf("Packet Header: 0x%04X\n", self->header);
    printf("Packet Length: %u\n", self->length);
    printf("Packet ID: %u\n", self->id);
    printf("Packet Data: %s\n", self->data ? self->data : "(null)");
    printf("Packet Checksum: 0x%02X\n", self->checksum);
}

int parsePacket(Packet *self, const uint8_t *buffer_original, size_t bufferSize)
{
    int calculatedChecksum = calculateChecksum(buffer_original, bufferSize);
    if (calculateChecksum == 0)
    {
        fprintf(stderr, "Checksum is invalid to parse\n");
        return -1;
    }

    if (bufferSize < 5)
    {
        fprintf(stderr, "Buffer to small to parse\n");
        return -1;
    }

    self->header = (buffer_original[0] << 8) | buffer_original[1];
    self->length = (buffer_original[2] << 8) | buffer_original[3];


    if (bufferSize < (3 + self->length)) {
        fprintf(stderr, "Buffer does not match declared length\n");
        return -1; // Error code
    }

    // Extract ID (1 byte)
    self->id = buffer_original[4];
    // printf("Parsed ID: %u\n", self->id);

    
    size_t elementsToRemove = 5; // remove first 5 elements from input package
    size_t newBufferSize = bufferSize - elementsToRemove - 1; // initialize new buffer size 
    uint8_t buffer_new[newBufferSize]; 
    memcpy(buffer_new, buffer_original + elementsToRemove, newBufferSize); // copy original buffer to new according new size 
    buffer_new[newBufferSize] = '\0'; // set to the end NULL character 
    
    // printf("Updated buffer: ");
    // for (size_t i = 0; i < newBufferSize; i++) {
    //     printf("%02X ", buffer_new[i]);
    // }
    printf("Parsed Header: %02X\n", self->header);
    printf("Parsed length: %u\n", self->length);
    printf("Parsed ID: %u\n", self->id);
    printf("Parsed text: %s\n", (char *)buffer_new);

}

int calculateChecksum(const uint8_t *buffer, size_t bufferSize)
{
    uint8_t checksum = 0;

    for (size_t i = 0; i < bufferSize - 1; i++)
    {
        // printf("%02X ", buffer[i]);
        checksum += buffer[i];
    }
    uint8_t calculatedChecksum = checksum % 0xFF;
    uint8_t receivedChecksum = buffer[bufferSize - 1];

    // printf("\nCalculated Sum: %d\n", checksum);

    // printf("Calculated Checksum: %02X\n", calculatedChecksum);

    // printf("Received Checksum: %d\n", receivedChecksum);

    if (calculatedChecksum == receivedChecksum)
    {
        printf("Checksum is valid.\n");
        return 1;
    }
    else
    {
        printf("Checksum is invalid.\n");
        return 0;
    }
}

// Constructor
Packet *newPacket()
{
    Packet *packet = malloc(sizeof(Packet));
    packet->header = 0;
    packet->length = 0;
    packet->id = 0;
    packet->checksum = 0;
    packet->data = NULL;

    // Assign methods
    packet->setHeader = setHeader;
    packet->setLength = setLength;
    packet->setId = setId;
    packet->setData = setData;
    packet->serialize = serialize;
    packet->printPacket = printPacket;

    return packet;
}

// Destructor
void deletePacket(Packet *packet)
{
    if (packet)
    {
        free(packet->data);
        free(packet);
    }
}