#include <iostream>
#include "cabecalhos/manipuladorBinario.h"
using namespace std;

void Menu()
{
    cout << "\n=====================================\n";
    cout << "        MENU DE OPÇÕES\n";
    cout << "=====================================\n";
    cout << " 1 - Adicionar Esporte\n";
    cout << " 2 - Visualizar Entre\n";
    cout << " 3 - Alterar Registro\n";
    cout << " 4 - Trocar Posição\n";
    cout << " 5 - Ordenar Arquivo\n";
    cout << " 6 - Imprimir Todos os Registros\n";
    cout << " 0 - Encerrar o Programa\n";
    cout << "=====================================\n";
    cout << "Escolha uma opção: ";
}

int main()
{
    manipuladorBinario m("dados.bin");
    m.converterCsvParaBinario("data_athlete_event.csv", "dados.bin");

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
            cout << "Informe a posição para inserir: ";
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
            // m.alterarRegistroNaPosicao();
            break;
        }
        case 4:
        {
            // m.trocarRegistros();
            break;
        }
        case 5:
        {
            // m.ordenarMergeMultiway("ordenado.bin");
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
            cout << "Opção inválida. Tente novamente.\n";
        }
    } while (numero != 0);

    return 0;
}
