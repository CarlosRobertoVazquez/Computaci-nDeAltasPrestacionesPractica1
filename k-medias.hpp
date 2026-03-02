#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include <cmath>
#include <limits>
#include <string>
#include <iostream>
#include <fstream>
#include <array>

// numero de grupos que vamos a utilizar

// clase del centroide
class centroide
{
private:
    float x;
    float y;

public:
    centroide() : x(0.0f), y(0.0f) {}
    centroide(float x, float y) : x(x), y(y) {}
    float getX() const { return x; }
    float getY() const { return y; }
    void setX(float nuevaX) { x = nuevaX; }
    void setY(float nuevaY) { y = nuevaY; }
};

// clase del punto
class punto
{
private:
    float x;
    float y;
    int idGrupo;

public:
    punto(float x, float y) : x(x), y(y), idGrupo(-1) {}
    float getX() const { return x; }
    float getY() const { return y; }
    int getIdGrupo() const { return idGrupo; }
    void setIdGrupo(int puntoNuevo) { idGrupo = puntoNuevo; }
};

inline float getDistanciaCentroide(const punto &punto, const centroide &centroide)
{
    float distanciaX = punto.getX() - centroide.getX();
    float distanciaY = punto.getY() - centroide.getY();
    // retorna la distancia entre los dos puntos con pitagoras
    return distanciaX * distanciaX + distanciaY * distanciaY;
}
template <int K>
inline bool asignarCentroides(std::vector<punto> &puntos, const std::array<centroide, K> &centroides)
{
    // booleano para el retorno de la función que indica si han cambiado los puntos
    bool cambiado = false;
    // recorremos todos los puntos
    for (auto &p : puntos)
    {
        // variables para inidicar que centroide está más cerca del punto
        float mejorDistancia = std::numeric_limits<float>::max();
        int grupoGanador = -1;

        // recorremos los centroides para compararlos con el punto con la función get distancia
        for (int j = 0; j < K; ++j)
        {
            float distanciaCentroide = getDistanciaCentroide(p, centroides[j]);
            // si la distacia al centroide j es más cercana, guardamos la distacia y su id
            if (mejorDistancia > distanciaCentroide)
            {
                mejorDistancia = distanciaCentroide;
                grupoGanador = j;
            }
        }
        // sila mejor distancia ha cambiado, cambiamos los datos del punto e indicamos que se ha cambiado
        if (p.getIdGrupo() != grupoGanador)
        {
            p.setIdGrupo(grupoGanador);
            cambiado = true;
        }
    }
    // si algún punto ha cambiado retorna true por lo que el programa sigue y si no false
    return cambiado;
}

template <int K>
inline void recalcularCentroide(const std::vector<punto> &puntos, std::array<centroide, K> &centroides)
{
    // vectores donde se guardarán las sumas de los puntos por id para la media
    std::array<float, K> sumaX{};
    std::array<float, K> sumaY{};
    // vector donde se guarda la cantidad de puntos por id para la media
    std::array<int, K> contador{};

    // recorremos todos los puntos disponibles
    for (const auto &p : puntos)
    {
        // identificamos a que grupo pertenece cada punto
        int id = p.getIdGrupo();
        // cada punto incrementa en su grupo
        if (id != -1)
        {
            sumaX[id] += p.getX();
            sumaY[id] += p.getY();
            contador[id]++;
        }
    }
    // recorremos los grupos
    for (int i = 0; i < K; i++)
    {
        // hacemos la media con su respectiva parte del vector
        if (contador[i] > 0)
        {
            centroides[i].setX(sumaX[i] / contador[i]);
            centroides[i].setY(sumaY[i] / contador[i]);
        }
    }
}

inline std::vector<punto> archivoApuntos(std::string nombreArchivo)
{
    std::ifstream archivo(nombreArchivo, std::ios::binary);

    if (!archivo.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo");
    }

    std::vector<punto> puntos;
    float bufferX, bufferY;
    while (archivo.read(reinterpret_cast<char *>(&bufferX), sizeof(float)) &&
           archivo.read(reinterpret_cast<char *>(&bufferY), sizeof(float)))
    {
        puntos.emplace_back(bufferX, bufferY);
    }
    return puntos;
}

template <int K>
// reparto equitativo de los puntos
inline void asignarCentroidesIniciales(std::vector<punto> &puntos, std::array<centroide, K> &centroides)
{
    // recorremos los grupos creando variables para calcular el centroide
    std::array<float, K> sumaX{};
    std::array<float, K> sumaY{};
    std::array<int, K> contador{};

    // recorremos los puntos y marcamos el id en el grupo
    for (int i = 0; i < puntos.size(); i++)
    {
        // El módulo asegura que el ID siempre esté entre 0 y kGrupos-1
        int grupo = i % K;

        puntos[i].setIdGrupo(grupo);
        sumaX[grupo] += puntos[i].getX();
        sumaY[grupo] += puntos[i].getY();
        contador[grupo]++;
    }
    // guardamos en que posición está el centroide del grupo
    for (int i = 0; i < K; i++)
    {
        centroides[i] = centroide(sumaX[i] / contador[i], sumaY[i] / contador[i]);
    }
}
#endif
