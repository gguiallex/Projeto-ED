#include "minheap.h"


MinHeap::MinHeap() : tamanho(0) {}

void MinHeap::corrigeSubindo(int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (dados[i].reg.id < dados[pai].reg.id) {
            std::swap(dados[i], dados[pai]);
            i = pai;
        } else {
            break;
        }
    }
}

void MinHeap::corrigeDescendo(int i) {
    while (2 * i + 1 < tamanho) {
        int filhoEsq = 2 * i + 1;
        int filhoDir = 2 * i + 2;
        int menor = i;

        if (filhoEsq < tamanho && dados[filhoEsq].reg.id < dados[menor].reg.id)
            menor = filhoEsq;
        if (filhoDir < tamanho && dados[filhoDir].reg.id < dados[menor].reg.id)
            menor = filhoDir;

        if (menor != i) {
            std::swap(dados[i], dados[menor]);
            i = menor;
        } else {
            break;
        }
    }
}

void MinHeap::inserir(const HeapItem& item) {
    if (tamanho < MAX_HEAP) {
        dados[tamanho] = item;
        corrigeSubindo(tamanho);
        tamanho++;
    }
}

HeapItem MinHeap::extrairMinimo() {
    HeapItem raiz = dados[0];
    dados[0] = dados[--tamanho];
    corrigeDescendo(0);
    return raiz;
}

bool MinHeap::vazio() const {
    return tamanho == 0;
}
