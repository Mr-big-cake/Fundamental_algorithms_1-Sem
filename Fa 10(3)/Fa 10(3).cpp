#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_INSTRUCTION_LENGTH 11
#define OPERATIONS_COUNT 12
#pragma warning(disable: 4996)
void error(int errorId);
#define ISGOOD(c) (isalnum(c) || c == ':' || c == '=' || \
c == '+' || c == '&' || c == '-' || c == '<' || c == '>' || \
c == '~' || c == '?' || c == '!' || c == '\\' || c == ',' || \
c == '(' || c == ')')

const char* operation[OPERATIONS_COUNT] =
{
	"+>", "&", "->", "<-", "<>", "~", "+", "?", "!", "\\"
	, "READ", "WRITE"
}, ** ptr_operation = operation;

const char* errors[] =
{
	"Input file not found!", // 1)
	"Multiline comment wasn't opened", // 2)
	"Multiline comment wasn't closed", // 3)
	"Invalid instruction", // 4)
	"Command wasn't found", // 5)
	"Too few arguments in command line", // 6)
	"Variable wasn't initialized", // 7)
	"Incorrect base" // 8)
	"Output file is not found!" // 9)
};

int commitcom(int B, int C, int comcount)
{
	switch (comcount)
	{
	case 0:
	{	return B & ~C; }
	case 1:
	{return B & C; } 
	case 2:
	{return ~B | C; }
	case 3:
	{return B | ~C; }
	case 4:
	{return B ^ C; }
	case 5:
	{if (B == C) return 1;
	else return 0; }
	case 6:
	{return B | C; }
	case 7:
	{return ~(B & C); }
	case 8:
	{return ~(B | C); }

	}
}


void error(int errorId)
{
	printf("Error #%d: %s \n", errorId, errors[errorId - 1]);
	system("pause");
	exit(errorId);
}



int main(int argc, char* argv[])
{


	FILE* fin, * fout;
	char c;
	char arg1, arg2, arg3;
	int inMultilineComment = 0;
	char buf[MAX_INSTRUCTION_LENGTH + 1], * b = buf;
	unsigned int alphabet[26], is_init = 0;
	unsigned int i, ucount = 0;
	int base, k, num, temp;
	char bufstr[BUFSIZ], * bs = bufstr;
	if (argc < 3)
	{
		error(6);
	}

	if (!(fin = fopen(argv[1], "r")))
	{
		error(1);	
	}

	if (!(fout = fopen(argv[2], "w")))
	{
		error(9);
	}
	while (!feof(fin))
	{
		c = fgetc(fin);
		if (c == '%' && !inMultilineComment)
		{
			while ((c = fgetc(fin)) != '\n' && c != EOF);
		}
		else if (c == '{')
		{
			inMultilineComment++;
		}
		else if (c == '}')
		{
			inMultilineComment--;
			if (inMultilineComment == -1)
			{
				error(2);
			}
		}
		else if (c == EOF)
		{
			if (inMultilineComment)
			{
				error(3);
			}
		}
		else if (ISGOOD(c) && !inMultilineComment)
		{
			*b++ = toupper(c);
			if (b - buf > MAX_INSTRUCTION_LENGTH) // не превышает ли интрукция максимально длины 
			{
				error(4);
			}
		}
		else if (c == ';' && !inMultilineComment) //если конец команды 
		{
			*b = 0; // в конце завершающий ноль
			printf("%s\n", buf); 
			b = buf;// обнуляем буфер
			fprintf(fout, "Command: %s\n", buf);

			if (!isalpha(*b++)) // {<>?D := C;
			{
				error(4);
			}
			if (*b == ':')// A = B
			{
				arg1 = buf[0]; // присваивание аргумента 
				if (*++b != '=') // A : B
				{
					error(4);
				}

				if (*++b == '\\')
				{
					if (!isalpha(*++b)) error(4);
					arg2 = buf[4];
					if (!(is_init & (1 << (arg2 - 'A')))) error(7);
					is_init = is_init | 1 << (arg1 - 'A');
					alphabet[arg1 - 'A'] = 0;
					for (i = 0; i < 32; i++)
					{
						if ((1 << i) & alphabet[arg2 - 'A']) ucount = i + 1;
					}
					for (i = 0; i < ucount; i++)
					{
						if (!((1 << i) & alphabet[arg2 - 'A'])) alphabet[arg1 - 'A'] |= 1 << i;
					}
					fprintf(fout, "%c = %d, %c = %d\n, ucount = %d\n", arg1, alphabet[arg1 - 'A'], arg2, alphabet[arg2 - 'A'], ucount);
					b = buf; 
					continue;
				}

				if (!isalpha(*b++)) error(4); // A := 10
				for (ptr_operation = operation; ptr_operation - operation < OPERATIONS_COUNT - 3; ptr_operation++)
				{
					if (strstr(buf, *ptr_operation) == b) break; // из первых 10
				}

				if (ptr_operation - operation == OPERATIONS_COUNT - 2)// неизвестная команда
				{
					error(5);
				}
				b += strlen(*ptr_operation);
				if (!isalpha(*b)) error(4);
				arg2 = buf[3];
				arg3 = *b;
				//printf("\n %c %c",buf[0]);
				if ((!(is_init & 1 << (arg2 - 'A')))|| (!(is_init & 1 << (arg3 - 'A')))) error(7);
				is_init = is_init | 1 << (arg1 - 'A');
				alphabet[arg1 - 'A'] = commitcom(alphabet[arg2 - 'A'], alphabet[arg3 - 'A'], ptr_operation - operation);
				fprintf(fout, "%c = %d, %c = %d, %c = %d\n", arg1, alphabet[arg1 - 'A'], arg2, alphabet[arg2 - 'A'], arg3, alphabet[arg3 - 'A']);
				b = buf;
				continue;

			}
			else if (isalpha(*b))
			{

				for (ptr_operation = operation + 10; ptr_operation - operation < OPERATIONS_COUNT; ptr_operation++)
				{
					if (strstr(buf, *ptr_operation) == buf)
					{
						break;
					}
				}
				if (ptr_operation - operation == OPERATIONS_COUNT)
				{
					error(5);
				}

				
				b = buf + strlen(*ptr_operation);
				if (*b++ != '(')
				{
					error(4);
				}
				if (!isalpha(arg1 = *b++))
				{
					error(4);
				}
				if (*b++ != ',')
				{
					error(4);
				}
				while (isdigit(*b))
				{
					b++;
				}
				if (*b != ')' || strlen(b) != 1)
				{
					error(4);
				}
				*b = 0;
				base = 0;
				k = 1;
				while (isdigit(*--b))
				{
					base += (*b - '0') * k;
					k *= 10;
				}
				if (base < 2 || base > 32) error(8);


				if (ptr_operation - operation == 10)
				{
					printf("\nEnter meaning of %c in base %d\n", arg1, base);
					scanf("%s", bufstr);
					num = 0;
					bs = bufstr - 1;
					while (*++bs)
					{
						temp = isdigit(*bs) ? *bs - '0' : *bs - 'A' + 10;
						if (temp >= base || temp < 0) error(4);
						num = num * base + temp;
					}
					is_init = is_init | 1 << (arg1 - 'A');
					alphabet[arg1 - 'A'] = num;
					fprintf(fout, "Input: %c = %d\n", arg1, alphabet[arg1 - 'A']);
				}

				else
				{
					if (!(is_init & 1 << (arg1 - 'A'))) error(7);
					num = alphabet[arg1 - 'A'];
					bs = bufstr + BUFSIZ - 1;
					*bs = 0;
					if (!num)
					{
						*--bs = '0';
					}
					else
					{
						while (num)
						{
							*--bs = num / base < 10 ? num % base + '0' : num % base - 10 + 'A';
							num /= base;
						}
					}
					printf("%c is %s \n", arg1, bs);
					fprintf(fout, "Output:%c is %s \n", arg1, bs);
				}


				b = buf;
				bufstr[0] = '\0';
				continue;

			}
			else
			{
				error(4);
			}
		}

	}

	fprintf(fout, "The program was successfully completedlly");
	fclose(fin);
	fclose(fout);
	system("pause");
	return 0;
}