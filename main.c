#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	//Initializing main mem, accumulator, index register
	int mem[65536];
	int accum;
	int index;

	//Inputs
	printf("Enter Object Code: ");
	char input[3];
	int deciValue = 0;
	int i = 0;

	do {
		//Reads object code string by string untill string = zz
		scanf("%s", &input);

		//Converts string into decimal value
		deciValue = strtol(input, NULL, 16);

		//Storing in main mem
		mem[i] = deciValue;

		i += 1;
	} while (strcmp(input, "zz") != 0);

	int j = 0;
	//While loop to work with object code
	while (mem[j] != '\0')
		//If mem value = 209/D1/LDBA load the next 2 cells of mem
		if (mem[j] == 209) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Creating strings to hold hexa values of the 2 cells
			char hexString[5];
			char hexString2[5];
			//Converting deci values to hexa
			sprintf(hexString, "%x", first);
			sprintf(hexString2, "%x", second);

			//Merging Strings, merged string is stored in hexString
			strcat(hexString, hexString2);

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = strtol(hexString, NULL, 16);

			//Accesses specified memory location
			//Have not done much testing but this accesses the correct cell when using "D1 00 0D F1 FC 16 D1 00 0E F1 FC 16 00 48 69 zz"
			accum = mem[memLocation];
			printf("Int accessed: %d\n", accum);
			j += 3;
		}
		//If mem = 241/STBA/F1 then load next 2 values in mem
		else if (mem[j] == 241) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Creating strings to hold hexa values of the 2 cells
			char hexString[5];
			char hexString2[5];
			//Converting deci values to hexa
			sprintf(hexString, "%x", first);
			sprintf(hexString2, "%x", second);

			//Merging Strings, merged string is stored in hexString
			strcat(hexString, hexString2);

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = strtol(hexString, NULL, 16);

			//If cells are FC16/64534 print them
			if (memLocation == 64534) {
				printf("String stored in output cells: %d\n", accum);
			}
			j += 3;
		}
		else {
			j += 1;
		}

	return 0;
}