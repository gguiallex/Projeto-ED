# Makefile para compilar projeto C++ multiplataforma (Windows e Linux)
#
# Comandos úteis:
#   make       - compila o projeto
#   make run   - compila e executa o programa
#   make clean - remove arquivos objeto e executáveis
#   ./programa.exe (no Windows) - executa o programa (após compilar)
#	./programa (no Linux) - executa o programa (após compilar)
#	
# Estrutura de pastas:
#   arquivos_cpp/   - arquivos fonte .cpp
#   cabecalhos/    - arquivos header .h
#   objetos/       - arquivos objeto .o (gerados na compilação)
#
# Como usar:
#   1) Abra o terminal na pasta do Makefile
#   2) Digite 'make' para compilar
#   3) Digite 'make run' para compilar e executar
#   4) Digite 'make clean' para limpar arquivos compilados
#
# OBS: O Makefile detecta o sistema operacional para gerar o executável correto
#      e usar os comandos adequados de limpeza e execução.


# Caminhos
CPP_DIR = arquivos_cpp
HEADER_DIR = cabecalhos
OBJ_DIR = objetos

# Detecta o sistema e define nome do binário
ifeq ($(OS),Windows_NT)
	BIN = programa.exe
	RM = del /Q objetos\*.o programa.exe 2>nul || exit 0
	RUN_CMD = .\$(BIN)
else
	BIN = programa
	RM = rm -rf objetos/*.o programa programa.exe
	RUN_CMD = ./$(BIN)
endif

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I $(HEADER_DIR)

# Arquivos fonte
SOURCES = $(wildcard $(CPP_DIR)/*.cpp)

# Arquivos objeto
OBJECTS = $(patsubst $(CPP_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Regra principal
all: $(BIN)

# Compila o executável
$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compila arquivos .cpp em .o
$(OBJ_DIR)/%.o: $(CPP_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cria diretório de objetos se não existir
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpa arquivos gerados
clean:
	$(RM)

# Compila e executa o programa
run: all
	$(RUN_CMD)
