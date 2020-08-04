#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<ctype.h>
#include<math.h>
#include<locale.h>


void error(int errorId, FILE* filetemp);
void errorwithexit(int errorId, int num, ...);

struct Operation;
struct Stack_i;
struct Stack_c;

void CreateOperation(const char operation, const int priority,
	Operation** Arrops, unsigned int* countops);
int comparison(const char leftoperation, const char rightoperation,
	Operation** Arrops, const unsigned int countops);

void push(Stack_c** head, char data);
char popd(Stack_c** head);
void printStack(FILE* postfix_expression, Stack_c* head, FILE* temp);


void pushi(Stack_i** head, int data);
int popi(Stack_i** head);
