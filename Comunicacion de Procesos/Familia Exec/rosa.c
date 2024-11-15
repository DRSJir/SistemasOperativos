#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

void esperar_proceso(int pid);

int main(int argc, char const *argv[])
{
	char *calificaciones[] = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", NULL};
    char calificacion[3];
    int i = 0;
    int pid_azul = 0, pid_verde = 0, pid_rojo = 0;
    FILE *archivo;

    // Abrir el archivo
    archivo = fopen("calificaciones.txt", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Leer las calificaciones
    for (int i = 0; i < 10; ++i)
    {
        fgets(calificacion, 3, archivo);
        calificaciones[i] = calificacion;
    }
    fclose(archivo);


    // Crear procesos
    pid_azul = fork();
    if (pid_azul == 0)
    {
    	execv("./azul", calificaciones);
    }

    pid_verde = fork();
    if (pid_verde == 0)
    {
        execv("./verde", calificaciones);
    }

    pid_rojo = fork();
    if (pid_rojo == 0)
    {
        execv("./rojo", calificaciones);
    }

    //Esperar a los hijos que devuelvan un valor
    printf("El promedio de las calificaciones es: ");
    esperar_proceso(pid_azul);
    printf("Alumnos que acreditaron el curso con calificaciones mayores o iguales a 7: ");
    esperar_proceso(pid_rojo);
    printf("Cantidad de alumnos que reprobaron el curso: ");
    esperar_proceso(pid_verde);

	return 0;
}

void esperar_proceso(int pid) 
{
    int status;
    int resultado;

    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
    {
        resultado = WEXITSTATUS(status);
        printf("%d\n",resultado);
    }
}
