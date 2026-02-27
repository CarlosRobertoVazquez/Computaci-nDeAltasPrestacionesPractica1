#include "k-medias.hpp"
#include <iostream>
#include <fstream>

int main()
{
    // vertemos los datos de los puntos en un vector de punto para poder accder a ellos
    std::ifstream archivopuntos("puntos.txt");

    if (!archivopuntos.is_open())
    {
        std::cout << "error al abrir el archivo";
        return 1;
    }

    std::vector<punto> puntos;

    float x, y;
    while (archivopuntos >> x >> y)
    {
        puntos.emplace_back(x, y);
    }
    archivopuntos.close();
    if (puntos.empty()) {
        std::cout << "El archivo esta vacio." << std::endl;
        return 1;
    }

    // elegimos tres puntos aleatorios para que sean los centroides
    // estos puntos aleatorios se van a elegir partiendo los vectores en k y el que caiga

    int t = puntos.size() / kGrupos;
    std::vector<centroide> centroides;
    for (int i = 0; i < kGrupos; i++)
    {
        centroides.emplace_back(puntos[i * t].getX(), puntos[i * t].getY());
    }

    bool cambios;
    int iteraciones=0;
    do {
        // 1. Asignar cada punto al centroide más cercano
        cambios = asignarCentroides(puntos, centroides);
        
        // 2. Recalcular la posición de los centroides basándose en sus nuevos puntos
        recalcularCentroide(puntos, centroides);
        
        iteraciones++;
    } while (cambios); // El bucle se detiene cuando ningún punto cambia de grupo

    // Mostrar resultados finales
    std::cout << "\nK-Medias finalizado en " << iteraciones << " iteraciones." << std::endl;
    for (int i = 0; i < kGrupos; i++) {
        std::cout << "Centroide " << i << ": (" << centroides[i].getX() << ", " << centroides[i].getY() << ")" << std::endl;
    }
    return 0;
}
