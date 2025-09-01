#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Info {
    int id;
    char nome[100];
    char equipe[100];
    char jogos[50];
    int ano;
    char estacao[20];
};

struct EntradaHeap {
    Info dado;
    int origem;
};

class GerenciadorDados {
private:
    const string entradaBin = "csv_em_binario.dat";
    const string ordenadoBin = "ordenado.dat";
    const int TAM_RAM = 1000;

    void ordenar(Info* arr, int n) {
        for (int i = 1; i < n; ++i) {
            Info chave = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j].id < chave.id) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = chave;
        }
    }

    void trocar(EntradaHeap& a, EntradaHeap& b) {
        EntradaHeap tmp = a;
        a = b;
        b = tmp;
    }

    void subirHeap(EntradaHeap* heap, int pos) {
        while (pos > 0) {
            int pai = (pos - 1) / 2;
            if (heap[pos].dado.id > heap[pai].dado.id) {
                trocar(heap[pos], heap[pai]);
                pos = pai;
            } else break;
        }
    }

    void descerHeap(EntradaHeap* heap, int tam) {
        int i = 0;
        while (2 * i + 1 < tam) {
            int filhoEsq = 2 * i + 1;
            int filhoDir = 2 * i + 2;
            int maior = filhoEsq;

            if (filhoDir < tam && heap[filhoDir].dado.id > heap[filhoEsq].dado.id)
                maior = filhoDir;

            if (heap[maior].dado.id > heap[i].dado.id) {
                trocar(heap[i], heap[maior]);
                i = maior;
            } else break;
        }
    }

public:
    int gerarParticoesOrdenadas() {
        ifstream entrada(entradaBin, ios::binary);
        if (!entrada.is_open()) {
            cerr << "Erro ao abrir o arquivo de entrada." << endl;
            exit(1);
        }

        Info* buffer = new Info[TAM_RAM];
        int partIndex = 0;

        while (!entrada.eof()) {
            int lidos = 0;
            for (int i = 0; i < TAM_RAM && entrada.read(reinterpret_cast<char*>(&buffer[i]), sizeof(Info)); ++i) {
                lidos++;
            }
            if (lidos == 0) break;

            ordenar(buffer, lidos);
            ofstream partFile("particao_" + to_string(partIndex) + ".dat", ios::binary);
            for (int i = 0; i < lidos; ++i) {
                partFile.write(reinterpret_cast<char*>(&buffer[i]), sizeof(Info));
            }
            partFile.close();
            partIndex++;
        }

        delete[] buffer;
        entrada.close();
        return partIndex;
    }

    void mergeParticoes(int numPart) {
        ifstream* arquivos = new ifstream[numPart];
        EntradaHeap* heap = new EntradaHeap[numPart];
        bool* ativos = new bool[numPart];

        for (int i = 0; i < numPart; ++i) {
            arquivos[i].open("particao_" + to_string(i) + ".dat", ios::binary);
            ativos[i] = true;
            if (!arquivos[i].read(reinterpret_cast<char*>(&heap[i].dado), sizeof(Info))) {
                ativos[i] = false;
            }
            heap[i].origem = i;
        }

        int heapTam = 0;
        for (int i = 0; i < numPart; ++i) {
            if (ativos[i]) {
                heap[heapTam++] = heap[i];
                subirHeap(heap, heapTam - 1);
            }
        }

        ofstream saida(ordenadoBin, ios::binary);
        while (heapTam > 0) {
            EntradaHeap maior = heap[0];
            saida.write(reinterpret_cast<char*>(&maior.dado), sizeof(Info));

            int origem = maior.origem;
            if (arquivos[origem].read(reinterpret_cast<char*>(&heap[0].dado), sizeof(Info))) {
                heap[0].origem = origem;
            } else {
                heap[0] = heap[--heapTam];
            }
            descerHeap(heap, heapTam);
        }

        for (int i = 0; i < numPart; ++i)
            arquivos[i].close();

        delete[] arquivos;
        delete[] heap;
        delete[] ativos;
        saida.close();
    }

    void imprimirCSV() {
        ifstream entrada(ordenadoBin, ios::binary);
        if (!entrada.is_open()) {
            cerr << "Erro ao abrir o arquivo final." << endl;
            return;
        }
        cout << "\nid,nome,equipe,jogos,ano,estacao" << endl;
        Info temp;
        while (entrada.read(reinterpret_cast<char*>(&temp), sizeof(Info))) {
            cout << temp.id << ",\"" << temp.nome << "\",\"" << temp.equipe << "\",\""
                 << temp.jogos << "\"," << temp.ano << "," << temp.estacao << endl;
        }
        entrada.close();
    }

    void visualizarIntervalo(int x, int y) {
        ifstream entrada(ordenadoBin, ios::binary);
        entrada.seekg(0, ios::end);
        int total = entrada.tellg() / sizeof(Info);
        if (x < 0 || x >= total || y < x) {
            cout << "Intervalo invalido." << endl;
            return;
        }
        entrada.seekg(x * sizeof(Info));
        Info temp;
        for (int i = x; i <= y && entrada.read(reinterpret_cast<char*>(&temp), sizeof(Info)); ++i) {
            cout << temp.id << ",\"" << temp.nome << "\",\"" << temp.equipe << "\",\""
                 << temp.jogos << "\"," << temp.ano << "," << temp.estacao << endl;
        }
        entrada.close();
    }

    void alterarRegistro(int pos) {
        fstream arquivo(ordenadoBin, ios::binary | ios::in | ios::out);
        arquivo.seekg(0, ios::end);
        int total = arquivo.tellg() / sizeof(Info);
        if (pos < 0 || pos >= total) {
            cout << "Posicao invalida." << endl;
            return;
        }
        arquivo.seekp(pos * sizeof(Info));
        Info novo;
        novo.id = pos;
        cout << "Nome: "; cin.getline(novo.nome, 100);
        cout << "Equipe: "; cin.getline(novo.equipe, 100);
        cout << "Jogos: "; cin.getline(novo.jogos, 50);
        cout << "Ano: "; cin >> novo.ano; cin.ignore();
        cout << "Estacao: "; cin.getline(novo.estacao, 20);
        arquivo.write(reinterpret_cast<char*>(&novo), sizeof(Info));
        arquivo.close();
    }

    void trocarRegistros(int pos1, int pos2) {
        fstream arquivo(ordenadoBin, ios::binary | ios::in | ios::out);
        Info a, b;
        arquivo.seekg(pos1 * sizeof(Info));
        arquivo.read(reinterpret_cast<char*>(&a), sizeof(Info));
        arquivo.seekg(pos2 * sizeof(Info));
        arquivo.read(reinterpret_cast<char*>(&b), sizeof(Info));
        arquivo.seekp(pos1 * sizeof(Info));
        arquivo.write(reinterpret_cast<char*>(&b), sizeof(Info));
        arquivo.seekp(pos2 * sizeof(Info));
        arquivo.write(reinterpret_cast<char*>(&a), sizeof(Info));
        arquivo.close();
    }

    void inserirNaPosicao(int pos) {
        ifstream entrada(ordenadoBin, ios::binary);
        ofstream temp("temp.dat", ios::binary);
        Info tempInfo;
        int i = 0;
        while (i < pos && entrada.read(reinterpret_cast<char*>(&tempInfo), sizeof(Info))) {
            temp.write(reinterpret_cast<char*>(&tempInfo), sizeof(Info));
            i++;
        }
        Info novo;
        cout << "ID: "; cin >> novo.id; cin.ignore();
        cout << "Nome: "; cin.getline(novo.nome, 100);
        cout << "Equipe: "; cin.getline(novo.equipe, 100);
        cout << "Jogos: "; cin.getline(novo.jogos, 50);
        cout << "Ano: "; cin >> novo.ano; cin.ignore();
        cout << "Estacao: "; cin.getline(novo.estacao, 20);
        temp.write(reinterpret_cast<char*>(&novo), sizeof(Info));
        while (entrada.read(reinterpret_cast<char*>(&tempInfo), sizeof(Info))) {
            temp.write(reinterpret_cast<char*>(&tempInfo), sizeof(Info));
        }
        entrada.close();
        temp.close();
        remove(ordenadoBin.c_str());
        rename("temp.dat", ordenadoBin.c_str());
    }

    friend int main();
};

int main() {
    GerenciadorDados gd;
    int totalPart = gd.gerarParticoesOrdenadas();
    gd.mergeParticoes(totalPart);

    int opcao;
    do {
        cout << "\n========= MENU =========\n";
		cout << "1. Imprimir registros ordenados em ordem decrescente\n";
		cout << "2. Visualizar registros entre duas posicoes\n";
		cout << "3. Alterar registro em uma posicao especifica\n";
		cout << "4. Trocar dois registros de posicao\n";
		cout << "5. Inserir novo registro em uma posicao\n";
		cout << "6. Sair do programa\n";
		cout << "========================\n";
		cout << "Escolha uma opcao: ";
		cin >> opcao;
		cin.ignore();

        if (opcao == 1) gd.imprimirCSV();
        else if (opcao == 2) {
            int x, y; cout << "x: "; cin >> x; cout << "y: "; cin >> y;
            gd.visualizarIntervalo(x, y);
        } else if (opcao == 3) {
            int pos; cout << "Posicao para alterar: "; cin >> pos; cin.ignore();
            gd.alterarRegistro(pos);
        } else if (opcao == 4) {
            int a, b; cout << "Posicoes a trocar: "; cin >> a >> b;
            gd.trocarRegistros(a, b);
        } else if (opcao == 5) {
            int pos; cout << "Posicao para inserir: "; cin >> pos; cin.ignore();
            gd.inserirNaPosicao(pos);
        }
    } while (opcao != 6);
    return 0;
}
