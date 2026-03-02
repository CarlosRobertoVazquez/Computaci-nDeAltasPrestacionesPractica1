#include "k-medias.hpp"
#include <iostream>
#include <fstream>
#include <time.h>

//este es main, donde se ejecuta el programa de k-medias
int main()
{
    //indicamos el numero iteraciones del programa que se van realizar para la correcta mediación
    int nIter = 1000;
    //creamos el vector de centroides, donde se alamacenaran los centros que vayamos calculando
    std::vector<centroide> centroides;
    //vertemos los datos de los puntos en un vector de punto para poder accder a ellos
    std::vector<punto> puntos = archivoApuntos("salida");

    //guardamos el tiempo cuando se empiezan a realizar las iteraciones y las iniciamos 
    clock_t t1 = clock();
    for (int iter = 0; iter < nIter; iter++)
    {
        //limpiamos el vector de centroides para cada iteración
        centroides.clear();
        //creamos el booleano para verificar cuando se estabilizan los centroides de los puntos 
        bool cambios;
        //iniciamos los centroides de los puntos
        asignarCentroidesIniciales(puntos,centroides);

        //iniciamos un do-while para calcular los centroides más optimos
        do
        {
            //Recalcular la posición de los centroides
            recalcularCentroide(puntos, centroides);
            //Asignar cada punto al centroide más cercano
            cambios = asignarCentroides(puntos, centroides);

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
