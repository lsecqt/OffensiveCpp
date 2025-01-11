#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void writeToADS(const char* fileName, const char* streamName, const char* data) {
    char fullPath[MAX_PATH];
    snprintf(fullPath, sizeof(fullPath), "%s:%s", fileName, streamName);

    FILE* file = fopen(fullPath, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error opening stream for writing.\n");
        return;
    }

    fwrite(data, sizeof(char), strlen(data), file);
    fclose(file);
    printf("Data written to stream '%s' successfully.\n", streamName);
}

void readFromADS(const char* fileName, const char* streamName) {
    char fullPath[MAX_PATH];
    snprintf(fullPath, sizeof(fullPath), "%s:%s", fileName, streamName);

    FILE* file = fopen(fullPath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening stream for reading.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(buffer, sizeof(char), size, file);
    buffer[size] = '\0'; // Null-terminate the string
    fclose(file);

    printf("Data read from stream '%s':\n%s\n", streamName, buffer);
    free(buffer);
}

int main() {
    const char* fileName = "example.txt";
    const char* streamName = "myStream";
    const char* data = "This is some data stored in an alternate data stream.";

    writeToADS(fileName, streamName, data);

    readFromADS(fileName, streamName);

    return 0;
}
