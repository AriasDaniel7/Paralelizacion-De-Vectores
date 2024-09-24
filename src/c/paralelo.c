#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <omp.h>

void generaVector(unsigned long int *v, unsigned long int N)
{
    unsigned long int i;
    srand(time(NULL));

#pragma omp parallel for
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
#pragma omp parallel for reduction(+ : suma)
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

#pragma omp parallel for reduction(+ : suma)
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

#pragma omp parallel for reduction(+ : suma)
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
    unsigned long int pos = 0;

#pragma omp parallel for private(i) shared(mayor, pos)
    for (i = 0; i < N; i++)
    {
        if (v[i] > mayor)
        {
            mayor = v[i];
            pos = i;
        }
    }

    *posicion = pos;
    return (mayor);
}

unsigned long long int modaVector(unsigned long int *v, unsigned long int N)
{
    unsigned long int i, j, moda = 0, frecuencia = 0, frecuenciaMax = 0;

#pragma omp parallel for private(i, j, frecuencia) shared(moda, frecuenciaMax)
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
#pragma omp critical
        if (frecuencia > frecuenciaMax)
        {
            frecuenciaMax = frecuencia;
            moda = v[i];
        }
    }
    return moda;
}

void guardarResultadosCSV(int NH, unsigned long int N, unsigned long long int suma, unsigned long long int mayor, unsigned long long int moda, double media, double desviacion, double tiempo)
{
    FILE *archivo;

    archivo = fopen("src/datos/paralelo.csv", "r"); // Intenta abrir el archivo en modo lectura
    if (archivo == NULL)                            // Si no existe el archivo, lo crea
    {
        archivo = fopen("src/datos/paralelo.csv", "w"); // Intenta abrir el archivo en modo escritura
        if (archivo != NULL)                            // Si se pudo abrir el archivo, escribe los encabezados
        {
            fprintf(archivo, "NH,N,SUMA,MAYOR,MODA,MEDIA,DESVIACION,TIEMPO\n"); // Escribe los encabezados
        }
    }
    else
    {
        // Si el archivo ya existe, lo cierra y lo abre en modo adición
        fclose(archivo);
        archivo = fopen("src/datos/paralelo.csv", "a");
    }

    if (archivo != NULL)
    {
        // Escribe los resultados en el archivo
        fprintf(archivo, "%d,%lu,%llu,%llu,%llu,%.2f,%.2f,%.2f\n", NH, N, suma, mayor, moda, media, desviacion, tiempo);
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
    int NH = 12;

    if (argc == 3)
    {
        NH = atoi(argv[1]);
        N = atoi(argv[2]);
    }

    omp_set_num_threads(NH);

    v = (unsigned long int *)malloc(N * sizeof(unsigned long int));

    start_time = omp_get_wtime();

    generaVector(v, N);
    printf(" Contenido del vector: \n");
    imprimirVector(v, 10);
    suma = sumarVector(v, N);
    media = promedioVector(v, N);
    desviacion = desviacionEstandarVector(v, N, media);
    mayor = encontrarMayorYPosicion(v, N, &i);
    moda = modaVector(v, N);

    end_time = omp_get_wtime();

    printf("El mayor elemento es %llu y se encuentra en la posicion %lu \n", mayor, i);
    printf("Tiempo de ejecucion = %f \n", end_time - start_time);
    printf("Suma de elementos = %llu \n", suma);
    printf("Promedio  = %.2f \n", media);
    printf("Moda = %llu \n", moda);
    printf("Desvacion estandar = %.2f \n", desviacion);
    printf("Numero de hilos = %d \n", NH);
    printf("Numero de elementos = %lu \n", N);

    guardarResultadosCSV(NH, N, suma, mayor, moda, media, desviacion, (end_time - start_time));

    free(v);

    return 0;
}
