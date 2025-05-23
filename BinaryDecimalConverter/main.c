#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 64

// Function prototypes
int binaryToDecimal(const char* binary);
void decimalToBinary(int decimal, char* binary);
int isValidBinary(const char* binary);
void menu();
void processFileIO();

int main() {
    int choice;
    char buffer[BUFFER_SIZE];

    while (1) {
        menu();
        printf("Enter your choice: ");
        if (scanf_s("%d", &choice) != 1) {
            printf("Invalid input!\n");
            while (getchar() != '\n'); // input temizle
            continue;
        }
        while (getchar() != '\n'); // newline'ý temizle

        if (choice == 1) { // Binary to Decimal
            printf("Enter binary number: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline

            if (!isValidBinary(buffer)) {
                printf("Invalid binary input!\n");
                continue;
            }

            int decimal = binaryToDecimal(buffer);
            printf("Result: %d\n", decimal);

        }
        else if (choice == 2) { // Decimal to Binary
            int decimal;
            printf("Enter decimal number: ");
            if (scanf_s("%d", &decimal) != 1) {
                printf("Invalid input!\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');

            char binary[BUFFER_SIZE];
            decimalToBinary(decimal, binary);
            printf("Result: %s\n", binary);

        }
        else if (choice == 3) { // File I/O
            processFileIO();

        }
        else if (choice == 0) {
            break;

        }
        else {
            printf("Invalid choice!\n");
        }
    }

    return 0;
}

// Menu display
void menu() {
    printf("\nBinary-Decimal Converter\n");
    printf("1: Binary › Decimal\n");
    printf("2: Decimal › Binary\n");
    printf("3: Batch Convert via File (input.txt › output.txt)\n");
    printf("0: Exit\n");
}

// Convert binary (two's complement if needed) to decimal
int binaryToDecimal(const char* binary) {
    size_t len = strlen(binary);
    int isNegative = (binary[0] == '1' && len == 8); // Assume 8-bit signed binary

    int result = 0;
    for (size_t i = 0; i < len; i++) {
        result = result << 1 | (binary[i] - '0');
    }

    if (isNegative) {
        result -= (1 << len);
    }

    return result;
}

// Convert decimal to binary string (two's complement for negatives)
void decimalToBinary(int decimal, char* binary) {
    unsigned int mask = 1 << 7; // 8-bit
    for (int i = 0; i < 8; i++) {
        binary[i] = (decimal & mask) ? '1' : '0';
        mask >>= 1;
    }
    binary[8] = '\0';
}

// Validate binary string
int isValidBinary(const char* binary) {
    size_t len = strlen(binary);
    if (len > 8) return 0;
    for (size_t i = 0; i < len; i++) {
        if (binary[i] != '0' && binary[i] != '1') return 0;
    }
    return 1;
}

// Batch processing from input.txt to output.txt
void processFileIO() {
    FILE* in = fopen("input.txt", "r");
    if (!in) {
        perror("Error opening input.txt");
        return;
    }

    FILE* out = fopen("output.txt", "w");
    if (!out) {
        perror("Error opening output.txt");
        fclose(in);
        return;
    }

    char line[BUFFER_SIZE];

    while (fgets(line, sizeof(line), in)) {
        line[strcspn(line, "\n")] = '\0';

        if (isValidBinary(line)) {
            int dec = binaryToDecimal(line);
            fprintf(out, "%s -> %d\n", line, dec);
        }
        else {
            int decimal = atoi(line);
            char binary[BUFFER_SIZE];
            decimalToBinary(decimal, binary);
            fprintf(out, "%d -> %s\n", decimal, binary);
        }
    }

    printf("Batch processing complete. Results saved to output.txt.\n");

    fclose(in);
    fclose(out);
}
