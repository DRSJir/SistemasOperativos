#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "rosa.h"

double calcular_promedio(int *promedio, int tam);

//calcula el promedio de las calificaciones de un conjunto de calificaciones.
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
		printf("Proceso azul: Error en shmget\n");
		return 1;
	}

	// Conectar la región de memoria al espacio de direcciones del proceso
    compartido = (struct memoria*) shmat(sm_id, NULL, 0);
    if (*((int *)compartido) == -1)
    {
    	printf("Proceso azul: Error en shmat\n");
    	return 1;
    }




    
    size_t tam = sizeof(int) * compartido -> num_notas;
    // Obtener un identificador de segmento de memoria compartida
	id_arreglo = shmget(clave2, tam, 0666);
	if (id_arreglo < 0)
	{
		printf("Proceso azul: Error en shmget del arreglo\n");
		return 1;
	}

	// Conectar la región de memoria al espacio de direcciones del proceso
    arreglo_compartido = (int*) shmat(id_arreglo, NULL, 0);
    if (*((int *)arreglo_compartido) == -1)
    {
    	printf("Proceso azul: Error en shmat del arreglo\n");
    	return 1;
    }


    int numnotas = compartido -> num_notas;

    // Usar memoria compartida
    compartido -> resultado[0] = calcular_promedio(arreglo_compartido, numnotas);
    compartido -> estado = AZUL;

    printf("Termina proceso azul librando memoria...\n");
	return 0;
}

double calcular_promedio(int *promedio, int tam)
{
	double resultado = 0;
	int i;

	for (int i = 0; i < tam; i++)
	{
		resultado = resultado + promedio[i];
	}
	resultado = resultado/tam;
	return resultado;
}
