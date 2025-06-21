// Adrielly Ferreira da Silva, Daniel de Jesus Moreira, Guilherme Alexandre Cunha Silva
// Tema: Dados de Evento dos Atletas em crescente utilizando Merge MultiWay

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;

// Constantes globais
const int MAX_RUNS = 100;
const int MAX_HEAP = MAX_RUNS;
const int MAX_BUFFER = 1000;

// ======================= Classe Registro ========================
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
    cout << "ID: " << id << " | Nome: " << nome << " | Cidade: " << cidade
         << " | Esporte: " << esporte << " | Evento: " << evento
         << " | Nacionalidade: " << nacionalidade << endl;
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

// ======================= Struct HeapItem ========================
struct HeapItem {
    Registro reg;
    int origem;
};

// ======================= Classe MinHeap ========================
class MinHeap {
private:
    HeapItem dados[MAX_HEAP];
    int tamanho;

    void corrigeSubindo(int i) {
        while (i > 0) {
            int pai = (i - 1) / 2;
            if (dados[i].reg.id < dados[pai].reg.id) {
                swap(dados[i], dados[pai]);
                i = pai;
            } else {
                return;
            }
        }
    }

    void corrigeDescendo(int i) {
        while (2 * i + 1 < tamanho) {
            int esq = 2 * i + 1;
            int dir = 2 * i + 2;
            int menor = i;

            if (esq < tamanho && dados[esq].reg.id < dados[menor].reg.id) menor = esq;
            if (dir < tamanho && dados[dir].reg.id < dados[menor].reg.id) menor = dir;

            if (menor != i) {
                swap(dados[i], dados[menor]);
                i = menor;
            } else {
                return;
            }
        }
    }

public:
    MinHeap() : tamanho(0) {}

    void inserir(const HeapItem& item) {
        if (tamanho < MAX_HEAP) {
            dados[tamanho] = item;
            corrigeSubindo(tamanho);
            tamanho++;
        }
    }

    HeapItem extrairMinimo() {
        HeapItem raiz = dados[0];
        dados[0] = dados[--tamanho];
        corrigeDescendo(0);
        return raiz;
    }

    bool vazio() const { return tamanho == 0; }
};

// ======================= Ordenação Merge MultiWay ========================
void ordenarMergeMultiway(const string& nomeEntrada, const string& nomeSaida, int bufferSize = MAX_BUFFER) {
    ifstream in(nomeEntrada, ios::binary);
    if (!in) {
        cerr << "Erro ao abrir arquivo binário de entrada!" << endl;
        return;
    }

    char nomesTemporarios[MAX_RUNS][20];
    int totalRuns = 0;

bool continuar = true;
while (continuar) {
    Registro buffer[MAX_BUFFER];
    int lidos = 0;
    Registro temp;

    while (lidos < bufferSize && temp.lerBinario(in)) {
        buffer[lidos++] = temp;
    }

    if (lidos == 0) {
        continuar = false;
    } else {
        // Ordenação e escrita de run
        for (int i = 1; i < lidos; ++i) {
            Registro chave = buffer[i];
            int j = i - 1;
            while (j >= 0 && buffer[j].obterId() > chave.obterId()) {
                buffer[j + 1] = buffer[j];
                j--;
            }
            buffer[j + 1] = chave;
        }

        sprintf(nomesTemporarios[totalRuns], "run%d.bin", totalRuns);
        ofstream out(nomesTemporarios[totalRuns], ios::binary);
        for (int i = 0; i < lidos; ++i)
            buffer[i].escreverBinario(out);
        out.close();
        totalRuns++;
    }
}

    in.close();
    if (totalRuns == 0) {
    cout << "[DEBUG] Nenhuma run foi gerada. Verifique se o arquivo de entrada possui registros válidos." << endl;
    return;
    }

    ofstream saida(nomeSaida, ios::binary);
    cout << "[DEBUG] Arquivo de saída aberto com sucesso.\n";

    if (!saida.is_open()) {
        cerr << "Erro ao abrir o arquivo de saída: " << nomeSaida << endl;
        return;
    }


    MinHeap heap;
    ifstream arquivos[MAX_RUNS];

    for (int i = 0; i < totalRuns; ++i) {
        arquivos[i].open(nomesTemporarios[i], ios::binary);
        Registro r;
        if (arquivos[i].is_open() && r.lerBinario(arquivos[i])) {
            HeapItem item = {r, i};
            heap.inserir(item);
        }
    }

    while (!heap.vazio()) {
        HeapItem menor = heap.extrairMinimo();
        menor.reg.escreverBinario(saida);

        Registro proximo;
        if (arquivos[menor.origem].is_open() && proximo.lerBinario(arquivos[menor.origem])) {
            HeapItem item = {proximo, menor.origem};
            heap.inserir(item);
        }
    }

    for (int i = 0; i < totalRuns; ++i) {
        if (arquivos[i].is_open()) arquivos[i].close();
        remove(nomesTemporarios[i]);
    }

    cout << "[DEBUG] Finalizando escrita e fechando arquivo de saída.\n";

    saida.close();
    cout << "Ordenação finalizada. Arquivo gerado: " << nomeSaida << endl;
}

//Classe manipuladorBinario

class manipuladorBinario{
    private:
    string nomeArquivo;
    public:
    manipuladorBinario(string nome);
    void alterarRegistroNaPosicao(int posicao, const Registro& novo);
    void trocarRegistros(int pos1, int pos2);
    void inserir(int posicao);
    void visualizarEntre();
    void imprimirTodos();
    void converterCsvParaBinario(const char* nomeCsv, const char* nomeBinario);


};

manipuladorBinario::manipuladorBinario(string nome){
    nomeArquivo = nome;

}

void manipuladorBinario::alterarRegistroNaPosicao(int posicao, const Registro& novo){
    fstream arquivo(nomeArquivo.c_str(), ios::in | ios::out | ios::binary);
    if(!arquivo.is_open()){
        cerr<<"Erro ao abrir o arquivo binário para alteração!";
    }
    else{
        int offset = posicao * Registro::tamanho();
        arquivo.seekp(offset, ios::beg);
        novo.escreverBinario(arquivo);
        arquivo.close();
    }

}

void manipuladorBinario::trocarRegistros(int pos1, int pos2) {
    fstream arquivo(nomeArquivo.c_str(), ios::in | ios::out | ios::binary);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo binário para troca!" << endl;
    } else {
        Registro reg1, reg2;

        arquivo.seekg(pos1 * Registro::tamanho(), ios::beg);
        reg1.lerBinario(arquivo);

        arquivo.seekg(pos2 * Registro::tamanho(), ios::beg);
        reg2.lerBinario(arquivo);

        arquivo.seekp(pos1 * Registro::tamanho(), ios::beg);
        reg2.escreverBinario(arquivo);

        arquivo.seekp(pos2 * Registro::tamanho(), ios::beg);
        reg1.escreverBinario(arquivo);

        arquivo.close();
        }
    }

void manipuladorBinario::inserir(int posicao) {
    Registro novoRegistro;
    novoRegistro.lerTeclado();
    bool sucesso = true; 

    fstream arquivo(nomeArquivo.c_str(), ios::in | ios::out | ios::binary);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo binário para inserção!" << endl;
        sucesso = false;
    }

    if(sucesso){
    arquivo.seekg(0, ios::end); 
    long long tamanhoArquivo = arquivo.tellg(); 
    int numRegistros = 0;
    if (tamanhoArquivo > 0) { 
        numRegistros = tamanhoArquivo / Registro::tamanho();
    }

    if (posicao < 0 || posicao > numRegistros) {
        cout << "Posição inválida. A posição deve estar entre 0 e " << numRegistros << "." << endl;
        arquivo.close();
        return;
    }

    if (posicao == numRegistros) {
        arquivo.seekp(0, ios::end); 
        novoRegistro.escreverBinario(arquivo); 
        cout << "Registro inserido no final do arquivo." << endl;
    } else { 
        
        Registro tempReg;
        for (int i = numRegistros - 1; i >= posicao; --i) {
            arquivo.seekg(i * Registro::tamanho(), ios::beg); 
            tempReg.lerBinario(arquivo); 

            arquivo.seekp((i + 1) * Registro::tamanho(), ios::beg); 
            tempReg.escreverBinario(arquivo); 
        }

        arquivo.seekp(posicao * Registro::tamanho(), ios::beg);
        novoRegistro.escreverBinario(arquivo);
        cout << "Registro inserido na posição " << posicao << " com sucesso." << endl;
    }
}

    arquivo.close(); 
}

void manipuladorBinario::visualizarEntre() {
    bool sucesso = true;
    ifstream in(nomeArquivo.c_str(), ios::binary);

    if (!in) {
        cerr << "Erro ao abrir o arquivo binário." << endl;
        sucesso = false;
    }

    int inicio = 0, fim = 0;
    if (sucesso) {
        cout << "Digite a posição inicial (0-based): ";
        cin >> inicio;
        cout << "Digite a posição final: ";
        cin >> fim;

        if (inicio > fim || inicio < 0) {
            cout << "Intervalo inválido!" << endl;
            sucesso = false;
        }
    }

    if (sucesso) {
        Registro reg;
        int posAtual = 0;
        bool terminou = false;

        while (in.read((char*)&reg, Registro::tamanho()) && !terminou) {
            if (posAtual >= inicio && posAtual <= fim) {
                cout << "Posição: " << posAtual << " | ";
                reg.imprimirLinha();
            }
            posAtual++;

            if (posAtual > fim) {
                terminou = true;
            }
        }

        if (posAtual <= inicio) {
            cout << "Faixa fora do tamanho do arquivo." << endl;
        }
    }

    if (in.is_open()) {
        in.close();
    }
}

void manipuladorBinario::imprimirTodos() {
    ifstream in(nomeArquivo.c_str(), ios::binary);
    bool leituraOk = true;

    if (!in.is_open()) {
        cerr << "Erro ao abrir o arquivo binário." << endl;
        leituraOk = false;
    }

    const int registrosPorPagina = 10;
    int contador = 0;

    if (leituraOk) {
        Registro reg;
        int pos = 0;

    reg.lerBinario(in);
    while (reg.lerBinario(in)) {
        cout << "Posicao: " << pos << " | ";
        reg.imprimirLinha();
        pos++;
        contador++;

        if (contador == registrosPorPagina){
            contador = 0;
            cout << "\nPressione ENTER para continuar...";
            cin.ignore();
            cin.get();
            cout <<"\n";
        }
        //reg.lerBinario(in);
        }

    
        in.close();
    }
}

void manipuladorBinario::converterCsvParaBinario(const char* nomeCsv,const char* nomeBinario) {
    ifstream csv(nomeCsv);
    ofstream bin(nomeBinario, ios::binary);
    bool sucesso = true;

    if (!csv || !bin) {
        cout << "Erro ao abrir os arquivos." << endl;
        sucesso = false;
    }

    if(sucesso){

    string linha;
    getline(csv, linha); 

    while (getline(csv, linha)) {
    string campos[6];
    int campoAtual = 0;
    string campoTemporario = "";
    bool dentroDeAspas = false;

    for (size_t i = 0; i < linha.length(); i++) {
        char c = linha[i];

        if (c == '"') {
            dentroDeAspas = !dentroDeAspas; 
            campoTemporario += c; 
        } else if (c == ',' && !dentroDeAspas && campoAtual < 5) {
            campos[campoAtual++] = campoTemporario;
            campoTemporario = "";
        } else {
            campoTemporario += c;
        }
    }

    campos[5] = campoTemporario; 

    Registro r;
    r.id = stoi(campos[0]);

    memset(r.nome, 0, sizeof(r.nome));
    memset(r.cidade, 0, sizeof(r.cidade));
    memset(r.esporte, 0, sizeof(r.esporte));
    memset(r.evento, 0, sizeof(r.evento));
    memset(r.nacionalidade, 0, sizeof(r.nacionalidade));

    strncpy(r.nome, campos[1].c_str(), sizeof(r.nome) - 1);
    strncpy(r.cidade, campos[2].c_str(), sizeof(r.cidade) - 1);
    strncpy(r.esporte, campos[3].c_str(), sizeof(r.esporte) - 1);
    strncpy(r.evento, campos[4].c_str(), sizeof(r.evento) - 1);
    strncpy(r.nacionalidade, campos[5].c_str(), sizeof(r.nacionalidade) - 1);

    r.escreverBinario(bin);
}

}
}

void Menu() {
    cout << "\n----- MENU -----\n";
    cout << "1 - Adicionar Esporte\n";
    cout << "2 - Visualizar Entre\n";
    cout << "3 - Alterar\n";
    cout << "4 - Trocar Posição\n";
    cout << "5 - Ordenar com Merge MultiWay\n";
    cout << "6 - Imprimir todos os Registros\n";
    cout << "0 - Encerrar o Programa\n";
}

int main() {
    manipuladorBinario m("dados.bin");
    //m.converterCsvParaBinario("data_athlete_event.csv", "dados.bin");

    ifstream teste("dados.bin", ios::binary);
    if (!teste || teste.peek() == EOF) {
        cout << "[ERRO] Arquivo dados.bin não existe ou está vazio!" << endl;
    } else {
        cout << "[DEBUG] dados.bin existe e será ordenado." << endl;
    }
    teste.close();
    int numero;
    do {
        Menu();
        cout << "\nEscolha uma opção: ";
        cin >> numero;

        switch (numero) {
        case 1: {
            int pos;
            cout << "Informe a posição para inserir: ";
            cin >> pos;
            m.inserir(pos);
            break;
        }
        case 2: m.visualizarEntre(); break;
        case 3: {
            int pos;
            cout << "Informe a posição para alterar: ";
            cin >> pos;
            Registro novo;
            novo.lerTeclado();
            m.alterarRegistroNaPosicao(pos, novo);
            break;
        }
        case 4: {
            int p1, p2;
            cout << "Informe a primeira posição: ";
            cin >> p1;
            cout << "Informe a segunda posição: ";
            cin >> p2;
            m.trocarRegistros(p1, p2);
            break;
        }
        case 5:{
            ordenarMergeMultiway("dados.bin", "ordenado.bin");
            remove("dados.bin");
            rename("ordenado.bin", "dados.bin");
            break;
        }
        case 6:{
            m.imprimirTodos();
            break;
        }
        case 0:{
            cout << "Encerrando o programa...\n"; 
            break;
        }
        default: cout << "Opção inválida.\n"; break;
        }
    } while (numero != 0);

    return 0;
}