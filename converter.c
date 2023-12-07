#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convert(int num) {

    char hexString[5];  // 4 characters for hex and 1 for the null
    char splitList[2][3];  // Array to hold two strings, each of 2 characters and a null terminator

    // Convert the decimal number to a hexadecimal string
    sprintf(hexString, "%04X", num);

    // Split the hexString into two parts
    strncpy(splitList[0], hexString, 2);    // Copy the first two characters
    splitList[0][2] = '\0';                 // Null terminate the first string
    strncpy(splitList[1], hexString + 2, 2); // Copy the next two characters
    splitList[1][2] = '\0';                 // Null terminate the second string

    // Print the split strings
    printf("First part: %s\n", splitList[0]);
    printf("Second part: %s\n", splitList[1]);

}

int main() {
	int num = 25;
	convert(num);
	return 0;
}

