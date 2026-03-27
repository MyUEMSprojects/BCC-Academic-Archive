#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iomanip>
#include <pthread.h>
#include <thread>

#define RAIO_TERRA 6371000 // em metros

// tem algumas para comentadas(couts) para a 'debugação'

using namespace std;

class Posto{
public:
  Posto() : preco(0), tipoCombustivel(0), latitude(0.0), longitude(0.0) {} // Definição do construtor

  int preco;
  int tipoCombustivel;
  double latitude;
  double longitude;
};

vector<Posto> postos; // vetor que armazena os postos sem precisar ficar abrindo o arquivo (ponto critico)

#define BUFFER_SIZE 2000

// esse mutex é usado para proteger o acesso a recursos compartilhados (arquivos e vector)
pthread_mutex_t lock;

struct Pacote {
  Pacote() : id(0), tipo_mensagem(' '), erro(false), tipo_combustivel(0), latitude(0.0), longitude(0.0) {
      _tipo.preco = 0;
  }

  int id;
  char tipo_mensagem;      // 'D' ou 'P' ou 'A' ou 'N' para ACK e NAK
  bool erro;               // true ou false
  int tipo_combustivel;    // 0-diesel, 1-álcool, 2-gasolina

  union {
    int preco;             // para tipo 'D'
    int raio_de_busca;     // para tipo 'P'
  } _tipo; // _tipo

  double latitude;
  double longitude;
};

// struct para passar multiplos valores para cada thread
typedef struct {
    int socket;
    struct sockaddr_in cliente_addr;
    Pacote pacote_recebido;
} request_data;

// funcao que será executada por cada thread
void* processa_requisicao(void* arg);

// funcao que le o arquivo e popula o vetor global 'postos'
void carregarPostosDoArquivo();

// funcao pra adicionar os postos no arquivo(D)
void adicionarPostoNoArquivo(const Posto& novoPosto);

// funcao para converter graus para radianos
double paraRadianos(double graus);

// calcula a distancia em metros entre duas coordenadas geograficas
double calcularDistancia(double lat1, double lon1, double lat2, double lon2);

int main(int argc, char *argv[]) {
    int meu_socket;
    struct sockaddr_in servidor_addr;

    if (argc < 2) {
        printf("Uso: %s <porta>\n", argv[0]);
        return 1;
    }

    if (pthread_mutex_init(&lock, NULL) != 0) {
        perror("Falha ao inicializar o mutex");
        return 1;
    }

    carregarPostosDoArquivo();

    // cria socket UDP
    meu_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (meu_socket == -1) {
        perror("Erro ao criar o socket");
        return 1;
    }

    // config do servidor
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_addr.s_addr = INADDR_ANY;
    servidor_addr.sin_port = htons(atoi(argv[1]));

    if (bind(meu_socket, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) < 0) {
        perror("Erro no bind");
        return 1;
    }

    //printf("Servidor com Threads esperando por mensagens na porta %s...\n", argv[1]); // debug

    while (1) {
        request_data* data = (request_data*) malloc(sizeof(request_data));
        if (data == NULL) {
            perror("Falha ao alocar memória para a requisição");
            continue;
        }

        socklen_t tamanho_addr_cliente = sizeof(data->cliente_addr);

        // Recebe uma nova requisição
        int len = recvfrom(meu_socket, &(data->pacote_recebido), sizeof(Pacote), 0, (struct sockaddr *)&data->cliente_addr, &tamanho_addr_cliente);
        if (len > 0) {
            data->socket = meu_socket;
            pthread_t thread_id;
            if (pthread_create(&thread_id, NULL, processa_requisicao, (void*)data) != 0) {
                perror("Falha ao criar o thread");
                free(data);
            }
        } else {
            free(data);
        }
    }

    close(meu_socket);
    pthread_mutex_destroy(&lock);
    return 0;
}

void* processa_requisicao(void* arg) {
    request_data* data = (request_data*)arg;
    Pacote pacote_recebido = data->pacote_recebido;

    cout << ": Pacote ID " << pacote_recebido.id << " recebido de " << inet_ntoa(data->cliente_addr.sin_addr) << endl; // debug
    cout << "  -> Tipo: " << pacote_recebido.tipo_mensagem << ", Erro Simulado: " << (pacote_recebido.erro ? "Sim" : "Não") << endl; // debug

    // verifica se o pacote tem erro
    if (pacote_recebido.erro) {
        // NAK
        Pacote pacote_nak;
        pacote_nak.id = pacote_recebido.id;
        pacote_nak.tipo_mensagem = 'N'; // 'N' para NAK
        cout << ": Detectado erro no pacote. Enviando NAK." << endl; // debug
        sendto(data->socket, &pacote_nak, sizeof(Pacote), 0, (struct sockaddr *)&data->cliente_addr, sizeof(data->cliente_addr));

    } else { // ACK e envia cofirmacao

        Pacote pacote_ack;
        pacote_ack.id = pacote_recebido.id;
        pacote_ack.tipo_mensagem = 'A'; // 'A' para ACK
        cout << "Pacote OK. Enviando ACK." << endl; // debug
        sendto(data->socket, &pacote_ack, sizeof(Pacote), 0, (struct sockaddr *)&data->cliente_addr, sizeof(data->cliente_addr));

        // processa a requisicao
        pthread_mutex_lock(&lock);

        if (pacote_recebido.tipo_mensagem == 'D') {
            Posto novoPosto;
            novoPosto.preco = pacote_recebido._tipo.preco;
            novoPosto.tipoCombustivel = pacote_recebido.tipo_combustivel;
            novoPosto.latitude = pacote_recebido.latitude;
            novoPosto.longitude = pacote_recebido.longitude;

            postos.push_back(novoPosto);
            adicionarPostoNoArquivo(novoPosto);

        } else if (pacote_recebido.tipo_mensagem == 'P') {
            cout << "  -> Iniciando pesquisa de preço..." << endl; // debug
            int menorPreco = -1; // nao encontrado = -1
            Posto melhorPosto;
            bool encontrouPosto = false;

            for (const Posto& posto_atual : postos) {
                if (posto_atual.tipoCombustivel == pacote_recebido.tipo_combustivel) {
                    double distancia = calcularDistancia(pacote_recebido.latitude, pacote_recebido.longitude, posto_atual.latitude, posto_atual.longitude);
                    if (distancia <= pacote_recebido._tipo.raio_de_busca) {
                        if (!encontrouPosto || posto_atual.preco < menorPreco) {
                            menorPreco = posto_atual.preco;
                            melhorPosto = posto_atual;
                            encontrouPosto = true;
                        }
                    }
                }
            }

            // envia o pacote resultado 'R'
            Pacote pacote_resultado_pesquisa;
            pacote_resultado_pesquisa.id = pacote_recebido.id;
            pacote_resultado_pesquisa.tipo_mensagem = 'R';

            if (encontrouPosto) {
                cout << "  -> Posto mais barato encontrado! Preço: " << menorPreco << endl; // debug
                pacote_resultado_pesquisa._tipo.preco = menorPreco;
                pacote_resultado_pesquisa.latitude = melhorPosto.latitude;
                pacote_resultado_pesquisa.longitude = melhorPosto.longitude;
            } else {
                cout << "  -> Nenhum posto encontrado nos critérios." << endl; // debug
                pacote_resultado_pesquisa._tipo.preco = -1;
            }

            sendto(data->socket, &pacote_resultado_pesquisa, sizeof(Pacote), 0, (struct sockaddr *)&data->cliente_addr, sizeof(data->cliente_addr));
            cout << "  -> Pacote de resultado da pesquisa enviado." << endl; // debug
        }

        pthread_mutex_unlock(&lock);
    }

    free(data);
    return NULL;
}

void carregarPostosDoArquivo() {
    ifstream arquivo("postos.txt");

    if (!arquivo.is_open()) {
        //cout << "[AVISO] Arquivo postos.txt não encontrado. Iniciando com base de dados vazia." << endl; // debug
        return;
    }

    string linha;
    Posto posto_temp;

    while (getline(arquivo, linha)) {
        if (linha.empty()) {
            continue;
        }

        stringstream ss(linha);

        if (ss >> posto_temp.tipoCombustivel >> posto_temp.preco >> posto_temp.latitude >> posto_temp.longitude) {
            postos.push_back(posto_temp);
        }
    }

    arquivo.close();
    //cout << "[INFO] " << postos.size() << " postos carregados do arquivo." << endl; // debug
}

void adicionarPostoNoArquivo(const Posto& novoPosto) {
    ofstream arquivo("postos.txt", std::ios::app);

    if (!arquivo.is_open()) {
        perror("Erro ao abrir postos.txt para escrita");
        return;
    }

    arquivo << novoPosto.tipoCombustivel << " "
            << novoPosto.preco << " "
            << novoPosto.latitude << " "
            << novoPosto.longitude << std::endl;

    arquivo.close();
    cout << "  -> Posto também salvo em postos.txt." << endl; // debug
}

double paraRadianos(double graus) {
    return graus * M_PI / 180.0;
}

double calcularDistancia(double lat1, double lon1, double lat2, double lon2) {
    double dLat = paraRadianos(lat2 - lat1);
    double dLon = paraRadianos(lon2 - lon1);

    double radLat1 = paraRadianos(lat1);
    double radLat2 = paraRadianos(lat2);

    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(radLat1) * cos(radLat2) *
               sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return RAIO_TERRA * c;
}