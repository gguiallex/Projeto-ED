#ifndef REGISTRO_H
#define REGISTRO_H

#include <iostream>
#include <cstring>
using namespace std;

class Registro {
    friend class manipuladorBinario;
    friend class MinHeap;

private:
    int id;
    char nome[50];
    char cidade[50];
    char esporte[50];
    char evento[100];
    char nacionalidade[10];

public:
    Registro(int i = 0, const char *n = "", const char *c = "", const char *e = "", const char *ev = "", const char *na = "");
    void escreverBinario(ostream &out) const;
    bool lerBinario(istream &in);
    void imprimirLinha() const;
    void lerTeclado();
    static int tamanho();
    int obterId() const { return id; }
};

#endif
