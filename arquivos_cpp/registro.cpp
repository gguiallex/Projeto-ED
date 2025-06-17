#include "registro.h"

Registro::Registro(int i, const char *n, const char *c, const char *e, const char *ev, const char *na) {
    id = i;
    strncpy(nome, n, sizeof(nome));
    strncpy(cidade, c, sizeof(cidade));
    strncpy(esporte, e, sizeof(esporte));
    strncpy(evento, ev, sizeof(evento));
    strncpy(nacionalidade, na, sizeof(nacionalidade));

    nome[sizeof(nome) - 1] = '\0';
    cidade[sizeof(cidade) - 1] = '\0';
    esporte[sizeof(esporte) - 1] = '\0';
    evento[sizeof(evento) - 1] = '\0';
    nacionalidade[sizeof(nacionalidade) - 1] = '\0';
}

void Registro::escreverBinario(ostream &out) const {
    out.write((char *)&id, sizeof(id));
    out.write(nome, sizeof(nome));
    out.write(cidade, sizeof(cidade));
    out.write(esporte, sizeof(esporte));
    out.write(evento, sizeof(evento));
    out.write(nacionalidade, sizeof(nacionalidade));
}

bool Registro::lerBinario(istream &in) {
    in.read((char *)&id, sizeof(id));
    in.read(nome, sizeof(nome));
    in.read(cidade, sizeof(cidade));
    in.read(esporte, sizeof(esporte));
    in.read(evento, sizeof(evento));
    in.read(nacionalidade, sizeof(nacionalidade));
    return in.gcount() > 0;
}

void Registro::imprimirLinha() const {
    cout << "ID: " << id << " | "
         << "Nome: " << nome << " | "
         << "Cidade: " << cidade << " | "
         << "Esporte: " << esporte << " | "
         << "Evento: " << evento << " | "
         << "Nacionalidade: " << nacionalidade << endl;
}

void Registro::lerTeclado() {
    cout << "Digite o ID: ";
    cin >> id;
    cin.ignore();

    cout << "Nome: ";
    cin.getline(nome, 50);
    cout << "Cidade: ";
    cin.getline(cidade, 50);
    cout << "Esporte: ";
    cin.getline(esporte, 50);
    cout << "Evento: ";
    cin.getline(evento, 100);
    cout << "Nacionalidade: ";
    cin.getline(nacionalidade, 10);
}

int Registro::tamanho() {
    return sizeof(int) + sizeof(nome) + sizeof(cidade) + sizeof(esporte) + sizeof(evento) + sizeof(nacionalidade);
}
