#include "ipc.h"

void setHeader(Package *self, uint16_t header){
    self->header = header;
}

void setLength(Package *self, uint16_t length)
{
    self->length = length;
}

void setId(Package *self, uint8_t id)
{
    self->id = id;
}

void setData(Package *self, const char *data)
{
    self->data = data;
}

void printReadyPackage(Package *self)
{
    printf("Packet Header: 0x%04X\n", self->header);
    printf("Packet Length: %u\n", self->length);
    printf("Packet ID: %u\n", self->id);
    printf("Packet Data: %s\n", self->data ? self->data : "(null)");
    if (self->checksum != 0){
        printf("Packet Checksum: 0x%02X\n", self->checksum);
    }
}

int getReadyPackage(Package *self)
{   
    uint8_t bufferReadyPackage[255] = {}; 

    bufferReadyPackage[0] = (self->header >> 8) & 0xFF; 
    bufferReadyPackage[1] = self->header & 0xFF;        
    bufferReadyPackage[2] = (self->length >> 8) & 0xFF; 
    bufferReadyPackage[3] = self->length & 0xFF;        
    bufferReadyPackage[4] = self->id;                   

    memcpy(bufferReadyPackage + 5, self->data, self->length);  
    // Calculate checksum
    uint8_t checksum = 0;
    for (int i = 0; i < self->length + 5; i++) {
        checksum = (checksum + bufferReadyPackage[i]) % 0xFF;
    }
    bufferReadyPackage[5 + self->length] = checksum;

    // Print the package with checksum
    for (int i = 0; i < self->length + 6; i++) { // Include checksum byte
        printf("%02X ", bufferReadyPackage[i]);
    }
    printf("\n");

    // Update the checksum field in the struct
    self->checksum = checksum;
    return 0;
}

int parsePackage(Package *self, const uint8_t *buffer, size_t bufferSize)
{
       // Minimum package size: header (2 bytes) + length (2 bytes) + ID (1 byte) + checksum (1 byte)
    if (bufferSize < 6) {
        return -1; // Error: Buffer too small
    }

    // Extract header
    self->header = (buffer[0] << 8) | buffer[1];

    // Extract length
    self->length = (buffer[2] << 8) | buffer[3];

    // Validate length
    if (self->length + 6 > bufferSize) {
        return -2; // Error: Declared length exceeds buffer size
    }

    // Extract ID
    self->id = buffer[4];

    // Extract data
    memcpy(self->data_buffer, buffer + 5, self->length);
    self->data_buffer[self->length] = '\0'; // Null-terminate for safety
    self->data = self->data_buffer; // Point data to the buffer

    // Extract checksum from the buffer
    uint8_t receivedChecksum = buffer[5 + self->length];

    // Calculate checksum from the rest of the package
    uint8_t calculatedChecksum = 0;
    for (size_t i = 0; i < 5 + self->length; i++) {
        calculatedChecksum = (calculatedChecksum + buffer[i]) % 0xFF;
    }

    // Validate checksum
    if (calculatedChecksum != receivedChecksum) {
        return -3; // Error: Checksum mismatch
    }

    // Store checksum in the struct
    self->checksum = receivedChecksum;

    return 0; // Success
}
