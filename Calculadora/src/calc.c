#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calc.h"

int main ()
{
	int o /*Operação*/, a /*Primeiro Valor*/, b /*Segundo Valor*/, s /*Saída*/;
	float r=0 /*Resultado*/;
	
	for(;;)
	{
		printf ("\t\t***CALCULADORA***\n");
		printf ("1-SOMA / 2-SUBTRACAO / 3-MULTIPLICACAO / 4-DIVISAO\n");
		printf ("Digite a operacao que deve ser feita: ");
		scanf  ("%d", &o);
		
		if (o == 1)
		{
			printf ("Digite o primeiro valor: ");
			scanf  ("%d", &a);
			printf ("Digite o segundo valor: ");
			scanf  ("%d", &b);
			r=soma(a,b);
			printf ("%d + %d = %.2f", a, b, r);
		}
		else if (o == 2)
		{
			printf ("Digite o primeiro valor: ");
			scanf  ("%d", &a);
			printf ("Digite o segundo valor: ");
			scanf  ("%d", &b);
			r=sub(a,b);
			printf ("%d - %d = %.2f", a, b, r);
		}
		else if (o == 3)
		{
			printf ("Digite o primeiro valor: ");
			scanf  ("%d", &a);
			printf ("Digite o segundo valor: ");
			scanf  ("%d", &b);
			r=mult(a,b);
			printf ("%d x %d = %.2f", a, b, r);
		}
		else if (o == 4)
		{
			printf ("Digite o primeiro valor: ");
			scanf  ("%d", &a);
			printf ("Digite o segundo valor: ");
			scanf  ("%d", &b);
			r=divis(a,b);
			printf ("%d / %d = %.2f", a, b, r);
		}
		else
		{
			printf ("Funcao Invalida!");
		}
		printf ("\nPara retornar ao menu, digite 1. Para sair, digite 2.\n");
		scanf  ("%d", &s);
		if (s == 1) printf ("\n\n\n");
		else if (s == 2) break;
	}
}

float soma (int a, int b)
{
	return a+b;
}

float sub (int a, int b)
{
	return a-b;
}

float mult (int a, int b)
{
	return a*b;
}

float divis (int a, int b)
{
	return a/b;
}
