#ifndef MINHEAP_H
#define MINHEAP_H

#include "manipuladorBinario.h"
#include "registro.h"


struct HeapItem {
    Registro reg;
    int origem;
};

class MinHeap {
private:
    HeapItem dados[MAX_HEAP];
    int tamanho;

    void corrigeSubindo(int i);
    void corrigeDescendo(int i);

public:
    MinHeap();
    void inserir(const HeapItem& item);
    HeapItem extrairMinimo();
    bool vazio() const;
};

#endif
