#include "k-medias.hpp"
#include <iostream>
#include <fstream>
#include <time.h>

int main()
{
    int nIter = 1000;
    std::vector<centroide> centroides;
    // vertemos los datos de los puntos en un vector de punto para poder accder a ellos
    std::vector<punto> puntos = archivoApuntos("salida");

    clock_t t1 = clock();
    for (int iter = 0; iter < nIter; iter++)
    {
        centroides.clear();
        bool cambios;
        // elegimos tres puntos aleatorios para que sean los centroides
        // estos puntos aleatorios se van a elegir partiendo los vectores en k y el que caiga

        int t = puntos.size() / kGrupos;
        for (int i = 0; i < kGrupos; i++)
        {
            centroides.emplace_back(puntos[i * t].getX(), puntos[i * t].getY());
        }

        do
        {
            // 1. Asignar cada punto al centroide más cercano
            cambios = asignarCentroides(puntos, centroides);

            // 2. Recalcular la posición de los centroides basándose en sus nuevos puntos
            recalcularCentroide(puntos, centroides);

        } while (cambios); // El bucle se detiene cuando ningún punto cambia de grupo
    }
    clock_t t2 = clock();
    double elapsed = (double)(t2 - t1) / nIter;

    // Mostrar resultados finales
    printf("k-medias finalizado en %g segundos\n", elapsed / CLOCKS_PER_SEC);
    for (int i = 0; i < kGrupos; i++)
    {
        printf("Centroide del grupo %d: %g,%g\n", i, centroides[i].getX(), centroides[i].getY());
    }
    return 0;
}
