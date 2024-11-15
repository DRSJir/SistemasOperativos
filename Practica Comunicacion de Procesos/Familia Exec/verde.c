#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int calcular_aprobado(int *calificaciones);

//argv = ["0"|"10"|"4"|"5"|"8"|...|NULL]
int main(int argc, char const *argv[])
{
	int calificaciones[argc];
	int aprobados;

	for (int i = 0; i < argc-1; i++)
	{
		calificaciones[i] = atoi(argv[i]);
	}

	aprobados = calcular_aprobado(calificaciones);
	exit(aprobados);
	return 0;
}

int calcular_aprobado(int *calificaciones) 
{
	int aprobados = 0;
	for (int i = 0; i < 10; i++)
	{
		if (calificaciones[i] < 6)
		{
			aprobados++;
		}
	}
	return aprobados;
}