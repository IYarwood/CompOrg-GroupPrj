#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Authors: Ian Yarwood, Aditya Shah, Nicholas Haught, Sahmi Willoughby
//Program: Pep 9 
//Date: 12/15/23
//Class: Comp Org/Arch
//Desc: Performs specific operations based off of given object code.

void convert(int num, int* firstHalf, int* secondHalf) {

	char hexString[5];  // 4 characters for hex and 1 for the null
	char splitList[2][3];  // Array to hold two strings, each of 2 characters and a null terminator

	// Convert the decimal number to a hexadecimal string
	sprintf(hexString, "%04X", num);

	// Split the hexString into two parts
	strncpy(splitList[0], hexString, 2);    // Copy the first two characters
	splitList[0][2] = '\0';                 // Null terminate the first string
	strncpy(splitList[1], hexString + 2, 2); // Copy the next two characters
	splitList[1][2] = '\0';                 // Null terminate the second string

	int deciFirstHalf = strtol(splitList[0], NULL, 16);
	int deciSecondHalf = strtol(splitList[1], NULL, 16);
	*firstHalf = deciFirstHalf;
	*secondHalf = deciSecondHalf;

	// Print the split strings
	//printf("First part: %s\n", splitList[0]);
	//printf("Second part: %s\n", splitList[1]);
}

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
	int heapPointer;
	int v = 0;
	int n = 0;
	int z = 0;
	int c = 0;

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
	heapPointer = i;


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
			j += 3;
		}
	//LDBA stack = D3 = 211
		else if (mem[j] == 211) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			accum = mem[sp + memLocation];
			j += 3;
		}
	//LDBA stack relative deferred
		else if (mem[j] == 212) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			accum = mem[mem[sp + memLocation]];
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
			j += 3;
		}
	//LDBX stack = DB = 219
		else if (mem[j] == 219) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			index = mem[sp + memLocation];
			j += 3;
		}
	//LDBX stack relative deferred
		else if (mem[j] == 220) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			index = mem[mem[sp + memLocation]];
			j += 3;
		}
	//LDBA indexed
		else if (mem[j] == 213) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = mem[memLocation + offSet];
			j += 3;
		}
	//LDBX indexed
		else if (mem[j] == 221) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			index = mem[memLocation + offSet];
			j += 3;
		}
	//LDBA stack indexed
		else if (mem[j] == 214) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = mem[sp + memLocation + offSet];
			j += 3;
		}
	//LDBX stack indexed
		else if (mem[j] == 222) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			index = mem[sp + memLocation + offSet];
			j += 3;
		}
	//LDBA stack indexed deferred
		else if (mem[j] == 215) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = mem[mem[sp + memLocation + offSet]];
			j += 3;
		}
	//LDBX stack indexed deferred
		else if (mem[j] == 223) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			index = mem[mem[sp + memLocation + offSet]];
			j += 3;
		}
	//LDBA indirect
		else if (mem[j] == 210) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			accum = mem[mem[memLocation]];
			j += 3;
		}
	//LDBX indirect
		else if (mem[j] == 218) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			index = mem[mem[memLocation]];
			j += 3;
		}

	//LDWr SECTION HERE
	//LDWA direct = C1 = 193
		else if (mem[j] == 193) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = extractAddressLocation(first, second);

			int word;
			word = extractAddressLocation(mem[memLocation], mem[memLocation + 1]);
			accum = word;
			j += 3;
		}
	//LDWX direct = C9 = 193
		else if (mem[j] == 201) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = extractAddressLocation(first, second);

			int word;
			word = extractAddressLocation(memLocation, memLocation + 1);
			index = word;
			//Accesses specified memory location
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
			j += 3;
		}
	//LDWX Immdeiate = C8 = 200
		else if (mem[j] == 200) {
			int first = mem[j + 1];
			int second = mem[j + 2];

			//Converts merged strings to decimal value
			int memLocation;
			memLocation = extractAddressLocation(first, second);

			index = memLocation;
			j += 3;
		}
	//LDWA s = C3 = 195
		else if (mem[j] == 195) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(sp+memLocation, sp+ memLocation + 1);
			accum = word;
			j += 3;
		}
	//LDWX s = CB = 203
		else if (mem[j] == 203) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int word;
			word = extractAddressLocation(sp + memLocation, sp + memLocation + 1);
			index = word;
			j += 3;
		}
	//LDWA stack relative deferred
		else if (mem[j] == 196) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int word;
			word = extractAddressLocation(mem[sp + memLocation], mem[sp + memLocation + 1]);
			accum = word;
			j += 3;
		}
	//LDWX stack relative deferred
		else if (mem[j] == 204) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int word;
			word = extractAddressLocation(mem[sp + memLocation], mem[sp + memLocation + 1]);
			index = word;
			//index = mem[mem[sp + memLocation]];
			j += 3;
		}
	//LDWA indexed
		else if (mem[j] == 197) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			int word;
			word = extractAddressLocation(memLocation+offSet, memLocation + offSet+ 1);
			accum = word;
			j += 3;
		}
	//LDWX indexed
		else if (mem[j] == 205) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			int word;
			word = extractAddressLocation(memLocation + offSet, memLocation + offSet + 1);
			index = word;
			j += 3;
		}
	//LDWA stack-indexed
		else if (mem[j] = 198) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			int word;
			word = extractAddressLocation(sp + memLocation + offSet, sp + memLocation + offSet + 1);
			accum = word;
			j += 3;
		}
	//LDWX stack-indexed
		else if (mem[j] == 206) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			int word;
			word = extractAddressLocation(sp + memLocation + offSet, sp + memLocation + offSet + 1);
			index = word;
			j += 3;
		}
	//LDWA stack-index-defferred
		else if (mem[j] == 199) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			int word;
			word = extractAddressLocation(mem[sp + memLocation + offSet], mem[sp + memLocation + offSet + 1]);
			accum = word;
			j += 3;
		}
	//LDWX stack-index-deferred
		else if (mem[j] == 207) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			int word;
			word = extractAddressLocation(mem[sp + memLocation + offSet], mem[sp + memLocation + offSet + 1]);
			index = word;
			j += 3;
		}
	//LDWA indirect
		else if (mem[j] == 194) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int word;
			word = extractAddressLocation(mem[memLocation], mem[memLocation + 1]);
			accum = word;
			j += 3;
		}
	//LDWX indirect
		else if (mem[j] == 202) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int word;
			word = extractAddressLocation(mem[memLocation], mem[memLocation + 1]);
			index = word;
			j += 3;
		}

	//STBr SECTION HERE
	//If mem = 241/STBA/F1 then load next 2 values in mem
		else if (mem[j] == 241) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			//If cells are FC16/64534 print them
			if (memLocation == 64534) {
				printf("%c", accum);
			}
			//If not FC16 then store accumulator at given address
			else {
				mem[memLocation] = accum;
			}
			j += 3;
		}
	//STBX direct = F9 = 249
		else if (mem[j] == 249) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			//If cells are FC16/64534 print them
			if (memLocation == 64534) {
				printf("%c", index);
			}
			//If not FC16 then store accumulator at given address
			else {
				mem[memLocation] = index;
			}
			j += 3;
		}
	//STBA stack = F3 = 243
		else if (mem[j] == 243) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int placeInStack = memLocation;
			mem[sp + placeInStack] = accum;

			j += 3;
		}
	//STBX stack = FB = 251
		else if (mem[j] == 251) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int placeInStack = memLocation;
			mem[sp + placeInStack] = index;

			j += 3;
		}
	//STBA stack relative deferred
		else if (mem[j] == 244) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int placeInStack = memLocation;
			mem[mem[sp + placeInStack]] = accum;

			j += 3;
		}
	//STBX stack relative deferred
		else if (mem[j] == 252) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int placeInStack = memLocation;
			mem[mem[sp + placeInStack]] = index;
			j += 3;
		}
	//STBA indexed
		else if (mem[j] == 245) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			mem[memLocation + offSet] = accum;
			j += 3;
		}
	//STBX indexed
		else if (mem[j] == 253) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			mem[memLocation + offSet] = index;
			j += 3;
		}
	//STBA stack-indexed
		else if (mem[j] == 246) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			mem[sp + memLocation + offSet] = accum;
			j += 3;
		}
	//STBX stack-indexed
		else if (mem[j] = 254) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			mem[sp + memLocation + offSet] = index;
			j += 3;
		}
	//STBA stack-indexed-defferred
		else if (mem[j] == 247) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			mem[mem[sp + memLocation + offSet]] = accum;
			j += 3;
		}
	//STBX stack-indexed-defferred
		else if (mem[j] == 255) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			mem[mem[sp + memLocation + offSet]] = index;
			j += 3;
		}
	//STBA indirect
		else if (mem[j] == 242) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			mem[mem[memLocation]] = accum;
			j += 3;
		}
	//STBX indirect
		else if (mem[j] == 250) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			mem[mem[memLocation]] = index;
			j += 3;
		}

	//STWr SECTION HERE
	//STWA direct = E1 = 225
		else if (mem[j] == 225) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			convert(accum, &deciFirstHalf, &deciSecondHalf);
			mem[memLocation] = deciFirstHalf;
			mem[memLocation + 1] = deciSecondHalf;
			j += 3;
		}
	//STWX direct = E9 = 233
		else if (mem[j] == 233) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			convert(index, &deciFirstHalf, &deciSecondHalf);
			mem[memLocation] = deciFirstHalf;
			mem[memLocation + 1] = deciSecondHalf;
			j += 3;
		}
	//STWA stack = E3 = 227
		else if (mem[j] == 227) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			convert(accum, &deciFirstHalf, &deciSecondHalf);
			mem[sp + memLocation] = deciFirstHalf;
			mem[sp + (memLocation + 1)] = deciSecondHalf;
			j += 3;
		}
	//STWX stack = EB = 235
		else if (mem[j] == 235) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			convert(index, &deciFirstHalf, &deciSecondHalf);
			mem[sp + memLocation] = deciFirstHalf;
			mem[sp + (memLocation + 1)] = deciSecondHalf;
			j += 3;
		}
	//STWA stack relative deferred
		else if (mem[j] == 228) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			convert(accum, &deciFirstHalf, &deciSecondHalf);
			mem[mem[sp + memLocation]] = deciFirstHalf;
			mem[mem[sp + (memLocation + 1)]] = deciSecondHalf;
			j += 3;
		}
	//STWX stack relative deferred
		else if (mem[j] == 236) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			convert(index, &deciFirstHalf, &deciSecondHalf);
			mem[mem[sp + memLocation]] = deciFirstHalf;
			mem[mem[sp + (memLocation + 1)]] = deciSecondHalf;
			j += 3;
		}
	//STWA indexed
		else if (mem[j] == 229) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			int offSet = index * 2;
			convert(accum, &deciFirstHalf, &deciSecondHalf);
			mem[memLocation + offSet] = deciFirstHalf;
			mem[memLocation + offSet + 1] = deciSecondHalf;
			j += 3;
		}
	//STWX Indexed
		else if (mem[j] = 237) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			int offSet = index * 2;
			convert(index, &deciFirstHalf, &deciSecondHalf);
			mem[memLocation + offSet] = deciFirstHalf;
			mem[memLocation + offSet + 1] = deciSecondHalf;
			j += 3;
		}
	//STWA stack-indexed
		else if (mem[j] == 230) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			int offSet = index * 2;
			convert(accum, &deciFirstHalf, &deciSecondHalf);
			mem[sp + memLocation + offSet] = deciFirstHalf;
			mem[sp + memLocation + offSet + 1] = deciSecondHalf;
			j += 3;
		}
	//STWX stack indexed
		else if (mem[j] == 238) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			int offSet = index * 2;
			convert(index, &deciFirstHalf, &deciSecondHalf);
			mem[sp + memLocation + offSet] = deciFirstHalf;
			mem[sp + memLocation + offSet + 1] = deciSecondHalf;
			j += 3;
		}
	//STWA stack-index-defferred
		else if (mem[j] == 231) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			int offSet = index * 2;
			convert(accum, &deciFirstHalf, &deciSecondHalf);
			mem[mem[sp + memLocation + offSet]] = deciFirstHalf;
			mem[mem[sp + memLocation + offSet + 1]] = deciSecondHalf;
			j += 3;
		}
	//STWX stack-indexed-defferred
		else if (mem[j] == 239) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			int offSet = index * 2;
			convert(index, &deciFirstHalf, &deciSecondHalf);
			mem[mem[sp + memLocation + offSet]] = deciFirstHalf;
			mem[mem[sp + memLocation + offSet + 1]] = deciSecondHalf;
			j += 3;
		}
	//STWA indirect
		else if (mem[j] == 226) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			convert(accum, &deciFirstHalf, &deciSecondHalf);
			mem[mem[memLocation]] = deciFirstHalf;
			mem[mem[memLocation + 1]] = deciSecondHalf;
			j += 3;
		}
	//STWX indirect
		else if (mem[j] == 234) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int deciFirstHalf;
			int deciSecondHalf;
			convert(index, &deciFirstHalf, &deciSecondHalf);
			mem[mem[memLocation]] = deciFirstHalf;
			mem[mem[memLocation + 1]] = deciSecondHalf;
			j += 3;
		}

	//DECO SECTION HERE
	//DECO = 39(hexa) = 57 direct
		else if (mem[j] == 57) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(memLocation, memLocation+1);
			printf("%d", word);
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
	//DECO stack
		else if (mem[j] == 59) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(sp + memLocation, sp + memLocation + 1);
			printf("%d", word);
			j += 3;
		}
	//DECO stack-deffered
		else if (mem[j] == 60) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int word;
			word = extractAddressLocation(mem[sp + memLocation], mem[sp + memLocation + 1]);
			printf("%d", word);
			j += 3;
		}
	//DECO indexed
		else if (mem[j] == 61) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			int word;
			word = extractAddressLocation(memLocation + offSet, memLocation + offSet + 1);
			printf("%d", word);
			j += 3;
		}
	//DECO stack indexed
		else if (mem[j] == 62) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			int word;
			word = extractAddressLocation(sp + memLocation + offSet, sp + memLocation + offSet + 1);
			printf("%d", word);
			j += 3;
		}
	//DECO stack indexed defferred
		else if (mem[j] == 63) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			int word;
			word = extractAddressLocation(mem[sp + memLocation + offSet], mem[sp + memLocation + offSet + 1]);
			printf("%d", word);
			j += 3;
		}
	//DECO indirect
		else if (mem[j] == 58) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			int word;
			word = extractAddressLocation(mem[memLocation], mem[memLocation + 1]);
			printf("%d", word);
			j += 3;
		}

	
	//DECI direct
		else if (mem[j] == 49) {
			printf("Enter Deci input: ");
			int input;
			scanf("%d", &input);
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			
			int deciFirstHalf;
			int deciSecondHalf;
			convert(input, &deciFirstHalf, &deciSecondHalf);
			mem[memLocation] = deciFirstHalf;
			mem[memLocation + 1] = deciSecondHalf;
			j += 3;
		}
	//DECI stack
		else if (mem[j] == 51) {
			printf("Enter Deci input: ");
			int input;
			scanf("%d", &input);
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int deciFirstHalf;
			int deciSecondHalf;
			convert(input, &deciFirstHalf, &deciSecondHalf);
			mem[sp+ memLocation] = deciFirstHalf;
			mem[sp+ memLocation + 1] = deciSecondHalf;
			j += 3;
		}
	//DECI stack defferred
		else if (mem[j] == 52) {
			printf("Enter Deci input: ");
			int input;
			scanf("%d", &input);
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int deciFirstHalf;
			int deciSecondHalf;
			convert(input, &deciFirstHalf, &deciSecondHalf);
			mem[mem[sp + memLocation]] = deciFirstHalf;
			mem[mem[sp + memLocation + 1]] = deciSecondHalf;
			j += 3;
		}
	//DECI indexed
		else if (mem[j] == 53) {
			printf("Enter Deci input: ");
			int input;
			scanf("%d", &input);
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int deciFirstHalf;
			int deciSecondHalf;
			int offSet = index * 2;
			convert(input, &deciFirstHalf, &deciSecondHalf);
			mem[memLocation + offSet] = deciFirstHalf;
			mem[memLocation + offSet + 1] = deciSecondHalf;
			j += 3;
		}
	//DECI stack indexed
		else if (mem[j] == 54) {
			printf("Enter Deci input: ");
			int input;
			scanf("%d", &input);
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int deciFirstHalf;
			int deciSecondHalf;
			int offSet = index * 2;
			convert(input, &deciFirstHalf, &deciSecondHalf);
			mem[sp + memLocation + offSet] = deciFirstHalf;
			mem[sp + memLocation + offSet + 1] = deciSecondHalf;
			j += 3;
		}
	//DECI stack index defferred
		else if (mem[j] == 55) {
			printf("Enter Deci input: ");
			int input;
			scanf("%d", &input);
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int deciFirstHalf;
			int deciSecondHalf;
			int offSet = index * 2;
			convert(input, &deciFirstHalf, &deciSecondHalf);
			mem[mem[sp + memLocation + offSet]] = deciFirstHalf;
			mem[mem[sp + memLocation + offSet + 1]] = deciSecondHalf;
			j += 3;
		}
	//DECI indirect
		else if (mem[j] == 50) {
			printf("Enter Deci input: ");
			int input;
			scanf("%d", &input);
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int deciFirstHalf;
			int deciSecondHalf;
			int offSet = index * 2;
			convert(input, &deciFirstHalf, &deciSecondHalf);
			mem[mem[memLocation]] = deciFirstHalf;
			mem[mem[memLocation + 1]] = deciSecondHalf;
			j += 3;
		}

	//STRO SECTION
	//STRO d = 49(hexa) = 73
		else if (mem[j] == 73) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int track = memLocation;
			while (mem[track] != 32) {
				printf("%c", mem[track]);
				track += 1;
			}
			j += 3;
		}
	//STRO stack
		else if (mem[j] == 75) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int track = memLocation;
			while (mem[sp + track] != 32) {
				printf("%c", mem[sp + track]);
				track += 1;
			}
			j += 3;
		}
	//STRO stack deferred
		else if (mem[j] == 76) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int track = memLocation;
			while (mem[mem[sp + track]] != 32) {
				printf("%c", mem[mem[sp + track]]);
				track += 1;
			}
			j += 3;
		}
	//STRO indexed
		else if (mem[j] == 77) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int offSet = index * 2;
			int track = memLocation;
			while (mem[track + offSet] != 32) {
				printf("%c", mem[track + offSet]);
				track += 1;
			}
			j += 3;
		}
	//STRO indirect
		else if (mem[j] == 74) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int offSet = index * 2;
			int track = memLocation;
			while (mem[mem[track]] != 32) {
				printf("%c", mem[mem[track]]);
				track += 1;
			}
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
	//BRV = 32
		else if (mem[j] == 32) {
			if (v = 1) {
				int first = mem[j + 1];
				int second = mem[j + 2];
				int memLocation = extractAddressLocation(first, second);
				j = memLocation;
			}
			else {
				j += 3;
			}
		}

	//CP SECTION HERE
	//CPWa = 160 (i)
		else if (mem[j] == 160) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = accum - memLocation;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWa= 161 (d)
		else if (mem[j] == 161) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[memLocation], mem[memLocation + 1]);
			check = accum - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWx= 168 (i)
		else if (mem[j] == 168) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = index - memLocation;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWx= 169 (d)
		else if (mem[j] == 169) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[memLocation], mem[memLocation + 1]);
			check = index - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWA stack (s)
		else if (mem[j] == 163) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[sp + memLocation], mem[sp + memLocation + 1]);
			check = accum - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWA stack relative deffered = 164 (sf)
		else if (mem[j] == 164) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[mem[sp + memLocation]], mem[mem[sp + memLocation + 1]]);
			check = accum - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWa= 165 (x)
		else if (mem[j] == 165) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[memLocation + offSet], mem[memLocation + 1 + offSet]);
			check = accum - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWa= 166 (Sx)
		else if (mem[j] == 166) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[sp + memLocation + offSet], mem[sp + memLocation + 1 + offSet]);
			check = accum - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWa= 167 (Sfx)
		else if (mem[j] == 167) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[mem[sp + memLocation + offSet]], mem[mem[sp + memLocation + 1 + offSet]]);
			check = accum - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWX stack
		else if (mem[j] == 171) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[sp + memLocation], mem[sp + memLocation + 1]);
			check = index - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWX stack relative deffered = 172(sf)
		else if (mem[j] == 172) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[mem[sp + memLocation]], mem[mem[sp + memLocation + 1]]);
			check = index - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWX = 173 (x)
		else if (mem[j] == 173) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[memLocation + offSet], mem[memLocation + 1 + offSet]);
			check = index - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWX = 174 (Sx)
		else if (mem[j] == 174) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[sp + memLocation + offSet], mem[sp + memLocation + 1 + offSet]);
			check = index - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWX = 175 (Sfx)
		else if (mem[j] == 175) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[mem[sp + memLocation + offSet]], mem[mem[sp + memLocation + 1 + offSet]]);
			check = index - word;
			if (check < 1) {
				n = 1;
			}
		}
	//CPWa= 162 (n)
		else if (mem[j] == 162) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[mem[memLocation]], mem[mem[memLocation + 1]]);
			check = accum - word;
			if (check < 1) {
				n = 1;
			}

		}
	//CPWx= 170 (n)
		else if (mem[j] == 170) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int word;
			word = extractAddressLocation(mem[mem[memLocation]], mem[mem[memLocation + 1]]);
			check = index - word;
			if (check < 1) {
				n = 1;
			}
		}


	//CPB SECTION
	//CPba= 176 (i)
		else if (mem[j] == 176) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = accum - memLocation;
			if (check < 1) {
				n = 1;
			}
		}
	//CPba= 177 (d)
		else if (mem[j] == 177) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = accum - mem[memLocation];
			if (check < 1) {
				n = 1;
			}
		}
	//CPbx= 184(i)
		else if (mem[j] == 184) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = index - memLocation;
			if (check < 1) {
				n = 1;
			}
		}
	//CPbx= 185(d)
		else if (mem[j] == 185) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = index - mem[memLocation];
			if (check < 1) {
				n = 1;
			}
		}
	//CPba stack = 179 (s)
		else if (mem[j] == 179) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = accum - mem[sp + memLocation];
			if (check < 1) {
				n = 1;
			}
		}
	//CPba stack relative deffered = 180 (sf)
		else if (mem[j] == 180) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = accum - mem[mem[sp + memLocation]];
			if (check < 1) {
				n = 1;
			}
		}
	//CPba indexed = 181 (x)
		else if (mem[j] == 181) {
			int check;
			int offSet = index * 2;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = accum - mem[memLocation + offSet];
			if (check < 1) {
				n = 1;
			}
		}
	//CPba stack - indexed = 182(sx)
		else if (mem[j] == 182) {
			int check;
			int offSet = index * 2;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = accum - mem[sp + memLocation + offSet];
			if (check < 1) {
				n = 1;
			}
		}
	//CPba stack deffered indexed = 183 (sfx)
		else if (mem[j] == 183) {
			int check;
			int offSet = index * 2; 
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = accum - mem[mem[sp + memLocation + offSet]];
			if (check < 1) {
				n = 1;
			}
		}
	//CPbx stack = 187 (s)
		else if (mem[j] == 187) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = index - mem[sp + memLocation];
			if (check < 1) {
				n = 1;
			}
		}
	//CPbx stack relative deffered = 188 (sf)
		else if (mem[j] == 188) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = index - mem[mem[sp + memLocation]];
			if (check < 1) {
				n = 1;
			}
		}
	//CPbx indexed = 189 (x)
		else if (mem[j] == 189) {
			int check;
			int offSet = index * 2;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = index - mem[memLocation + offSet];
			if (check < 1) {
				n = 1;
			}
		}
	//CPbx stack - indexed = 190 (sx)
		else if (mem[j] == 190) {
			int check;
			int offSet = index * 2;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = index - mem[sp + memLocation + offSet];
			if (check < 1) {
				n = 1;
			}
		}
	//CPbx stack deffered indexed = 183 (sfx)
		else if (mem[j] == 183) {
			int check;
			int offSet = index * 2;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = index - mem[mem[sp + memLocation + offSet]];
			if (check < 1) {
				n = 1;
			}
		}
	//CPba= 178 (n)
		else if (mem[j] == 178) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = accum - mem[mem[memLocation]];
			if (check < 1) {
				n = 1;
			}
		}
	//CPbx= 186 (n)
		else if (mem[j] == 186) {
			int check;
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			check = index - mem[mem[memLocation]];
			if (check < 1) {
				n = 1;
			}
		}


	//STACK POINTER SECTION HERE
	//SUBSP SECTION
	//SUBSP Immediate = 58(hexa) = 88
		else if (mem[j] == 88) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			sp -= memLocation;
		}
	//SUBSP direct
		else if (mem[j] == 89) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			sp -= mem[memLocation];
		}
	//SUBSP stack
		else if (mem[j] == 91) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			sp -= mem[sp + memLocation];
		}
	//SUBSP Stack Defered = 92
		else if (mem[j] == 88) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			sp -= mem[mem[sp + memLocation]];
		}
	//SUBSP indexed
		else if (mem[j] == 93) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int offSet = index * 2; 
			sp -= mem[memLocation + offSet];
		}
	//SUBSP stack indexed
		else if (mem[j] == 94) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int offSet = index * 2;
			sp -= mem[sp + memLocation + offSet];
		}
	//SUBSP stack indexed deferred
		else if (mem[j] == 95) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int offSet = index * 2;
			sp -= mem[mem[sp + memLocation + offSet]];
		}
	//SUBSP indirect
		else if (mem[j] == 90) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
		
			sp -= mem[mem[memLocation]];
		}

	//ADDSP SECTION
	//ADDSP Immediate = 50(hexa) = 80
		else if (mem[j] == 80) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			sp += memLocation;
		}
	//ADDSP direct
		else if (mem[j] == 81) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			sp += mem[memLocation];
		}
	//ADDSP stack
		else if (mem[j] == 83) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			sp += mem[sp + memLocation];
		}
	//ADDSP Stack Deferred = 84
		else if (mem[j] == 80) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			sp += mem[mem[sp + memLocation]];
		}
	//ADDSP indexed
		else if (mem[j] == 85) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int offSet = index * 2;
			sp += mem[memLocation + offSet];
		}
	//ADDSP stack indexed
		else if (mem[j] == 86) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2; 
			sp += mem[sp + memLocation + offSet];
		}
	//ADDSP stack indexed deferred
		else if (mem[j] == 87) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			sp += mem[mem[sp + memLocation + offSet]];
		}
	//ADDSP indirect
		else if (mem[j] == 82) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			sp += mem[mem[memLocation]];
		}

	//ADDr SECTION
	//ADDA direct
		else if (mem[j] == 97) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum + mem[memLocation];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;

		}
	//ADDA stack 
		else if (mem[j] == 99) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum + mem[sp + memLocation];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDA stack relative deferred
		else if (mem[j] == 100) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum + mem[mem[sp + memLocation]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;

		}
	//ADDA immediate
		else if (mem[j] == 96) {//adda,i
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum + memLocation;
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;

		}
	//ADDA indexed
		else if (mem[j] == 101) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = accum + mem[memLocation + offSet];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDA stack indexed
		else if (mem[j] == 102) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = accum + mem[sp + memLocation + offSet];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDA stack indexed deferred
		else if (mem[j] == 103) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = accum + mem[mem[sp + memLocation + offSet]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDA indirect
		else if (mem[j] == 98) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = accum + mem[mem[memLocation]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDX direct
		else if (mem[j] == 105) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index + mem[memLocation];
			if (index > 32767) {
				v = 1;
			}
			if (index < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDX immediate
		else if (mem[j] == 104) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index + memLocation;
			if (index > 32767) {
				v = 1;
			}
			if (index < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDX stack
		else if (mem[j] == 107) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index + mem[sp + memLocation];
			if (index > 32767) {
				v = 1;
			}
			if (index < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDX stack relative deferred
		else if (mem[j] == 108) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index + mem[mem[sp + memLocation]];
			if (index > 32767) {
				v = 1;
			}
			if (index < -32768) {
				v = 1;
			}
			j += 3;

		}
	//ADDX indexed
		else if (mem[j] == 109) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			index = index + mem[memLocation + offSet];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDA stack indexed
		else if (mem[j] == 110) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			index = index + mem[sp + memLocation + offSet];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDX stack indexed deferred
		else if (mem[j] == 111) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			index = index + mem[mem[sp + memLocation + offSet]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//ADDX indirect
		else if (mem[j] == 106) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			index = index + mem[mem[memLocation]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}

	//SUBr SECTION
	//SUBA direct
		else if (mem[j] == 113) {//suba,d
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum - mem[memLocation];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBA immediate
		else if (mem[j] == 112) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum - memLocation;
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBA stack
		else if (mem[j] == 115) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum - mem[sp + memLocation];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBA stack relative deferred
		else if (mem[j] == 116) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			accum = accum - mem[mem[sp + memLocation]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBA indexed
		else if (mem[j] == 117) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = accum - mem[memLocation + offSet];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBA stack indexed
		else if (mem[j] == 118) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = accum - mem[sp + memLocation + offSet];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBA stack index deferred
		else if (mem[j] == 119) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = accum - mem[mem[sp + memLocation + offSet]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBA indirect
		else if (mem[j] == 114) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);

			int offSet = index * 2;
			accum = accum - mem[mem[memLocation]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBX direct
		else if (mem[j] == 121) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index - mem[memLocation];
			if (index > 32767) {
				v = 1;
			}
			if (index < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBX immediate
		else if (mem[j] == 120) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index - memLocation;
			if (index > 32767) {
				v = 1;
			}
			if (index < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBX stack
		else if (mem[j] == 123) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index - mem[sp + memLocation];
			if (index > 32767) {
				v = 1;
			}
			if (index < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBX stack relative deferred
		else if (mem[j] == 124) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			index = index - mem[mem[sp + memLocation]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBX indexed
		else if (mem[j] == 125) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int offSet = index * 2; 
			index = index - mem[memLocation + offSet];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBX stack indexed
		else if (mem[j] == 126) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int offSet = index * 2;
			index = index - mem[sp + memLocation + offSet];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBX stack index deferred
		else if (mem[j] == 127) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int offSet = index * 2;
			index = index - mem[mem[sp + memLocation + offSet]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}
	//SUBX indirect
		else if (mem[j] == 122) {
			int first = mem[j + 1];
			int second = mem[j + 2];
			int memLocation = extractAddressLocation(first, second);
			int offSet = index * 2;
			index = index - mem[mem[memLocation]];
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 3;
		}

	//AS SECTION
	//ASLA
		else if (mem[j] == 10) {
			accum = accum * 2;
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 1;
		}

	//ASLX
		else if (mem[j] == 11) {
			index = index * 2;
			if (index > 32767) {
				v = 1;
			}
			if (index < -32768) {
				v = 1;
			}
			j += 1;
		}

	//ASRA
		else if (mem[j] == 12) {
			accum = accum / 2;
			j += 1;
		}
	//ASRX
		else if (mem[j] == 13) {
			index = index / 2;
			j += 1;
		}

	//NOT and NEG SECTION
	//NOTA = 06 = 6
		else if (mem[j] == 6) {
			accum = ~accum;
			j += 1;
		}
	//NOTA = 07 = 7
		else if (mem[j] == 7) {
			index = ~index;
			j += 1;
		}
	//NEGA = 08 = 8
		else if (mem[j] == 8) {
			accum = ~accum;
			accum += 1;
			if (accum > 32767) {
				v = 1;
			}
			if (accum < -32768) {
				v = 1;
			}
			j += 1;
		}
	//NEGX = 09 = 9
		else if (mem[j] == 9) {
			index = ~index;
			index += 1;
			if (index > 32767) {
				v = 1;
			}
			if (index < -32768) {
				v = 1;
			}
			j += 1;
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
	//MOVSPA
		else if (mem[j] == 3) {
			accum = sp;
			j += 1;
		}
	//This else statement takes care of STOP/00
		else {
			j += 1;
		}

	//Addressing Modes to Add = indirect
	return 0;
}