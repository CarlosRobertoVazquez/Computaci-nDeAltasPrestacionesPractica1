#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include <cmath>
#include <limits>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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
    //funcion simple que calcula la distancia entre los dos puntos con pitagoras
    return (sqrt(pow(punto.getX() - centroide.getX(), 2) +
                 pow(punto.getY() - centroide.getY(), 2)));
}

inline bool asignarCentroides(std::vector<punto> &puntos, std::vector<centroide> &centroides)
{
    //booleano para el retorno de la función que indica si han cambiado los puntos 
    bool cambiado = false;
    //recorremos todos los puntos
    for (int i = 0; i < puntos.size(); i++)
    {
        //variables para inidicar que centroide está más cerca del punto
        float mejorDistancia = std::numeric_limits<float>::max();
        int grupoGanador = -1;

        //recorremos los centroides para compararlos con el punto con la función get distancia
        for (int j = 0; j < centroides.size(); j++)
        {
            float distanciaCentroide = getDistanciaCentroide(puntos[i], centroides[j]);
            //si la distacia al centroide j es más cercana, guardamos la distacia y su id
            if (mejorDistancia > distanciaCentroide)
            {
                mejorDistancia = distanciaCentroide;
                grupoGanador = j;
            }
        }
        //sila mejor distancia ha cambiado, cambiamos los datos del punto e indicamos que se ha cambiado
        if (puntos[i].getIdGrupo() != grupoGanador)
        {
            puntos[i].setIdGrupo(grupoGanador);
            cambiado = true;
        }
    }
    // si algún punto ha cambiado retorna true por lo que el programa sigue y si no false
    return cambiado;
}

inline void recalcularCentroide(std::vector<punto> &puntos, std::vector<centroide> &centroides)
{
    //vectores donde se guardarán las sumas de los puntos por id para la media
    std::vector<float> sumaX(kGrupos, 0.0f);
    std::vector<float> sumaY(kGrupos, 0.0f);
    //vector donde se guarda la cantidad de puntos por id para la media
    std::vector<int> contador(kGrupos, 0);

    //recorremos todos los puntos disponibles
    for (int i = 0; i < puntos.size(); i++)
    {
        //identificamos a que grupo pertenece cada punto
        int id = puntos[i].getIdGrupo();
        //cada punto incrementa en su grupo
        if (id != -1)
        {
            sumaX[id] += puntos[i].getX();
            sumaY[id] += puntos[i].getY();
            contador[id]++;
        }
    }
    //recorremos los grupos
    for (int i = 0; i < kGrupos; i++)
    {
        //hacemos la media con su respectiva parte del vector
        if (contador[i] > 0)
        {
            centroides[i].setX(sumaX[i] / contador[i]);
            centroides[i].setY(sumaY[i] / contador[i]);
        }
    }
}

inline std::vector<punto> archivoApuntos(std::string nombreArchivo){
    std::ifstream archivo(nombreArchivo, std::ios::binary);
    std::vector<punto> puntos;

    if (!archivo.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo"); 
    }
    punto p(0,0);
    std::string linea;
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(punto)))
    {
        puntos.push_back(p);
    }
    archivo.close();
    if (puntos.empty())
    {
        throw std::runtime_error("El archivo está vacio"); 
    }
    return puntos;
}
//reparto equitativo de los puntos
inline void asignarCentroidesIniciales(std::vector<punto>& puntos, std::vector<centroide>& centroides){
    //dividimos el tamaño del vector de puntos entre el numero de grupos
    int indice=puntos.size()/kGrupos;
    
    //recorremos los grupos creando variables para calcular el centroide
    for(int i=0; i<kGrupos; i++){
        double sumaX,sumaY;
        int contador;

        //recorremos la cantidad de puntos que corresponden por grupo, marcamos el id en el grupo 
        //y sumamos sus posiciones para el calculo del centroide
        for(int j=indice*i;j<indice*(i+1);j++){
            puntos[j].setIdGrupo(i);
            sumaX += puntos[j].getX();
            sumaY += puntos[j].getY();
            contador++;
        }
        //guardamos en que posición está el centroide del grupo
        centroides[i].setX(sumaX/contador);
        centroides[i].setY(sumaY/contador);
    }
}
#endif
