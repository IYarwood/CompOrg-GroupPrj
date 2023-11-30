#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int extractAddressLocation(int first, int second) {
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
	return memLocation;
}

int main() {
	//Initializing main mem, accumulator, index register
	int mem[65536];
	int accum;
	int index;
	int sp = 64399;

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
		//If mem value = 209 = D1 = LDBA Direct load the next 2 cells of mem
		if (mem[j] == 209) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = extractAddressLocation(first, second);

			//Accesses specified memory location
			//Have not done much testing but this accesses the correct cell when using "D1 00 0D F1 FC 16 D1 00 0E F1 FC 16 00 48 69 zz"
			accum = mem[memLocation];
			printf("Int accessed: %d\n", accum);
			j += 3;
		}
		//208 = D0 = LDBA Immediate
		else if (mem[j] == 208) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = extractAddressLocation(first, second);

			accum = memLocation;
			j += 3 
		}
		//217 = D9 = LDBX Direct
		else if (mem[j] == 217) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = extractAddressLocation(first, second);

			//Accesses specified memory location
			//Have not done much testing but this accesses the correct cell when using "D1 00 0D F1 FC 16 D1 00 0E F1 FC 16 00 48 69 zz"
			index = mem[memLocation];
			
			j += 3;
		}
		//216 = D8 = LDBX immediate
		else if (mem[j] == 216) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = extractAddressLocation(first, second);

			index = memLocation;
			j += 3
		}
		//If mem = 241/STBA/F1 then load next 2 values in mem
		else if (mem[j] == 241) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			//If cells are FC16/64534 print them
			if (memLocation == 64534) {
				printf("String stored in output cells: %d\n", accum);
			}
			//If not FC16 then store accumulator at given address
			else {
				mem[memLocation] = accum;
			}
			j += 3;
		}
		//STWX stack = EB = 235
		else if (mem[j] == 235) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int placeInStack = memLocation;
			mem[sp - placeInStack] = index;
		}
		//DECO = 39(hexa) = 57 direct
		else if (mem[j] == 57) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			printf("%d", mem[memLocation]);
			j += 3;
		}
		//DECO = 38(hexa) = 56 immediate
		else if (mem[j] == 56) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			printf("%d", memLocation);
			j += 3;
		}
		//BR = 12(hexa) = 18
		else if (mem[j] == 18) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			j = memLocation;
		}
		//SUBSP Immediate = 58(hexa) = 88
		else if (mem[j] == 88) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			sp -= memLocation;
		}
		//ADDSP Immediate = 50(hexa) = 80
		else if (mem[j] == 80) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			sp += memLocation;
		}
		//This else statement takes care of STOP/00
		else {
			j += 1;
		}

	return 0;
}