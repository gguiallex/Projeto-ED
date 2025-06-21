#include "manipuladorBinario.h"
#include "minHeap.h"

manipuladorBinario::manipuladorBinario(string nome) {
    nomeArquivo = nome;
}

void manipuladorBinario::alterarRegistroNaPosicao(int posicao, const Registro &novo) {
    bool sucesso = true;

    fstream arquivo(nomeArquivo.c_str(), ios::in | ios::out | ios::binary);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo binário para alteração!" << endl;
        sucesso = false;
    }

    if (sucesso) {
        int offset = posicao * Registro::tamanho();
        arquivo.seekp(offset, ios::beg);
        novo.escreverBinario(arquivo);
        arquivo.close();
    }
}


void manipuladorBinario::trocarRegistros(int pos1, int pos2) {
    bool sucesso = true;
    fstream arquivo(nomeArquivo.c_str(), ios::in | ios::out | ios::binary);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo binário para troca!" << endl;
        sucesso = false;
    }

    if (sucesso) {
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
        sucesso = false;
    }

    if(sucesso){

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

        while (in.read((char *)&reg, Registro::tamanho()) && posAtual <= fim) {
            if (posAtual >= inicio) {
                cout << "Posição: " << posAtual << " | ";
                reg.imprimirLinha();
            }
            posAtual++;
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
    bool sucesso = true;
    ifstream in(nomeArquivo.c_str(), ios::binary);
    if (!in.is_open()) {
        cerr << "Erro ao abrir o arquivo binário." << endl;
        sucesso = false;
    }

    if (sucesso) {
        Registro reg;
        int pos = 0;

        reg.lerBinario(in);
        while (in) {
            cout << "Posição: " << pos << " | ";
            reg.imprimirLinha();
            pos++;
            reg.lerBinario(in);
        }
    }

    if (in.is_open()) {
        in.close();
    }
}


void manipuladorBinario::converterCsvParaBinario(char* nomeCsv,const char* nomeBinario) {
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
    cout<<"Conversão de CSV para binário executada com sucesso!"<<endl;
}
}

void manipuladorBinario::ordenarMergeMultiway(int bufferSize) {
    bool sucesso = true;

    ifstream in(nomeArquivo.c_str(), ios::binary);
    if (!in.is_open()) {
        cerr << "Erro ao abrir arquivo binário de entrada!" << endl;
        sucesso = false;
    }

    char nomesTemporarios[MAX_RUNS][20];
    int totalRuns = 0;

    if (sucesso) {
        while (!in.eof() && totalRuns < MAX_RUNS) {
            Registro buffer[MAX_BUFFER];
            int lidos = 0;
            Registro temp;

            while (lidos < bufferSize && temp.lerBinario(in)) {
                buffer[lidos++] = temp;
            }

            if (lidos > 0) {
                // Ordenação por inserção pelo campo esporte
                for (int i = 1; i < lidos; ++i) {
                    Registro chave = buffer[i];
                    int j = i - 1;
                    while (j >= 0 && strcmp(buffer[j].esporte, chave.esporte) > 0) {
                        buffer[j + 1] = buffer[j];
                        j--;
                    }
                    buffer[j + 1] = chave;
                }

                sprintf(nomesTemporarios[totalRuns], "run%d.bin", totalRuns);
                ofstream out(nomesTemporarios[totalRuns], ios::binary);
                if (out.is_open()) {
                    for (int i = 0; i < lidos; ++i) {
                        buffer[i].escreverBinario(out);
                    }
                    out.close();
                    totalRuns++;
                } else {
                    cerr << "Erro ao criar arquivo temporário." << endl;
                    sucesso = false;
                }
            }
        }
        in.close();
    }

    const char* tempSaida = "ordenado_temp.bin";
    ofstream saida(tempSaida, ios::binary);
    if (!saida.is_open()) {
        cerr << "Erro ao criar arquivo temporário de saída!" << endl;
        sucesso = false;
    }

    if (sucesso) {
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

        saida.close();
    }

    if (sucesso) {
        if (remove(nomeArquivo.c_str()) != 0) {
            cerr << "Erro ao remover arquivo original!" << endl;
            sucesso = false;
        }

        if (rename(tempSaida, nomeArquivo.c_str()) != 0) {
            cerr << "Erro ao renomear arquivo temporário para nome original!" << endl;
            sucesso = false;
        }
    }

    if (sucesso) {
        cout << "Ordenação finalizada. Arquivo '" << nomeArquivo << "' sobrescrito com os dados ordenados." << endl;
    }
}