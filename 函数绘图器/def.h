#pragma once

/*error code definition for RPN.cpp*/
#define SUCCEED       0
#define MATH_ERROR   -1
#define SYNTAX_ERROR -2
#define PARA_ERROR   -3

/*data structure difinition*/
typedef struct Function
{
	char Expression[1024];
	COLORREF cGraph;
}FUNC;
