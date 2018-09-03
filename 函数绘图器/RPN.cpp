#include <string.h>
#include <malloc.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <errno.h>
#include "def.h"
#include "resource.h"

extern HWND hDebug;

int IsPrior(char c1, char c2)
{
	switch (c1)
	{
	case '+':
	case '-':
		if (c2 != '+' && c2 != '-' && c2 != '*' && c2 != '/' && c2 != '^')
			return (-1);
		return 0;
	case '*':
	case '/':
		if (c2 == '+' || c2 == '-')
			return 1;
		else if (c2 == '*' || c2 == '/' || c2 == '^')
			return 0;
		else
			return (-1);
    case '^':
        if (c2 != '+' && c2 != '-' && c2 != '*' && c2 != '/' && c2 != '^')
			return (-1);
        return 1;
	default:
		return (-1);

	}

	return (-1);
}

int IsNumber(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	else
		return 0;
}

int IsOperator(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
		return 1;
	else
		return 0;
}

int ToRPNExpression(char * s, char * res, int size, int * status)
{
	if (s == NULL)
    {
        *status = PARA_ERROR;
        return 0;
    }

	char stack[1024];
	int len;
	int i;
	int index_res = 0;
	int index_stack = -1;

	memset(res, 0, size);
	len = strlen(s);

	for (i = 0; i < len; ++i)
	{
		if (IsNumber(s[i]))
		{
			while (IsNumber(s[i]) || s[i] == '.')
			{
				res[index_res++] = s[i];
				i++;

			}
			res[index_res++] = ' ';
			i--;
			continue;
		}
		else if (IsOperator(s[i]))
		{
			if (index_stack == -1 || IsPrior(s[i], stack[index_stack]))
			{
				stack[++index_stack] = s[i];
			}
			else
			{
				while (index_stack != -1 && !IsPrior(s[i], stack[index_stack]))
				{
					res[index_res++] = stack[index_stack--];
					res[index_res++] = ' ';
				}
				stack[++index_stack] = s[i];
			}
		}
		else if (s[i] == '(')
		{
			stack[++index_stack] = s[i];
		}
		else if (s[i] == ')')
		{
			while (index_stack != -1 && stack[index_stack] != '(')
			{
				res[index_res++] = stack[index_stack--];
				res[index_res++] = ' ';
			}
			if (index_stack == -1)
            {
                *status = SYNTAX_ERROR;
                return 0;
            }
			index_stack--;
		}
		else if (s[i] == 'x' ||s[i] == 'y')
        {
            res[index_res++] = s[i];
            res[index_res++] = ' ';
        }
		else if (s[i] == ' ')
        {
            continue;
        }
        else
        {
            *status = SYNTAX_ERROR;
            return 0;
        }
	}

	while (index_stack != -1)
	{
		res[index_res++] = stack[index_stack--];
		res[index_res++] = ' ';
	}

    *status = SUCCEED;
	return 1;
}

double GetValue(char * s, double x, double y, int * status)
{
    ///char buf[100];

	double stack[256] = {0};
	int len = strlen(s);
	char * t = NULL;
	double res;
	int cnt;
	int index = -1;
	int i, j;

	for (i = 0; i <len; ++i)
	{
		if (IsNumber(s[i]))
		{

			cnt = 0;
			j = i;
			while (IsNumber(s[j]) || s[j] == '.')
			{
				cnt++;
				j++;
			}
			t = (char *)malloc(sizeof(char) * (cnt+1));
			cnt = 0;
			while (IsNumber(s[i]) || s[i] == '.')
			{
				t[cnt++] = s[i];
				i++;
			}
			t[cnt] = '\0';    ///important!!!   这里一定要加'\0'!

			stack[++index] = atof(t);
			free(t);
		}
		else if (IsOperator(s[i]))
		{
		    if (index < 1)
            {
                *status = SYNTAX_ERROR;
                return 0;
            }
			switch (s[i])
			{
			case '+':
				res = stack[index-1] + stack[index];
				break;
			case '-':
				res = stack[index-1] - stack[index];
				break;
			case '*':
				res = stack[index-1] * stack[index];
				break;
			case '/':
			    if (fabs(stack[index]-0) <= 1e-10)
                {
                    *status = MATH_ERROR;
                    return 0;
                }
				res = stack[index-1] / stack[index];
				break;
            case '^':
                errno = 0;
                res = pow(stack[index-1], stack[index]);
                if (errno != 0)
                {
                    *status = MATH_ERROR;
                    return 0;
                }
                break;
			}
			index -= 1;
			stack[index] = res;
		}
		else if (s[i] == 'x')
        {
            stack[++index] = x;
        }
		else if (s[i] == ' ')
        {
            continue;
        }
        else
        {
            *status = SYNTAX_ERROR;
            return 0;
        }
	}

    *status = SUCCEED;
	return stack[index];
}

double Calculator(char * s, double x1, double y, int * status)
{
	char res[1024];
	double x;

	ToRPNExpression(s, res, 1024, status);
	if (*status != SUCCEED)
        return 0;

    x = GetValue(res, x1, y, status);
    if (* status!= SUCCEED)
        return 0;

	return x;
}

void PreProc(char * s, int * status)
{
	int len = strlen(s);
	if (len == 0)
	{
		*status = SYNTAX_ERROR;
		return;
	}

	int i, j;

	/*-1+2; +5-3*2*/
	if (s[0] == '-' || s[0] == '+')
	{
		for (i = len - 1; i >= 0; --i)
			s[i + 1] = s[i];
		s[0] = '0';
		s[len + 1] = '\0';
	}

	/*(-12)*3*/
	len = strlen(s);
	for (i = 0; i < len; ++i)
	{
		if (s[i] == '(' && (s[i + 1] == '-' || s[i + 1] == '+'))
		{
			for (j = len - 1; j >= i + 1; --j)
				s[j + 1] = s[j];
			s[len + 1] = '\0';
			s[i + 1] = '0';
			len = strlen(s);
		}
	}
	/*3(6+5)*/
	len = strlen(s);
	for (i = 0; i < len; ++i)
	{
		if ((IsNumber(s[i]) || s[i] == 'x' || s[i] == 'y') && s[i + 1] == '(')
		{
			for (j = len - 1; j >= i + 1; --j)
				s[j + 1] = s[j];
			s[i + 1] = '*';
			s[len + 1] = '\0';
			len = strlen(s);
		}
	}

	/*(1+2)(3+4)*/
	len = strlen(s);
	for (i = 0; i < len; ++i)
	{
		if (s[i] == ')' && s[i + 1] == '(')
		{
			for (j = len - 1; j >= i + 1; --j)
				s[j + 1] = s[j];
			s[i + 1] = '*';
			s[len + 1] = '\0';
			len = strlen(s);
		}
	}

	/*2x+1*/
	len = strlen(s);
	for (i = 0; i < len; ++i)
	{
		if (IsNumber(s[i]) && (s[i + 1] == 'x' || s[i + 1] == 'y'))
		{
			for (j = len - 1; j >= i + 1; --j)
				s[j + 1] = s[j];
			s[i + 1] = '*';
			s[len + 1] = '\0';
			len = strlen(s);
		}
	}
}
