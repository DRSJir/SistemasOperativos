#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

double calcular_promedio(int *promedio);

//argv = ["0"|"10"|"4"|"5"|"8"|...|NULL]
int main(int argc, char const *argv[])
{
	int calificaciones[argc];
	double promedio;
	int i;

	//Convertir argumentos a un número
	for (i = 0; i < argc-1; i++)
	{
		calificaciones[i] = atoi(argv[i]);
	}

	promedio = calcular_promedio(calificaciones);

	//devolver promedio
	exit(promedio);
	return 0;
}

double calcular_promedio(int *promedio)
{
	double resultado = 0;
	int i;

	for (int i = 0; i < 10; i++)
	{
		resultado = resultado + promedio[i];
	}
	resultado = resultado/10;
	return resultado;
}
