#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "rosa.h"

int calcular_aprobado(int *calificaciones, int num_notas);

// calculará la cantidad de alumnos que acreditaron el curso con calificaciones mayores o iguales a 7.
int main()
{
	struct memoria *compartido;
	int *arreglo_compartido;
	key_t clave;
	key_t clave2;
	int sm_id;
	int id_arreglo;

	// Crear una clave
	clave = ftok(".",'x');
	clave2 = ftok(".",'y');

	// Obtener un identificador de segmento de memoria compartida
	sm_id = shmget(clave, sizeof(compartido), 0666);
	if (sm_id < 0)
	{
		printf("Proceso rojo: Error en shmget\n");
		return 1;
	}

	// Conectar la región de memoria al espacio de direcciones del proceso
    compartido = (struct memoria*) shmat(sm_id, NULL, 0);
    if (*((int *)compartido) == -1)
    {
    	printf("Proceso rojo: Error en shmat\n");
    	return 1;
    }

    // Memoria compartida de las calificaciones
    size_t tam = sizeof(int) * compartido -> num_notas;
    // Obtener un identificador de segmento de memoria compartida
	id_arreglo = shmget(clave2, tam, 0666);
	if (id_arreglo < 0)
	{
		printf("Proceso rojo: Error en shmget del arreglo\n");
		return 1;
	}

	// Conectar la región de memoria al espacio de direcciones del proceso
    arreglo_compartido = (int*) shmat(id_arreglo, NULL, 0);
    if (*((int *)arreglo_compartido) == -1)
    {
    	printf("Proceso rojo: Error en shmat del arreglo\n");
    	return 1;
    }

    int numnotas = compartido -> num_notas;

    // Usar memoria compartida
    compartido -> resultado[1] = calcular_aprobado(arreglo_compartido, numnotas);
    compartido -> estado = ROJO;

    printf("Termina proceso rojo librando memoria...\n");
	return 0;
}

int calcular_aprobado(int *calificaciones, int num_notas)
{
	int aprobados = 0;
	for (int i = 0; i < num_notas; i++)
	{
		if (calificaciones[i] >= 7)
		{
			aprobados++;
		}
	}
	return aprobados;
}