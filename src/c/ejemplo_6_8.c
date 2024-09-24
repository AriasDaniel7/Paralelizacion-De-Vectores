#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

void generaVector(unsigned long int *v, unsigned long int N)
{
	unsigned long int i;
	srand(time(NULL));

	for (i = 0; i < N; i++)
	{
		v[i] = rand();
	}
}

void imprimirVector(unsigned long int *v, int N)
{
	int i;
	for (i = 0; i < N; i++)
	{
		printf("  %d  ", v[i]);
	}
	printf("\n");
}

unsigned long long int sumarVector(unsigned long int *v, unsigned long int N)
{
	long long int i, suma = 0;

	for (i = 0; i < N; i++)
	{
		suma = suma + v[i];
	}
	return (suma);
}

double promedioVector(unsigned long int *v, unsigned long int N)
{
	long long int i, suma = 0;
	double promedio;
	for (i = 0; i < N; i++)
	{
		suma = suma + v[i];
	}
	promedio = suma / N;
	return (promedio);
}

double desviacionEstandarVector(unsigned long int *v, unsigned long int N, double media)
{
	long long int i, suma = 0;
	double desviacion;
	for (i = 0; i < N; i++)
	{
		suma = suma + pow((v[i] - media), 2);
	}
	desviacion = sqrt(suma) / N;
	return (desviacion);
}

unsigned long long int encontrarMayorYPosicion(unsigned long int *v, unsigned long int N, unsigned long int *posicion)
{
	unsigned long int i;
	unsigned long int mayor = 0;
	*posicion = 0;
	for (i = 0; i < N; i++)
	{
		if (v[i] > mayor)
		{
			mayor = v[i];
			*posicion = i;
		}
	}
	return (mayor);
}

unsigned long long int modaVector(unsigned long int *v, unsigned long int N)
{
	unsigned long int i, j, moda = 0, frecuencia = 0, frecuenciaMax = 0;
	for (i = 0; i < N; i++)
	{
		frecuencia = 0;
		for (j = 0; j < N; j++)
		{
			if (v[i] == v[j])
			{
				frecuencia++;
			}
		}
		if (frecuencia > frecuenciaMax)
		{
			frecuenciaMax = frecuencia;
			moda = v[i];
		}
	}
	return moda;
}

void guardarResultadosCSV(unsigned long int N, unsigned long long int suma, double media, double desviacion, unsigned long long int mayor, unsigned long long int moda, double tiempo)
{
	FILE *archivo;

	archivo = fopen("src/datos/secuencial.csv", "r"); // Intenta abrir el archivo en modo lectura
	if (archivo == NULL)							  // Si no existe el archivo, lo crea
	{
		archivo = fopen("src/datos/secuencial.csv", "w"); // Intenta abrir el archivo en modo escritura
		if (archivo != NULL)							  // Si se pudo abrir el archivo, escribe los encabezados
		{
			fprintf(archivo, "N,SUMA,MEDIA,DESVIACION,MAYOR,MODA,TIEMPO\n");
		}
	}
	else
	{
		// Si el archivo ya existe, lo cierra y lo abre en modo adición
		fclose(archivo);
		archivo = fopen("src/datos/secuencial.csv", "a");
	}

	if (archivo != NULL)
	{
		// Escribe los resultados en el archivo
		fprintf(archivo, "%lu, %llu, %.2f, %.2f, %llu, %llu, %.2f\n", N, suma, media, desviacion, mayor, moda, tiempo);
		fclose(archivo);
	}
	else
	{
		printf("Error al abrir el archivo.\n");
	}
}

int main(int argc, char *argv[])
{

	unsigned long int *v, i, N;
	unsigned long long int suma, mayor, moda = 0;
	double desviacion, media;
	double start_time, end_time;
	N = 10000;

	if (argc == 2)
	{
		N = atoi(argv[1]);
	}

	v = (unsigned long int *)malloc(N * sizeof(unsigned long int));

	start_time = clock();

	generaVector(v, N);

	printf(" Contenido del vector: \n");
	imprimirVector(v, 10);

	suma = sumarVector(v, N);
	media = promedioVector(v, N);
	desviacion = desviacionEstandarVector(v, N, media);
	mayor = encontrarMayorYPosicion(v, N, &i);
	moda = modaVector(v, N);

	end_time = clock();

	printf("El mayor elemento es %llu y se encuentra en la posicion %lu \n", mayor, i);
	printf("Tiempo de ejecucion = %f segundos \n", (end_time - start_time) / CLOCKS_PER_SEC);
	printf("Suma de elementos = %llu \n", suma);
	printf("Moda = %llu \n", moda);
	printf("Promedio  = %.2f \n", media);
	printf("Desvacion estandar = %.2f \n", desviacion);
	printf("Numero de elementos = %lu \n", N);

	guardarResultadosCSV(N, suma, media, desviacion, mayor, moda, ((end_time - start_time) / CLOCKS_PER_SEC));

	free(v);
	return 0;
}
