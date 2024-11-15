#define NO_LISTO 0
#define AZUL 1
#define ROJO 2
#define VERDE 3

struct memoria
{
	int estado;
	int num_notas;
	// resultados[0 Azul|1 Rojo|2 Verde]
	int resultado[3];
};