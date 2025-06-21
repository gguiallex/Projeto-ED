#ifndef MANIPULADORBINARIO_H
#define MANIPULADORBINARIO_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "registro.h"

using namespace std;

const int MAX_RUNS = 300;
const int MAX_HEAP = MAX_RUNS;
const int MAX_BUFFER = 1000;

class manipuladorBinario {
private:
    string nomeArquivo;

public:
    manipuladorBinario(string nome);
    void alterarRegistroNaPosicao(int posicao, const Registro &novo);
    void trocarRegistros(int pos1, int pos2);
    void inserir(int posicao);
    void visualizarEntre();
    void imprimirTodos();
    void converterCsvParaBinario(char *nomeCsv, const char *nomeBinario); 
    void ordenarMergeMultiway(int bufferSize = MAX_BUFFER);
};

#endif

