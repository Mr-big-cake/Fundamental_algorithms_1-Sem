#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<ctype.h>
#include<math.h>
#include<locale.h>
//#include"Header.h"
#pragma warning (disable: 4996)


#define SUBTRACTION 0
#define ADDITION 1
#define DIVISION 2
#define MULTIPLICATION 3
#define TAKING_THE_REMAINDER_FROM_DIVIDING 4
#define POW 5


#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101


#define FEW_ARGUMENTS 1
#define FIN_NO_OPEN 2
#define FOUT_NO_OPEN 3
#define OUT_OF_MEMORY 4
#define UNAVAILABLE_CHARACTER 5
#define ERROR_BRACKETS 6
#define ERROR_INT_OVERFLOW 7
const char* errors[] =
{
	//1
	"Few arguments\n",

	//2
	"The input file did not open\n",

	//3
	"The output file did not open\n",

	//4
	"Out of memmory\n",

	//5
	"Unavailable character\n",

	//6
	"The balance of brackets is broken\n",

	//7
	"Integer overflow occurred\n"
};

//void error(int errorId, FILE*);
//void error_with_exit(int errorId, unsigned num, ...);

void error(int errorId, FILE* filetemp) {
	fprintf(filetemp, "\nError #%d: %s \n", errorId, errors[errorId - 1]);
	system("pause");
	//exit(errorId);
}

//void errorwithexit(int errorId, int num, ...) {
//	va_list args;
//	va_start(args, num);
//	FILE* temp = va_arg(args, FILE*);
//	fprintf(temp, "\nError #%d: %s \n", errorId, errors[errorId - 1]);
//	fclose(temp);
//	while (num--) fclose(va_arg(args, FILE*));
//	va_end(args);
//	system("pause");
//	exit(errorId);
//
//}
//++++++++++++++++++++++++++++++++++++++++++++++++++


typedef struct Operation {
	int priority;
	char operation;
}Operation;


void CreateOperation(const char operation, const int priority,
	Operation** Arrops, unsigned int* countops) {
	Operation* temp = (Operation*)malloc(sizeof(Operation));
	temp->priority = priority;
	temp->operation = operation;
	Arrops[*countops] = temp;
	(*countops)++;
}

int comparison(const char leftoperation, const char rightoperation,
	Operation** Arrops, const unsigned int countops) {
	int leftpriority = -1, rightpriority = -1;
	unsigned int i;
	//printf("\n\nOTLADKA\ncuntops = %d\n", countops);
	for (i = 0; i < countops; i++) {
		//printf("%c, %d", Arrops[i]->operation, Arrops[i] ->priority);
		if (leftoperation == Arrops[i]->operation) leftpriority = Arrops[i]->priority;
		if (rightoperation == Arrops[i]->operation) rightpriority = Arrops[i]->priority;
		//if (leftoperation != -1 && rightoperation != -1) break;
	}
	if (leftpriority < rightpriority) return -1;
	else if (leftpriority > rightpriority) return 1;
	else return 0;
}


//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
typedef struct Stack_c {
	char data;
	struct Stack_c* next;
} Stack_c;

void push(Stack_c** head, char data) {
	Stack_c* tmp = (Stack_c*)malloc(sizeof(Stack_c));
	if (tmp == NULL) exit(STACK_OVERFLOW);
	tmp->next = *head;
	tmp->data = data;
	*head = tmp;
}


char popd(Stack_c** head) {
	Stack_c* out;
	char data;
	if (*head == NULL) exit(STACK_UNDERFLOW);
	out = *head;
	*head = (*head)->next;
	data = out->data;
	free(out);
	return data;
}

void printStack(FILE* postfix_expression, Stack_c* head, FILE* temp) {
	while (head) {
		if (head->data == '(') error(ERROR_BRACKETS, temp);
		fprintf(postfix_expression, " %c ", head->data);
		head = head->next;
	}
}


//______________________________________________________
typedef struct Stack_i {
	int data;
	struct Stack_i* next;
} Stack_i;

void pushi(Stack_i** head, int data) {
	Stack_i* tmp = (Stack_i*)malloc(sizeof(Stack_i));
	if (tmp == NULL) exit(STACK_OVERFLOW);
	tmp->next = *head;
	tmp->data = data;
	*head = tmp;
}


int popi(Stack_i** head) {
	Stack_i* out;
	int data;
	if (*head == NULL) exit(STACK_UNDERFLOW);
	out = *head;
	*head = (*head)->next;
	data = out->data;
	free(out);
	return data;
}


