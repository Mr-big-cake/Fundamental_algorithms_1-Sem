#pragma warning(disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define COMMANDS_COUNT 3
#define OBJECT_COUNT 3
#define MAX_LEN_TOKEN 7
#define MIN_LEN_TOKEN 3


typedef struct Shore {
	int wolf;
	int goat;
	int cabbage;
	int boat;
}Shore;

typedef struct{
	int wolf;
	int goat;
	int cabbage;
	int shore;
}Boat;

Shore coast1, coast2;
Boat boat;
char object;

const char* errors[] =
{
	"I can`t find all arguments\n",
	"I can`t find first input file\n"
	"I can`t find second input file\n",
	"Command is not found\n",
	"There's no room in the boat\n",
	"The specified object is not on this shore\n",
	"There is no object in the boat\n",
	"The wolf ate the goat",
	"The goat ate the cabbage"
};
void error(int number)
{
	printf("Error #%d: %s \n", number, errors[number - 1]);
	exit(number);
}
const char* com[COMMANDS_COUNT] = {
	"take", "put", "move"
};
const char* objects[OBJECT_COUNT] = {
	"wolf", "goat", "cabbage"
};
void create_task();
void get();
void take();
void put();
void move();
void (*operations[3])() = { take, put, move };
void comparison_of_tokens(char*, unsigned*, char*);

int main(int argc, char* argv[]) {
	FILE *fin1, * fin2;
	unsigned k = 10;
	char c, *instruction = (char*)malloc(pow(2, k));
	unsigned count = 0;
	char buf[MAX_LEN_TOKEN + 1], * b = buf;
	unsigned flag_space = 0, flag_first_space = 1, flag_last_space = 0;
	create_task();
//---------------------------------------------------------------------------------------
	if (argc > 3) error(1);
	fin1 = fopen(argv[2], "r");
	if (!fin1) error(2);
	while ((c = fgetc(fin1)) != EOF) {
		if (flag_first_space && isspace(c)) while (isspace(c = fgetc(fin1))) if (c == EOF) break;
		if (isspace(c) && flag_space != 0) continue;
		else if (isspace(c) && flag_space == 0) flag_space++;
		else flag_space = 0;
		if (isspace(c)) {
			if(b - buf < MIN_LEN_TOKEN) error(3);
			*b = 0;
			b = buf;
			printf("<%s>\n", buf);
			comparison_of_tokens(instruction, &count, buf);
			flag_last_space = 0;
			if (pow(2, k) == count) instruction = (char*)realloc(instruction, pow(2, ++k));
			continue;
		}
		*b++ = tolower(c);
		if (b - buf > MAX_LEN_TOKEN) error(3);
		if(flag_first_space) flag_first_space = 0;
		flag_last_space = 1;
	}
	if (flag_last_space == 1) {
		*b = 0;
		printf("<%s>\n", buf);
		comparison_of_tokens(instruction, &count, buf);
	}
	instruction[count] = 0;
	if (instruction[count - 1] == '0') error(3);
	if (coast2.wolf && coast2.goat && coast2.cabbage && boat.shore == 2) 
		printf("\n\n\n!!!________________The task is solved________________!!!\n");
	else
		printf("\n\n\n!!!________________The task is NOT solved________________!!!\n");
	//printf("<%s>, %d\n---------------------\n", instruction, count);
	//get();
	fclose(fin1);
	return 0;
}
//_______________________________________________________________________________________
//_______________________________________________________________________________________
//_______________________________________________________________________________________
//_______________________________________________________________________________________

//_______________________________________________________________________________________
//_______________________________________________________________________________________
//_______________________________________________________________________________________
//_______________________________________________________________________________________
void create_task() {
	coast1.wolf = 1;
	coast1.goat = 1;
	coast1.cabbage = 1;
	coast1.boat = 1;
	coast2.wolf = 0;
	coast2.goat = 0;
	coast2.cabbage = 0;
	coast2.boat = 0;
	boat.wolf = 0;
	boat.goat = 0;
	boat.cabbage = 0;
	boat.shore = 1;
}

void get() {
	printf("\
coast1.wolf = %d\n\
coast1.goat = %d\n\
coast1.cabbage = %d\n\n\
coast2.wolf = %d\n\
coast2.goat = %d\n\
coast2.cabbage = %d\n\n\
boat.wolf = %d\n\
boat.goat = %d\n\
boat.cabbage = %d\n\
boat.shore = %d\n______________________\n",
coast1.wolf,
coast1.goat,
coast1.cabbage,
coast2.wolf,
coast2.goat,
coast2.cabbage,
boat.wolf,
boat.goat,
boat.cabbage,
boat.shore);
}

void move() {
	Shore* temp;
	boat.shore == 1 ? temp = &coast1 : temp = &coast2;
	boat.shore == 1 ? boat.shore = 2 : boat.shore = 1;
	if (temp->wolf && temp->goat) error(7);
	if (temp->cabbage && temp->goat) error(8);
	get();
}

void put() {

	Shore* temp;
	if (!(boat.wolf) && !(boat.goat) && !(boat.cabbage)) error(6);
	boat.shore == 1 ? temp = &coast1 : temp = &coast2;
	if (boat.wolf == 1) {
		temp->wolf += 1;
		boat.wolf -= 1;
	}
	else if (boat.goat == 1) {
		temp->goat += 1;
		boat.goat -= 1;
	}
	else if (boat.cabbage == 1) {
		temp->cabbage += 1;
		boat.cabbage -= 1;
	}
	get();
}

void take() {

	Shore* temp;
	unsigned obj = object - 'a';
	boat.shore == 1 ? temp = &coast1 : temp = &coast2;
	if (boat.wolf || boat.goat || boat.cabbage) {
		error(4);
	}
	switch (obj)
	{
	case 0:
		if (temp->wolf == 0) error(5);
		temp->wolf -= 1;
		boat.wolf += 1;
		break;
	case 1:
		if (temp->goat == 0) error(5);
		temp->goat -= 1;
		boat.goat += 1;
		break;
	case 2:
		if (temp->cabbage == 0) error(5);
		temp->cabbage -= 1;
		boat.cabbage += 1;
		break;
	}
	get();
}

void comparison_of_tokens(char* instruction, unsigned* count, char* buf) {
	unsigned is_first_instruction, i, is_true_instruction = 0;
	if (*count == 0) {
		for (i = 0; i < 3; i++)
			if (!strcmp(com[i], buf)) {
				instruction[*count] = i + '0';
				(*count)++;
				is_true_instruction = 1;
				operations[i]();
				break;
			}
	}
	else if (instruction[(*count) - 1] != '0' && !strcmp(com[0], buf)) {
		instruction[*count] = '0';
		(*count)++;
		is_true_instruction = 1;
	}
	else if (instruction[(*count) - 1] != '0') {
		for (i = 1; i < 3; i++)
			if (!strcmp(com[i], buf)) {
				instruction[*count] = i + '0';
				(*count)++;
				operations[i]();
				is_true_instruction = 1;
				break;
			}
	}
	else if (instruction[(*count) - 1] == '0') {
		for (i = 0; i < 3; i++)
			if (!strcmp(objects[i], buf)) {
				instruction[*count] = i + 'a';
				object = instruction[*count];
				(*count)++;
				is_true_instruction = 1;
				operations[0]();
				break;
			}
	}
	if (!is_true_instruction) error(3);
}