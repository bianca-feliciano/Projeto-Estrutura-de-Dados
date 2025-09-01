#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

// Estrutura de dados
struct Info {
    int id;
    char nome[100];
    char equipe[100];
    char jogos[50];
    int ano;
    char estacao[20];
};

// Classe responsável por ler CSV e gravar em binário
class LeitorCSV {
private:
    string nomeCSV;
    string nomeBin;

public:
    LeitorCSV(const string& csv, const string& bin) : nomeCSV(csv), nomeBin(bin) {}

    void processar() {
        ifstream arquivocsv(nomeCSV);
        ofstream arquivoBin(nomeBin, ios::binary);

        if (!arquivoBin.is_open() || !arquivocsv.is_open()) {
            cerr << "Erro ao abrir arquivos CSV ou BIN!" << endl;
            exit(1);
        }

        Info aux;
        string linha;
        getline(arquivocsv, linha); // pular o cabeçalho

        while (getline(arquivocsv, linha)) {
            string campos[6];
            string campo = "";
            int campoAtual = 0;
            bool dentroAspas = false;

            for (size_t i = 0; i < linha.length(); i++) {
                char c = linha[i];
                if (c == '"') {
                    if (i + 1 < linha.length() && linha[i + 1] == '"') {
                        campo += '"';
                        i++;
                    } else {
                        dentroAspas = !dentroAspas;
                    }
                } else if (c == ',' && !dentroAspas) {
                    if (campoAtual < 6) {
                        campos[campoAtual++] = campo;
                        campo = "";
                    }
                } else {
                    campo += c;
                }
            }
            if (campoAtual < 6) {
                campos[campoAtual++] = campo;
            }

            try {
                aux.id = stoi(campos[0]);
            } catch (...) {
                aux.id = -1;
            }

            strncpy(aux.nome, campos[1].c_str(), sizeof(aux.nome) - 1);
            aux.nome[sizeof(aux.nome) - 1] = '\0';

            strncpy(aux.equipe, campos[2].c_str(), sizeof(aux.equipe) - 1);
            aux.equipe[sizeof(aux.equipe) - 1] = '\0';

            strncpy(aux.jogos, campos[3].c_str(), sizeof(aux.jogos) - 1);
            aux.jogos[sizeof(aux.jogos) - 1] = '\0';

            try {
                aux.ano = stoi(campos[4]);
            } catch (...) {
                aux.ano = -1;
            }

            strncpy(aux.estacao, campos[5].c_str(), sizeof(aux.estacao) - 1);
            aux.estacao[sizeof(aux.estacao) - 1] = '\0';

            arquivoBin.write(reinterpret_cast<char*>(&aux), sizeof(Info));
        }

        cout << "Leitura do CSV e gravação no binário concluídas!" << endl;

        arquivoBin.close();
        arquivocsv.close();
    }
};

// Classe responsável por ler o binário e imprimir como CSV
class ConversorBinarioCSV {
private:
    string nomeArquivoBin;

public:
    ConversorBinarioCSV(const string& nome) : nomeArquivoBin(nome) {}

    void imprimirComoCSV() {
        ifstream arquivoBin(nomeArquivoBin, ios::binary);
        if (!arquivoBin.is_open()) {
            cerr << "Erro ao abrir o arquivo binário!" << endl;
            return;
        }

        Info registro;
        cout << "\nid,nome,equipe,jogos,ano,estacao" << endl;

        while (arquivoBin.read(reinterpret_cast<char*>(&registro), sizeof(Info))) {
            cout << registro.id << ",";
            cout << '"' << registro.nome << '"' << ",";
            cout << '"' << registro.equipe << '"' << ",";
            cout << '"' << registro.jogos << '"' << ",";
            cout << registro.ano << ",";
            cout << registro.estacao << endl;
        }

        arquivoBin.close();
    }
};

// Função principal
int main() {
    LeitorCSV leitor("data_athlete_game.csv", "csv_em_binario.dat");
    leitor.processar();

    ConversorBinarioCSV conversor("csv_em_binario.dat");
    conversor.imprimirComoCSV();

    return 0;
}
