#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include <cmath>
#include <limits>

// numero de grupos que vamos a utilizar
inline int kGrupos = 3;

class centroide
{
private:
    float x;
    float y;

public:
    centroide(float x, float y) : x(x), y(y) {}
    float getX() { return x; }
    float getY() { return y; }
    void setX(float nuevaX) { x = nuevaX; }
    void setY(float nuevaY) { y = nuevaY; }
};

class punto
{
private:
    float x;
    float y;
    int idGrupo;

public:
    punto(float x, float y) : x(x), y(y), idGrupo(-1) {}
    float getX() { return x; }
    float getY() { return y; }
    int getIdGrupo() { return idGrupo; }
    void setIdGrupo(int puntoNuevo) { idGrupo = puntoNuevo; }
};

inline float getDistanciaCentroide(punto punto, centroide centroide)
{
    return (sqrt(pow(punto.getX() - centroide.getX(), 2) +
                 pow(punto.getY() - centroide.getY(), 2)));
}

inline bool asignarCentroides(std::vector<punto> &puntos, std::vector<centroide> &centroides)
{
    bool cambiado = false;
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

        if (puntos[i].getIdGrupo() != grupoGanador)
        {
            puntos[i].setIdGrupo(grupoGanador);
            cambiado = true;
        }
        return cambiado; // Se mantiene tu lógica original (retorno prematuro)
    }
    return cambiado;
}

inline void recalcularCentroide(std::vector<punto> &puntos, std::vector<centroide> &centroides)
{
    std::vector<float> sumaX(kGrupos, 0.0f);
    std::vector<float> sumaY(kGrupos, 0.0f);
    std::vector<int> contador(kGrupos, 0);
    for (int i = 0; i < puntos.size(); i++)
    {
        int id = puntos[i].getIdGrupo();
        if (id != -1)
        {
            sumaX[id] += puntos[i].getX();
            sumaY[id] += puntos[i].getY();
            contador[id]++;
        }
    }
    for (int i = 0; i < kGrupos; i++)
    {
        if (contador[i] > 0)
        {
            centroides[i].setX(sumaX[i] / contador[i]);
            centroides[i].setY(sumaY[i] / contador[i]);
        }
    }
}

#endif
