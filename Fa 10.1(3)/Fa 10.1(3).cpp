#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_INSTRUCTION_LENGTH 11
#define COMMANDS_COUNT 12
#pragma warning(disable: 4996)
void error(int errorId);

#define ISGOOD(c) (isalnum(c) || c == ':' || c == '=' || \
c == '+' || c == '&' || c == '-' || c == '<' || c == '>' || \
c == '~' || c == '?' || c == '!' || c == '\\' || c == ',' || \
c == '(' || c == ')')

const char* operation[COMMANDS_COUNT] =
{
	"+>", "&", "->", "<-", "<>", "~", "+", "?", "!", "\\"
	, "READ", "WRITE"
}, ** ptr_operation = operation;

const char* errors[] =
{
	"Input file not found!!1!1",	    // 1)
	"Multiline comment wasn't opened",  // 2)
	"Multiline comment wasn't closed",  // 3)
	"Invalid instruction",			    // 4)
	"Command wasn't found",			    // 5)
	"Too few arguments in command line",// 6)
	"Variable wasn't initialized",		// 7)
	"Incorrect base"					// 8)
	"Output file is not found!"		    // 9)
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

char* reverse(char* buf)
{
	char* pt1, * pt2, c;
	pt1 = buf - 1;
	pt2 = buf + strlen(buf);
	while (--pt2 > ++pt1)
	{
		c = *pt2;
		*pt2 = *pt1;
		*pt1 = c;
	}
	return buf;
}

int All_in_Ten(char* num, int base_in) 
{
	char* ptr = num;
	int EntrNum = 0, flag = 0, ChangeSign = 1;
	if (*ptr == '-') ChangeSign = -1;
	else ptr--;
	while (*++ptr)
	{
		if (isalpha(*ptr) && (toupper(*ptr) - 'A' + 10) < base_in)
		{
			EntrNum += toupper(*ptr) - 'A' + 10;
		}
		else if (isdigit(*ptr) && *ptr - '0' < base_in)
		{
			EntrNum += *ptr - '0';
		}
		else
		{
			flag = 1;
			break;
		}

		EntrNum *= base_in;
	}
	return flag ? 0 : EntrNum / base_in * ChangeSign;
}

char* Ten_in_All(int num, int base_out)
{
	char* BufForNum = (char*)malloc(5), * ptr = BufForNum;;
	int EntrNum = num, flag = 0;
	if (!EntrNum) flag = 1;
	while (EntrNum && flag == 0)
	{
		*ptr++ = EntrNum % base_out < 10 ? EntrNum % base_out + '0' : EntrNum % base_out - 10 + 'A';
		EntrNum /= base_out;
		*(ptr + 1) = 0;
	}
	return flag ? (char*)"0" : reverse(BufForNum);
}

void error(int errorId)
{
	printf("Error #%d: %s \n", errorId, errors[errorId - 1]);
	system("pause");
	exit(errorId);
}



int main(int argc, char* argv[])
{


	FILE* fIn, * fout;
	char c;
	char arg1, arg2, arg3;
	int inMultilineComment = 0;
	char buf[MAX_INSTRUCTION_LENGTH + 1], * b = buf;
	unsigned int alphabet[26], is_init = 0;
	int base, k, num, temp;
	char bufstr[BUFSIZ], * bs = bufstr;
	if (argc < 3)
	{
		error(6);
	}

	if (!(fIn = fopen(argv[1], "r")))
	{
		error(1);
	}

	if (!(fout = fopen(argv[2], "w")))
	{
		error(9);
	}
	while (!feof(fIn))
	{
		c = fgetc(fIn);
		if (c == '%' && !inMultilineComment)
		{
			while ((c = fgetc(fIn)) != '\n' && c != EOF);
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
			printf("%s\n", buf);//принтим инстуркцию
			b = buf;// обнуляем буфер
			fprintf(fout, "Command: %s\n", buf);

			if (!isalpha(*b++)) // первый символ - не буква
			{
				error(4);
			}
			if (*b == ':')
			{
				arg1 = buf[0]; // присваивание аргумента 
				if (*++b != '=')
				{
					error(4);
				}

				if (*++b == '\\') //если НЕ
				{
					if (!isalpha(*++b)) error(4);
					arg2 = buf[4];
					if (!(is_init & (1 << (arg2 - 'A')))) error(7);
					is_init = is_init | 1 << (arg1 - 'A');
					alphabet[arg1 - 'A'] = !alphabet[arg2 - 'A'];
					fprintf(fout, "++++++%c = %d, %c = %d\n", arg1, alphabet[arg1 - 'A'], arg2, alphabet[arg2 - 'A']);
					b = buf;
					continue;
				}
			}
		}
	}
}
