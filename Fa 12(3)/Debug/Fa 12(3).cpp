#include "Functions and structures.h"
#define INT_ZEROS 9
#define COUNT_OPERATION 6
#define COUNT_FILES 3
#define IS_OPERATOR(c) (c == '-' || c == '+' || c == '/' || c == '*' || c == '%' || c == '^')


Operation * *Arrops = (Operation * *)malloc(sizeof(Operation*) * COUNT_OPERATION);
unsigned int countops = 0;

int commitcom(unsigned B, unsigned A, int opcount, FILE* fout)
{
	unsigned out;
	switch (opcount) {
	case SUBTRACTION:
		return (A - B);
		break;
	case ADDITION:
		if (A + B == 0 && A != 0 && B != 0) error(ERROR_INT_OVERFLOW, fout);
		return (A + B);
		break;
	case DIVISION:
		return (A / B);
		break;
	case MULTIPLICATION:
		if (A * B == 0 && A != 0 && B != 0) error(ERROR_INT_OVERFLOW, fout);
		return (A * B);
		break;
	case TAKING_THE_REMAINDER_FROM_DIVIDING:
		return (A % B);
		break;
	case POW:
		out = pow(A, B);
		if (pow(A, B) == 0 && A != 0) error(ERROR_INT_OVERFLOW, fout);
		return out;
	}
}




int main(int argc, char* argv[]) {
	FILE* fin1, * fin2, * fout;
	unsigned positiondigit, flagspace, emptiness;
	unsigned countdigit = 0;
	Stack_c* headop = NULL;
	Stack_i* headargs = NULL;
	char c = 0, cf = 0;
	char  buf[INT_ZEROS] = { 0 };
	int i = 0, j;
	char tempop;
	int tempint = -1;


	CreateOperation('-', 1, Arrops, &countops);
	CreateOperation('+', 1, Arrops, &countops);
	CreateOperation('/', 2, Arrops, &countops);
	CreateOperation('*', 2, Arrops, &countops);
	CreateOperation('%', 2, Arrops, &countops);
	CreateOperation('^', 3, Arrops, &countops);

	/*if (argc < COUNT_FILES + 1) errorwithexit(FEW_ARGUMENTS, 0);
	if (!(fin1 = fopen(argv[1], "r"))) errorwithexit(FIN_NO_OPEN, 0);
	if (!(fout = fopen(argv[2], "w"))) errorwithexit(FOUT_NO_OPEN, 1, fin1);
	if (!(fin2 = fopen(argv[2], "r"))) errorwithexit(FOUT_NO_OPEN, 2, fin1, fin2);*/

	if (!(fout = fopen(argv[2], "w"))) error(FOUT_NO_OPEN, fout);
	if (argc < COUNT_FILES + 1) error(FEW_ARGUMENTS, fout);
	if (!(fin1 = fopen(argv[1], "r"))) error(FIN_NO_OPEN, fout);

	if (!(fin2 = fopen(argv[2], "r"))) error(FOUT_NO_OPEN, fout);

	while (c != EOF) {
		positiondigit = 2;
		emptiness = 1;
		flagspace = 0;
		countdigit = 0;
		headop = NULL;
		while (((c = fgetc(fin1)) != EOF) && c != '\n') {
			if (!IS_OPERATOR(c) && c != ')' && c != '(' && !isdigit(c) && !isspace(c))
				error(UNAVAILABLE_CHARACTER, fout);
			if (isspace(c)) {
				flagspace = 1;
				countdigit = 0;
				continue;
			}
			else if (isdigit(c)) {
				if (positiondigit == 0 && flagspace == 1) error(UNAVAILABLE_CHARACTER, fout);
				fprintf(fout, "%c", c);
				if (++countdigit > INT_ZEROS) error(ERROR_INT_OVERFLOW, fout);
				positiondigit = 0;
				flagspace = 0;
				emptiness = 0;
				continue;
			}
			else if (positiondigit == 1 && c != ')') {
				positiondigit = 2;
				flagspace = 0;
			}
			else if (positiondigit == 0) {
				fprintf(fout, " ");
				if (c != ')')positiondigit = 1;
				flagspace = 0;
			}
			emptiness = 0;
			countdigit = 0;

			if (c == '(') {
				if (positiondigit != 2) error(UNAVAILABLE_CHARACTER, fout);
				push(&headop, c);
			}
			else if (c == ')') {
				if (positiondigit == 1) error(UNAVAILABLE_CHARACTER, fout);
				if (headop == NULL) error(ERROR_BRACKETS, fout);
				flagspace = 1;
				while (headop->data != '(') {
					//*pb++ = popd(&headop);
					fprintf(fout, "%c ", popd(&headop));
					if (headop == NULL) error(ERROR_BRACKETS, fout);
				}
				popd(&headop);
			}
			else if (positiondigit == 2) error(UNAVAILABLE_CHARACTER, fout);
			else if (headop == NULL || headop->data == '(') push(&headop, c);
			else if (comparison(c, headop->data, Arrops, countops) == 1) push(&headop, c);
			else {
				while (headop->data != '(') {
					fprintf(fout, "%c ", popd(&headop));
					if (headop == NULL) break;
					if (comparison(c, headop->data, Arrops, countops) == 1) break;
				}
				push(&headop, c);
			}

		}
		if (positiondigit != 0 && emptiness == 0) error(ERROR_BRACKETS, fout);
		printStack(fout, headop, fout);
		fprintf(fout, " \n");
	}
	fclose(fout);
	if (!(fout = fopen(argv[2], "r"))) {
		fclose(fin1);
		error(FOUT_NO_OPEN, fout);
	}
	while (1) {
		cf = fgetc(fout);
		if (isdigit(cf)) {
			if (tempint == -1) tempint = 0;
			tempint = tempint * 10 + cf - '0';
			//printf("tempint = %d \n", tempint);
		}
		else if (cf == ' ' && tempint != -1) {

			pushi(&headargs, tempint);
			//printf("headargs->data = %d\n", headargs->data);
			tempint = -1;
		}
		else if (IS_OPERATOR(cf)) {
			if (cf == '-') tempop = SUBTRACTION;
			else if (cf == '+') tempop = ADDITION;
			else if (cf == '/') tempop = DIVISION;
			else if (cf == '*') tempop = MULTIPLICATION;
			else if (cf == '%') tempop = TAKING_THE_REMAINDER_FROM_DIVIDING;
			else if (cf == '^') tempop = POW;
			int a = popi(&headargs);
			int b = popi(&headargs);
			pushi(&headargs, commitcom(a, b, tempop, fout));
		}
		else if (cf == '#') while ((cf = fgetc(fout)) != EOF || cf != '\n')
			printf("%c", cf);
		else if (cf == '\n' || cf == EOF) {
			if (headargs != NULL) {
				printf("= %d\n", popi(&headargs));
				headargs = NULL;
			}
			if (cf == EOF) break;
		}
	}
}