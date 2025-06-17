#include "cabecalhos/manipuladorBinario.h"
#include "cabecalhos/minHeap.h"

manipuladorBinario::manipuladorBinario(string nome) {
    nomeArquivo = nome;
}

void manipuladorBinario::alterarRegistroNaPosicao(int posicao, const Registro &novo) {
    fstream arquivo(nomeArquivo.c_str(), ios::in | ios::out | ios::binary);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo binário para alteração!";
        return;
    }
    int offset = posicao * Registro::tamanho();
    arquivo.seekp(offset, ios::beg);
    novo.escreverBinario(arquivo);
    arquivo.close();
}

void manipuladorBinario::trocarRegistros(int pos1, int pos2) {
    fstream arquivo(nomeArquivo.c_str(), ios::in | ios::out | ios::binary);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo binário para troca!" << endl;
        return;
    }
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
    ifstream in(nomeArquivo.c_str(), ios::binary);
    if (!in) {
        cerr << "Erro ao abrir o arquivo binário." << endl;
        return;
    }

    int inicio = 0, fim = 0;
    cout << "Digite a posição inicial (0-based): ";
    cin >> inicio;
    cout << "Digite a posição final: ";
    cin >> fim;

    if (inicio > fim || inicio < 0) {
        cout << "Intervalo inválido!" << endl;
        in.close();
        return;
    }

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

    in.close();
}

void manipuladorBinario::imprimirTodos() {
    ifstream in(nomeArquivo.c_str(), ios::binary);
    if (!in.is_open()) {
        cerr << "Erro ao abrir o arquivo binário." << endl;
        return;
    }

    Registro reg;
    int pos = 0;

    reg.lerBinario(in);
    while (in) {
        cout << "Posição: " << pos << " | ";
        reg.imprimirLinha();
        pos++;
        reg.lerBinario(in);
    }

    in.close();
}

void manipuladorBinario::converterCsvParaBinario(char *nomeCsv, const char *nomeBinario) {
    ifstream csv(nomeCsv);
    ofstream bin(nomeBinario, ios::binary);
    if (!csv || !bin) {
        cout << "Erro ao abrir os arquivos." << endl;
        return;
    }

    string linha;
    getline(csv, linha); // Pula cabeçalho

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

void manipuladorBinario::ordenarMergeMultiway(const string& nomeSaida, int bufferSize) {
    ifstream in(nomeArquivo.c_str(), ios::binary);
    if (!in) {
        cerr << "Erro ao abrir arquivo binário de entrada!" << endl;
        return;
    }

    char nomesTemporarios[MAX_RUNS][20];
    int totalRuns = 0;

    while (!in.eof()) {
        Registro buffer[MAX_BUFFER];
        int lidos = 0;
        Registro temp;

        while (lidos < bufferSize && temp.lerBinario(in)) {
            buffer[lidos++] = temp;
        }

        if (lidos == 0) break;

        // Ordenação simples por inserção
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
        for (int i = 0; i < lidos; ++i) {
            buffer[i].escreverBinario(out);
        }
        out.close();
        totalRuns++;
    }
    in.close();

    ofstream saida(nomeSaida.c_str(), ios::binary);
    if (!saida) {
        cerr << "Erro ao abrir arquivo de saída!" << endl;
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

    saida.close();
    cout << "Ordenação finalizada. Arquivo gerado: " << nomeSaida << endl;
}

