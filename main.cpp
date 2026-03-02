#include "k-medias.hpp"
#include <iostream>
#include <time.h>
#include "mpi.h"

//este es main, donde se ejecuta el programa de k-medias
int main()
{
    //definimos k como constexpr
    constexpr int K=3;
    //numero iteraciones del programa que se van realizar para la correcta medición
    int nIter = 100000;
    //vectores de centroides y puntos, donde se alamacenaran los datos que vayamos calculando
    std::array<centroide,K> centroides;
    std::vector<punto> puntos = archivoApuntos("salida");

    //t cuando se empiezan a realizar las iteraciones
    clock_t t1 = clock();
    for (int iter = 0; iter < nIter; iter++)
    {
        //booleano para verificar cuando se estabilizan los centroides de los puntos 
        bool cambios;
        //inicialización los centroides de los puntos
        asignarCentroidesIniciales<K>(puntos,centroides);

        //do-while para calcular los centroides más optimos
        do
        {
            //Asignar cada punto al centroide más cercano
            cambios = asignarCentroides<K>(puntos, centroides);
            //Recalcular la posición de los centroides
            recalcularCentroide<K>(puntos, centroides);
        } while (cambios); // El bucle se detiene cuando ningún punto cambia de grupo
    }

    //t cuando se acaban de ejecutar las iteraciones
    clock_t t2 = clock();
    //tiempo tardado por iteración
    double elapsed = (double)(t2 - t1) / nIter;

    // Mostrar resultados finales
    printf("k-medias finalizado en %g segundos\n", elapsed / CLOCKS_PER_SEC);
    for (int i = 0; i < K; i++)
    {
        printf("Centroide del grupo %d: %g,%g\n", i, centroides[i].getX(), centroides[i].getY());
    }
    return 0;
}
