#pragma once

int IsPrior(char c1, char c2);
int IsNumber(char c);
int IsOperator(char c);
int ToRPNExpression(char * s, char * res, int size, int * status);
double GetValue(char * s, double x, double y, int * status);
double Calculator(char * s, double x1, double y, int * status);
void PreProc(char * s, int * status);
