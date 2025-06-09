//Adrielly Ferreira da Silva, Daniel de Jesus Moreira, Guilherme Alexandre Cunha Silva
//Tema: Dados de Evento dos Atletas em crescente utilizando Merge MultiWay

#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

class Registro {
    private: 
    int id;
    char nome[50];
    char cidade[50];
    char esporte[50];
    char evento[100];
    char nacionalidade[10];
    public:
    Registro(int i, const char* n, const char* c, const char* e, const char* ev, const char* na);
    void escreverBinario(ofstream& out);
    void lerBinario(ifstream& in);
    static int tamanho();
};
    //Construtor que serve tanto para criar um objeto vazio ou preencher.
    Registro::Registro(int i = 0, const char* n = "", const char* c = "", const char* e = "", const char* ev = "", const char* na = ""){
        id = i;
        strncpy(nome, n, sizeof(nome));
        strncpy(cidade, c, sizeof(cidade));
        strncpy(esporte, e, sizeof(esporte));
        strncpy(evento, ev, sizeof(evento));
        strncpy(nacionalidade, na, sizeof(nacionalidade));

        // Garantir que as strings sempre terminem com "\0" -> indica onde a string termina.
        nome[sizeof(nome-1)] = '\0';
        cidade[sizeof(nome-1)] = '\0';
        esporte[sizeof(nome-1)] = '\0';
        evento[sizeof(nome-1)] = '\0';
        nacionalidade[sizeof(nome-1)] = '\0';
    }

        //Escrever um registro em binário.
    void Registro::escreverBinario(ofstream& out){
        out.write((char*)&id, sizeof(id));
        out.write(nome, sizeof(nome));
        out.write(cidade, sizeof(cidade));
        out.write(esporte, sizeof(esporte));
        out.write(evento, sizeof(evento));
        out.write(nacionalidade, sizeof(nacionalidade));   
    }

        //Ler um registro em binário.
    void Registro::lerBinario(ifstream& in){
        in.read((char*)&id, sizeof(id));
        in.read(nome, sizeof(nome));
        in.read(cidade, sizeof(cidade));
        in.read(esporte, sizeof(esporte));
        in.read(evento, sizeof(evento));
        in.read(nacionalidade, sizeof(nacionalidade));
    }
        //Retorna o tamanho fixo em bytes que um objeto da classe Registro ocupa quando armazenado em arquivo binário.
    int Registro::tamanho(){
        return sizeof(int) + sizeof(nome) + sizeof(cidade) + sizeof(esporte) + sizeof(evento) + sizeof(nacionalidade);
    }

    void converterCSVParaBinario(char* nomeCSV, char* nomeBinario){
        ifstream arquivoCSV(nomeCSV);
        ofstream arquivoBinario(nomeBinario);
        bool erro = false;

        if(!arquivoCSV.is_open()){
            cerr<<"Erro ao abrir o arquivo CSV!";
            erro = true;
        }

        if(!arquivoBinario.is_open()){
            cerr<<"Erro ao abrir o arquivo binário!";
            erro = true;
        }

        if(!erro){
            string linha;
            getline(arquivoCSV, linha); // Para ignorar o cabeçalho.

            while(getline(arquivoCSV, linha)){
                int id;
                char nome[50], cidade[50], esporte[50], evento[100], nacionalidade[10];
                int lidos = sscanf(linha.c_str(), "%d,%49[^,],%49[^,],%49[^,],%99[^,],%9[^,\n]", &id, nome, cidade, esporte, evento, nacionalidade);

                if(lidos == 6){
                    Registro reg(id, nome, cidade, esporte, evento, nacionalidade);
                    reg.escreverBinario(arquivoBinario);
                }

                
            }
            std::cout<<"Conversão concluída com sucesso!";

        }

        arquivoCSV.close();
        arquivoBinario.close();


    }






class MinHeap {
    private:
        dado* heap;
        int capacidade;
        int tamanho;
        inline int pai(int i);
        inline int esquerdo(int i);
        inline int direito(int i); 
        void corrigeDescendo(int i); 
        void corrigeSubindo(int i);
    public:
        MaxHeap(int cap);
        ~MaxHeap();
        void imprime();
        dado retiraRaiz();
        void insere(dado d);
};

void Menu(){
    cout<<"     Escolha uma Opcao       "<<endl;
    cout<<"1 - Buscar Pelo Id"<<endl;
    cout<<"2 - Buscar Pelo Nome"<<endl;
    cout<<"3 - Inserir nova Informacao"<<endl;
    cout<<"4 - Editar" << endl;
    cout<<"5 - Excluir" << endl;
    cout<<"6 - Ordernar"<<endl;
    cout<<"0 - Encerrar o Programa"<<endl;
    
}

int main(){

    cout<<"informe de qual arquivo deseja carregar os dados: "<<endl
		<<"1 - carregar dados do arquivo binario"<<endl
		<<"2 - carregar dados do arquivo CSV"<<endl;

string nomeArq = "arquivo.csv";
int capacidade = qntPessoas("arquivo.csv");
int tamanhoAtual = capacidade;

Bandas* bandas = new Bandas[capacidade];

int opcao;
	cin>> opcao;
    bool realizou = false;
    do{
        if(opcao == 1){
            tamanhoAtual = lerBinario(bandas, "Bandas.dat");
            if (tamanhoAtual == 0) {
                cout << "Falha ao ler o arquivo binário ou o arquivo está vazio." << endl;
            } else {
                realizou = true;
            }
        }else if(opcao == 2){
            if(!lerDadosCSV(bandas,"arquivo.csv",capacidade)){
                cout<<"falha ao ler"<<endl;
            }else{ 
                tamanhoAtual = capacidade;
                realizou = true;
                }
        }else{
            cout<< "Opcao invalida. Tente novamente..." << endl;
            cin>> opcao;
        }
    }while(not realizou);

    int numero;
    do
    {
        Menu();
        cout<<"Escolha uma opcao"<<endl;
        cin>>numero;

        switch (numero)
        {
            case 1:{
                BuscaId(bandas, tamanhoAtual);
                break;
            }
            case 2:{
                buscarNome(bandas,tamanhoAtual);
                break;
            }
            case 3:{
                Inserir(bandas, tamanhoAtual, capacidade);
                break;
            }
            case 4:{
                editar(bandas,tamanhoAtual);
                break;
            }
            case 5:{
                Remove_componente(bandas, tamanhoAtual);
                break;
            }
            case 6:{
                Exibir(bandas, tamanhoAtual);
                break;
            }
            case 7:{
                salvarBinario(bandas, "Bandas.dat", tamanhoAtual);
                salvaCSV(bandas, "arquivo.csv", tamanhoAtual);
                cout<<"Dados salvos nos arquivos!"<<endl;
                break;
            }
        }

    } while (numero != 0);

    cout<<"Encerrando o programa...."<<endl;

    delete[] bandas;

    return 0;
}