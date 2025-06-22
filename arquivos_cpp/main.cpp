#include <iostream>
#include "../cabecalhos/manipuladorBinario.h"


using namespace std;


// Exibe o menu de opções para o usuário.

void Menu()
{
    cout << "\n=====================================\n";
    cout << "        MENU DE OPCOES\n";
    cout << "=====================================\n";
    cout << " 1 - Inserir Registro\n";
    cout << " 2 - Visualizar Entre\n";
    cout << " 3 - Alterar Registro\n";
    cout << " 4 - Trocar Posicao\n";
    cout << " 5 - Ordenar Arquivo\n";
    cout << " 6 - Imprimir Todos os Registros\n";
    cout << " 0 - Encerrar o Programa\n";
    cout << "=====================================\n";
    cout << "Escolha uma opcao: ";
}

// Ponto de entrada da aplicação; controla o fluxo do programa.
int main()
{
    manipuladorBinario m("dados1.bin");
    m.converterCsvParaBinario("data_athlete_event.csv", "dados1.bin");
    m.ordenarMergeMultiway();

    int numero;
    do
    {
        Menu();
        cin >> numero;

        switch (numero)
        {
        case 1:
        {
            int pos;
            cout << "Informe a posicao para inserir: ";
            cin >> pos;
            m.inserir(pos);
            break;
        }
        case 2:
        {
            m.visualizarEntre();
            break;
        }
        case 3:
        {
            int pos;
            cout << "Informe a posicaoo do registro que deseja alterar: ";
            cin >> pos;
            Registro novo;
            novo.lerTeclado(); 
            m.alterarRegistroNaPosicao(pos, novo);
            break;
        }
        case 4:
        {
            int pos1, pos2;
            cout << "Informe a primeira posicao: ";
            cin >> pos1;
            cout << "Informe a segunda posicao: ";
            cin >> pos2;
            m.trocarRegistros(pos1, pos2);
            break;
        }
        case 5:
        {
            m.ordenarMergeMultiway();
            break;
        }
        case 6:
        {
            m.imprimirTodos();
            break;
        }
        case 0:
            cout << "Encerrando o programa...\n";
            break;
        default:
            cout << "Opcao invalida. Tente novamente.\n";
        }
    } while (numero != 0);

    return 0;
}
