//Adrielly Ferreira da Silva, Daniel de Jesus Moreira, Guilherme Alexandre Cunha Silva
//Tema: Dados de Evento dos Atletas em crescente utilizando Merge MultiWay

#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

struct Registro {
    int id;
    char name[50];
    char team[50];
    char games[20];
    int year;
    char season[10];
};

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