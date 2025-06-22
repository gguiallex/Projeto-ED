#ifndef MINHEAP_H
#define MINHEAP_H

#include "registro.h"
#include "manipuladorBinario.h" 

struct HeapItem {
    Registro reg;
    int origem;
};

// Implementa uma heap mínima manualmente (sem STL).

class MinHeap {
private:
    HeapItem dados[MAX_HEAP];
    int tamanho;

    inline int pai(int i);
    inline int esquerdo(int i);
    inline int direito(int i);
    void corrigeSubindo(int i);
    void corrigeDescendo(int i);

public:
    MinHeap();
    void inserir(const HeapItem& item);
    HeapItem extrairMinimo();
    bool vazio() const;
};

#endif