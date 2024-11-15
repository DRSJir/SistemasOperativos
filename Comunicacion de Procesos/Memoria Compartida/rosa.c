#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "rosa.h"

int contarLineas(FILE *nombreArchivo);

// Proceso rosa
int main(int argc, char const *argv[])
{
	// Leer el archivo y almacenar las calificaciones en un arreglo
	int *calificaciones = NULL;
    int i = 0;
    int num_calificaciones = 0;
    FILE *archivo;

    // Memoria compartida
    struct memoria *compartido;
    key_t clave;
    key_t clave2;
    int sm_id;
    int id_arreglo;
    int *arreglo;

    // Abrir el archivocalificaciones
    archivo = fopen(argv[1], "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo");
        return 1;
    }

    num_calificaciones = contarLineas(archivo);
    rewind(archivo);
    
    // Leer las calificaciones
    calificaciones = (int*)malloc(num_calificaciones * sizeof(int));
    
    for (int i = 0; i < num_calificaciones; i++)
    {
        fscanf(archivo, "%d", &calificaciones[i]);
    }

    fclose(archivo);

    // Memoria compartida
    // Crear una clave
		// para que los diferentes procesos puedan acceder a la misma región.
    clave = ftok(".",'x');

	// Obtener un identificador de segmento de memoria compartida
    sm_id = shmget(clave, sizeof(compartido), IPC_CREAT | 0666);
	if (sm_id < 0) // Captura de errores
	{
		printf("Proceso 1: Error en shmget\n");
		return 1;
	}

    // Conectar la región de memoria al espacio de direcciones del proceso
    compartido = (struct memoria*) shmat(sm_id, NULL, 0);
    if (*((int *)compartido) == -1) // Captura de errores
    {
    	printf("Proceso 1: Error en shmat\n");
    	return 1;
    }


    clave2 = ftok(".",'y');

    // Obtener un identificador de segmento de memoria compartida
    id_arreglo = shmget(clave2, sizeof(int) * num_calificaciones, IPC_CREAT | 0666);
    if (id_arreglo < 0) // Captura de errores
    {
        printf("Proceso 1: Error en shmget del arreglo\n");
        return 1;
    }

    // Conectar la región de memoria al espacio de direcciones del proceso
    arreglo = (int*) shmat(id_arreglo, NULL, 0);
    if (arreglo == (int*)-1) // Captura de errores
    {
        printf("Proceso 1: Error en shmat\n");
        return 1;
    }


    for (int i = 0; i < num_calificaciones; i++)
    {
        arreglo[i] = calificaciones[i];
        printf("%d ",arreglo[i]);
    }


    // Usar memoria compartida
    printf("Proceso 1: Usando la memoria compartida...\n");
    compartido -> num_notas = num_calificaciones;

    // "Esperar a los procesos que terminen"
    compartido -> estado = NO_LISTO;
    while(compartido -> estado != 1)
    {
        printf("Proceso Rosa: Esperando a el proceso azul que termine...\n");
        sleep(2);
    }
    while(compartido -> estado != 2)
    {
        printf("Proceso Rosa: Esperando a el proceso rojo que termine...\n");
        sleep(2);
    }
    while(compartido -> estado != 3)
    {
        printf("Proceso Rosa: Esperando a el proceso verde que termine...\n");
        sleep(2);
    }

    printf("El promedio de las calificaciones es: %d\n",compartido -> resultado[0]);
    printf("Alumnos que acreditaron el curso con calificaciones mayores o iguales a 7: %d\n",compartido -> resultado[1]);
    printf("Cantidad de alumnos que reprobaron el curso: %d\n",compartido -> resultado[2]);

    return 0;
}

int contarLineas(FILE *nombreArchivo) {
    int lineas = 0;
    char caracter;

    // Lee cada carácter hasta el final del archivo
    while ((caracter = fgetc(nombreArchivo)) != EOF) {
        if (caracter == '\n') {
            lineas++;
        }
    }
    return lineas + 1;
}