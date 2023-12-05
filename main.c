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
		//LDBr SECTION HERE
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
		//LDBA stack = D3 = 211
		else if (mem[j] == 211) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			accum = mem[sp - memLocation];
			j += 3;
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
		//LDBX stack = DB = 219
		else if (mem[j] == 219) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			index = mem[sp - memLocation];
			j += 3;
		}

		//LDWr SECTION HERE, direct=DONE, immediate=DONE, stack=DONE
		//LDWA direct = C1 = 193
		else if (mem[j] == 193) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = extractAddressLocation(first, second);

			//I think we have to change all our word functions to something like this
			//Reason is because if we only go to the memLocation that would only be half the word
			int word;
			word = extractAddressLocation(memLocation, memLocation + 1);
			accum = word;
			printf("Int accessed: %d\n", accum);
			j += 3;
		}
		//LDWX direct = C9 = 193
		else if (mem[j] == 201) {
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
		//LDWA Immediate = C0 = 192
		else if (mem[j] == 192) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = extractAddressLocation(first, second);

			accum = memLocation;
			j += 3
		}
		//LDWX Immdeiate = C8 = 200
		else if (mem[j] == 200) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = extractAddressLocation(first, second);

			index = memLocation;
			j += 3
		}
		//LDWA s = C3 = 195
		else if (mem[j] == 195) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			accum = mem[sp - memLocation];
			j += 3;
		}
		//LDWX s = CB = 203
		else if (mem[j] == 203) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			index = mem[sp - memLocation];
			j += 3;
		}
		//For stack relative defered, store address of value in mem, when we get the load word instruction, we will know it will be there already

		//STBr SECTION HERE
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

		//STWr SECTION HERE, stack=DONE
		//STWA direct = E1 = 225
		else if (mem[j] = 225) {
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

		//STWA stack = E3 = 227
		else if (mem[j] == 227) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int placeInStack = memLocation;
			mem[sp - placeInStack] = accum;

			j += 3;
		}
		//STWX stack = EB = 235
		else if (mem[j] == 235) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int placeInStack = memLocation;
			mem[sp - placeInStack] = index;

			j += 3;
		}

		//DECO SECTION HERE
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

		//BR SECTION HERE
		//BR = 12(hexa) = 18
		else if (mem[j] == 18) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			j = memLocation;
		}
		//BRLE = 14(hexa) = 20
		else if (mem[j] == 20) {
			if (accum <= 0) {
				int first = mem[j + 1];
				int second = mem[j + 2];
				int memLocation = extractAddressLocation(first, second);
				j = memLocation;
			}
			else {
				j += 3;
			}
		}
		//BRLT = 16(hexa) = 22
		else if (mem[j] == 22) {
			if (accum < 0) {
				int first = mem[j + 1];
				int second = mem[j + 2];
				int memLocation = extractAddressLocation(first, second);
				j = memLocation;
			}
			else {
				j += 3;
			}
		}
		//BREQ = 18(hexa) = 24
		else if (mem[j] == 24) {
			if (accum == 0) {
				int first = mem[j + 1];
				int second = mem[j + 2];
				int memLocation = extractAddressLocation(first, second);
				j = memLocation;
			}
			else {
				j += 3;
			}
		}
		//BRNE = 1A(hexa) = 26
		else if (mem[j] == 26) {
			if (accum != 0) {
				int first = mem[j + 1];
				int second = mem[j + 2];
				int memLocation = extractAddressLocation(first, second);
				j = memLocation;
			}
			else {
				j += 3;
			}
		}
		//BRGE = 1C(hexa) = 28
		else if (mem[j] == 28) {
			if (accum >= 0) {
				int first = mem[j + 1];
				int second = mem[j + 2];
				int memLocation = extractAddressLocation(first, second);
				j = memLocation;
			}
			else {
				j += 3;
			}
		}
		//BRGT = 1E(hexa) = 30
		else if (mem[j] == 30) {
			if (accum > 0) {
				int first = mem[j + 1];
				int second = mem[j + 2];
				int memLocation = extractAddressLocation(first, second);
				j = memLocation;
			}
			else {
				j += 3;
			}
		}
		//STILL NEED BRV

		//STACK POINTER SECTION HERE
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
		
		//ADDA direct
		else if (mem[j] == 97) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum + mem[memLocation];
			printf("accumulator sum is", accum);
			j += 3;

		}
		//ADDA immediate
		else if (mem[j] == 96) {//adda,i
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum + memLocation;
			j += 3;

		}
		//ADDX direct
		else if (mem[j] == 105) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index + mem[memLocation];
			printf("index sum is", accum);
			j += 3;
		}
		//ADDX immediate
		else if (mem[j] == 104) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index + memLocation;
			j += 3;
		}

		//SUBA direct
		else if (mem[j] == 113) {//suba,d
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum - mem[memLocation];
			printf("accumulator sum is", accum);
			j += 3;
		}
		//SUBA immediate
		else if (mem[j] == 112) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum - memLocation;
			printf("accumulator sum is", accum);
			j += 3;
		}
		
		//SUBX direct
		else if (mem[j] == 121) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index - mem[memLocation];
			printf("index sum is", accum);
			j += 3;
		}
		//SUBX immediate
		else if (mem[j] == 120) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index - memLocation;
			printf("index sum is", accum);
			j += 3;
		}
		//CALL = 24(hexa) = 36
		else if (mem[j] == 36) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			mem[sp - 2] = j;
			j = memLocation;
		}
		//RET = 01 = 1
		else if (mem[j] == 1) {
			int first = mem[sp];
			int second = mem[sp + 1];
			int memLocation = extractAddressLocation(first, second);

			j = memLocation;
		}
		//This else statement takes care of STOP/00
		else {
			j += 1;
		}
		
	//TODO = BRV, ASL, ASR, DECI, STRO, NOT, NEG, CPWr, CPBr, MOVSPA
	//Addressing Modes to Add = 
	return 0;
}