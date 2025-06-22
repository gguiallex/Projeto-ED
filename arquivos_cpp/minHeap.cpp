#include "minheap.h"


using namespace std;

MinHeap::MinHeap() {
    tamanho = 0;
}

// Retornam os índices dos nós relacionados na heap.

int MinHeap::pai(int i) {
    return (i - 1) / 2;
}

int MinHeap::esquerdo(int i) {
    return 2 * i + 1;
}

int MinHeap::direito(int i) {
    return 2 * i + 2;
}

// Reorganiza a heap após uma inserção.

void MinHeap::corrigeSubindo(int i) {
    if (i > 0) {
        int p = pai(i);
        if (strcmp(dados[i].reg.esporte, dados[p].reg.esporte) < 0) {
            swap(dados[i], dados[p]);
            corrigeSubindo(p);
        }
    }
}

//Reorganiza a heap após uma remoção.

void MinHeap::corrigeDescendo(int i) {
    int esq = esquerdo(i);
    int dir = direito(i);
    int menor = i;

    if (esq < tamanho && strcmp(dados[esq].reg.esporte, dados[menor].reg.esporte) < 0) {
        menor = esq;
    }

    if (dir < tamanho && strcmp(dados[dir].reg.esporte, dados[menor].reg.esporte) < 0) {
        menor = dir;
    }

    if (menor != i) {
        swap(dados[i], dados[menor]);
        corrigeDescendo(menor);
    }
}

//Adiciona um item no heap mantendo a ordenação.

void MinHeap::inserir(const HeapItem& item) {
    if (tamanho < MAX_HEAP) {
        dados[tamanho] = item;
        corrigeSubindo(tamanho);
        tamanho++;
    } else {
        cerr << "Erro: heap cheio ao inserir." << endl;
    }
}

//Remove e retorna o menor item da heap.

HeapItem MinHeap::extrairMinimo() {
    HeapItem raiz = dados[0];
    dados[0] = dados[tamanho - 1];
    tamanho--;
    corrigeDescendo(0);
    return raiz;
}

//Verifica se a heap está vazia.

bool MinHeap::vazio() const {
    return tamanho == 0;
}