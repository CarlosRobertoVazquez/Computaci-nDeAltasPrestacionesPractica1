#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

class punto;
class centroide;
// nuemero de grupos que vamos a utilizar
int kGrupos = 3;
class centroide
{
private:
    float x;
    float y;

public:
    centroide(float x, float y) : x(x), y(y) {}
    float getX()
    {
        return x;
    }
    float getY()
    {
        return y;
    }
    void setX(float nuevaX)
    {
        x = nuevaX;
    }
    void setY(float nuevaY)
    {
        y = nuevaY;
    }
};
class punto
{
private:
    float x;
    float y;
    int idGrupo;

public:
    punto(float x, float y) : x(x), y(y), idGrupo(-1) {}
    float getX()
    {
        return x;
    }
    float getY()
    {
        return y;
    }
    int getIdGrupo()
    {
        return idGrupo;
    };
    void setIdGrupo(int puntoNuevo)
    {
        idGrupo = puntoNuevo;
    }
};

float getDistanciaCentroide(punto punto, centroide centroide)
{
    return (sqrt(
        pow(punto.getX() - centroide.getX(), 2) +
        pow(punto.getY() - centroide.getY(), 2)));
}

bool asignarCentroides(std::vector<punto>& puntos, std::vector<centroide>& centroides)
{
    bool cambiado=false;
    for (int i = 0; i < puntos.size(); i++)
    {
        float mejorDistancia = std::numeric_limits<float>::max();
        int grupoGanador = -1;

        for (int j = 0; j < centroides.size(); j++)
        {
            float distanciaCentroide = getDistanciaCentroide(puntos[i], centroides[j]);
            if (mejorDistancia > distanciaCentroide)
            {
                mejorDistancia = distanciaCentroide;
                grupoGanador = j;
            }
        }

        if (puntos[i].getIdGrupo() != grupoGanador) {
            puntos[i].setIdGrupo(grupoGanador);
            cambiado = true;
        }
        return cambiado;
    }
}

void recalcularCentroide(std::vector<punto>& puntos, std::vector<centroide>& centroides)
{
    std::vector<float> sumaX(kGrupos, 0.0f);
    std::vector<float> sumaY(kGrupos, 0.0f);
    std::vector<int> contador(kGrupos, 0);
    for (int i=0; i < puntos.size(); i++)
    {
        int id = puntos[i].getIdGrupo();

        if (id != -1) {
            sumaX[id] += puntos[i].getX();
            sumaY[id] += puntos[i].getY();
            contador[id]++;
        }
    }

    for (int i = 0; i < kGrupos; i++)
    {
        if (contador[i] > 0) {
            centroides[i].setX(sumaX[i] / contador[i]);
            centroides[i].setY(sumaY[i] / contador[i]);
        }
    }
}

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
    std::cout << "\nK-Means finalizado en " << iteraciones << " iteraciones." << std::endl;
    for (int i = 0; i < kGrupos; i++) {
        std::cout << "Centroide " << i << ": (" << centroides[i].getX() << ", " << centroides[i].getY() << ")" << std::endl;
    }
    return 0;
}
