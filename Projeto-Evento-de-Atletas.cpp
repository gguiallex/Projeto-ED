// Adrielly Ferreira da Silva, Daniel de Jesus Moreira, Guilherme Alexandre Cunha Silva
// Tema: Dados de Evento dos Atletas em crescente utilizando Merge MultiWay

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Registro
{
private:
    int id;
    char nome[50];
    char cidade[50];
    char esporte[50];
    char evento[100];
    char nacionalidade[10];

public:
    Registro(int i, const char *n, const char *c, const char *e, const char *ev, const char *na);
    void escreverBinario(ofstream &out);
    void lerBinario(ifstream &in);
    static int tamanho();

    int getId() const { return id; }
    const char* getNome() const { return nome; }
    const char* getCidade() const { return cidade; }
    const char* getEsporte() const { return esporte; }
    const char* getEvento() const { return evento; }
    const char* getNacionalidade() const { return nacionalidade; }
};
// Construtor que serve tanto para criar um objeto vazio ou preencher.
Registro::Registro(int i = 0, const char *n = "", const char *c = "", const char *e = "", const char *ev = "", const char *na = "")
{
    id = i;
    strncpy(nome, n, sizeof(nome));
    strncpy(cidade, c, sizeof(cidade));
    strncpy(esporte, e, sizeof(esporte));
    strncpy(evento, ev, sizeof(evento));
    strncpy(nacionalidade, na, sizeof(nacionalidade));

    // Garantir que as strings sempre terminem com "\0" -> indica onde a string termina.
    nome[sizeof(nome - 1)] = '\0';
    cidade[sizeof(nome - 1)] = '\0';
    esporte[sizeof(nome - 1)] = '\0';
    evento[sizeof(nome - 1)] = '\0';
    nacionalidade[sizeof(nome - 1)] = '\0';
}

// Escrever um registro em binário.
void Registro::escreverBinario(ofstream &out)
{
    out.write((char *)&id, sizeof(id));
    out.write(nome, sizeof(nome));
    out.write(cidade, sizeof(cidade));
    out.write(esporte, sizeof(esporte));
    out.write(evento, sizeof(evento));
    out.write(nacionalidade, sizeof(nacionalidade));
}

// Ler um registro em binário.
void Registro::lerBinario(ifstream &in)
{
    in.read((char *)&id, sizeof(id));
    in.read(nome, sizeof(nome));
    in.read(cidade, sizeof(cidade));
    in.read(esporte, sizeof(esporte));
    in.read(evento, sizeof(evento));
    in.read(nacionalidade, sizeof(nacionalidade));
}
// Retorna o tamanho fixo em bytes que um objeto da classe Registro ocupa quando armazenado em arquivo binário.
int Registro::tamanho()
{
    return sizeof(int) + sizeof(nome) + sizeof(cidade) + sizeof(esporte) + sizeof(evento) + sizeof(nacionalidade);
}

void converterCSVParaBinario(char *nomeCSV, char *nomeBinario)
{
    ifstream arquivoCSV(nomeCSV);
    ofstream arquivoBinario(nomeBinario);
    bool erro = false;

    if (!arquivoCSV.is_open())
    {
        cerr << "Erro ao abrir o arquivo CSV!";
        erro = true;
    }

    if (!arquivoBinario.is_open())
    {
        cerr << "Erro ao abrir o arquivo binário!";
        erro = true;
    }

    if (!erro)
    {
        string linha;
        getline(arquivoCSV, linha); // Para ignorar o cabeçalho.

        while (getline(arquivoCSV, linha))
        {
            int id;
            char nome[50], cidade[50], esporte[50], evento[100], nacionalidade[10];
            int lidos = sscanf(linha.c_str(), "%d,%49[^,],%49[^,],%49[^,],%99[^,],%9[^,\n]", &id, nome, cidade, esporte, evento, nacionalidade);

            if (lidos == 6)
            {
                Registro reg(id, nome, cidade, esporte, evento, nacionalidade);
                reg.escreverBinario(arquivoBinario);
            }
        }
        std::cout << "Conversão concluída com sucesso!";
    }

    arquivoCSV.close();
    arquivoBinario.close();
}

void inserir(const char* nomeArquivo) {
    ofstream out(nomeArquivo, ios::binary | ios::app);
    if (!out) {
        cerr << "Erro ao abrir o arquivo binário para escrita." << endl;
        return;
    } 

    int id;
    char nome[50], cidade[50], esporte[50], evento[100], nacionalidade[10];

    cout << "Digite o ID: ";
    cin >> id;
    cin.ignore(); // limpar o buffer

    cout << "Nome: ";
    cin.getline(nome, 50);

    cout << "Cidade: ";
    cin.getline(cidade, 50);
    
    cout << "Esporte: ";
    cin.getline(evento, 100);

    cout << "Nacionalidade: ";
    cin.getline(nacionalidade, 10);

    Registro reg(id, nome, cidade, esporte, evento, nacionalidade);
    reg.escreverBinario(out);

    cout << "Registro inserido com sucesso." << endl;
    out.close();
}

void visualizarEntre(const char* nomeArquivo) {
    ifstream in(nomeArquivo, ios::binary);
    if(!in) {
        cerr << "Erro ao abrir o arquivo binário." << endl;
        return;
    }

    int inicio, fim;
    cout << "Digite a posi;áo inicial (0-based): ";
    cin >> inicio;
    cout << "Digite a posição final: ";
    cin >> fim;

    if(inicio > fim || inicio < 0) {
        cout << "Intervalo inválido!" << endl;
        return;
    }

    Registro reg;
    int posAtual = 0;

    while(in.read((char*)&reg, Registro::tamanho())) {
        if(posAtual >= inicio && posAtual <= fim) {
            cout << "--------------------------" << endl;
            cout << "Posição: " << posAtual << endl;
            cout << "ID: " << reg.getId() << endl;
            cout << "Nome: " << reg.getNome() << endl;
            cout << "Cidade: " << reg.getCidade() << endl;
            cout << "Esporte: " << reg.getEsporte() << endl;
            cout << "Evento: " << reg.getEvento() << endl;
            cout << "Nacionalidade: " << reg.getNacionalidade() << endl;
        }
        posAtual++;
        if(posAtual > fim) break;
    }

    if(posAtual <= inicio) {
        cout << "Faixa fora do tamanho do arquivo." << endl;
    }

    in.close();
}

class MinHeap
{
private:
    Registro *heap;
    int capacidade;
    int tamanho;
    inline int pai(int i);
    inline int esquerdo(int i);
    inline int direito(int i);
    void corrigeDescendo(int i);
    void corrigeSubindo(int i);

public:
    MinHeap(int cap);
    ~MinHeap();
    void imprime();
    Registro retiraRaiz();
    void insere(Registro d);
};

void Menu()
{
    cout << "     Escolha uma Opcao       " << endl;
    cout << "1 - Adicionar Esporte" << endl;
    cout << "2 - Visualizar Entre" << endl;
    cout << "3 - Alterar" << endl;
    cout << "4 - Trocar Posição" << endl;
    cout << "5 - Ordernar" << endl;
    cout << "6 - Imprimir todos os Registros" << endl;
    cout << "0 - Encerrar o Programa" << endl;
}

int main()
{

    converterCSVParaBinario("data_athlete_event.csv", "dados.bin");

    int numero;
    do
    {
        Menu();
        cout << "Escolha uma opção:" << endl;
        cin >> numero;

        switch (numero)
        {
        case 1:
        {
            inserir("dados.bin");
            break;
        }
        case 2:
        {
            visualizarEntre("dados.bin");
            break;
        }
        case 3:
        {
            //alterarRegistro("dados.bin");
            break;
        }
        case 4:
        {
            //trocarRegistros("dados.bin");
            break;
        }
        case 5:
        {
            //ordenarArquivoComMergeMultiway("dados.bin", "ordenado.bin");
            break;
        }
        case 6:
        {
            //imprimirTodos("dados.bin");
            break;
        }
        case 0:
                cout << "Encerrando o programa...\n";
                break;
            default:
                cout << "Opção inválida.\n";
        }
    } while (numero != 0);

    return 0;
}