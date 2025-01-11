#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Global variables
unsigned char* buf = NULL; // Buffer to store file content
DWORD size = 0;            // Variable to store file size

// Function to read binary file
void readBin(const char* fileName) {
    FILE* file;

    file = fopen(fileName, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    fseek(file, 0, SEEK_END); // Move to end of file
    size = ftell(file);       // Get file size
    fseek(file, 0, SEEK_SET); // Move back to start of file

    buf = (unsigned char*)malloc(size * sizeof(char)); // Allocate memory for file content
    if (buf == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(buf, sizeof(char), size, file); // Read file content into buffer
    fclose(file);

    printf("File read successfully. Size: %lu bytes.\n", size);
}


int main() {
    const char* fileName = "example.bin"; // Specify the binary file to read
    readBin(fileName);                   // Call the function to read the file

    return 0;
}
