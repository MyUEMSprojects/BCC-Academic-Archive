#include <iostream>
#include <stdlib.h> // Para atoi
#include <unistd.h>
#include <string>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h> // simular erro
#include <limits>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iomanip>

using namespace std;

enum Estado {
    AGUARDANDO_DADOS,
    AGUARDANDO_CONFIRMACAO,
    AGUARDANDO_RESULTADO_PESQUISA
};

struct Pacote {
  Pacote() : id(0), tipo_mensagem(' '), erro(false), tipo_combustivel(0), latitude(0.0), longitude(0.0) {
      _tipo.preco = 0;
  }

  int id;
  char tipo_mensagem;      // 'D' ou 'P' ou 'A' ou 'N' ou 'R'
  bool erro;
  int tipo_combustivel;    // 0-diesel, 1-álcool, 2-gasolina

  union {
    int preco;             // para tipo 'D'.
    int raio_de_busca;     // para tipo 'P'.
  } _tipo;

  double latitude;
  double longitude;
};

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Uso correto: %s <ip_do_servidor> <porta_do_servidor>\n", argv[0]);
        return 1;
    }

    char* ip_servidor = argv[1];
    int porta_servidor = atoi(argv[2]);

    //printf("Tentando enviar para o servidor no IP %s e porta %d\n", ip_servidor, porta_servidor); // debug

    int meu_socket;
    struct sockaddr_in servidor_addr;

    meu_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (meu_socket == -1) {
        perror("Erro ao criar o socket");
        return 1;
    }

    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(porta_servidor);
    servidor_addr.sin_addr.s_addr = inet_addr(ip_servidor);

    srand(time(NULL));

    Estado estado_atual = AGUARDANDO_DADOS;
    Pacote pacote_para_enviar;
    char tipo_ultimo_pacote_enviado; // guardar o tipo D ou P

    // loop da maquina de estados
    while (true) {
        switch (estado_atual) {

            case AGUARDANDO_DADOS: {
                string linha;
                std::getline(std::cin, linha);

                std::stringstream ss(linha);

                string tipo_msg;
                ss >> tipo_msg;
                pacote_para_enviar.tipo_mensagem = tipo_msg[0];

                if(pacote_para_enviar.tipo_mensagem == 'D')
                {
                    ss >> pacote_para_enviar.id >> pacote_para_enviar.tipo_combustivel
                       >> pacote_para_enviar._tipo.preco >> pacote_para_enviar.latitude
                       >> pacote_para_enviar.longitude;

                }else if(pacote_para_enviar.tipo_mensagem == 'P'){
                    ss >> pacote_para_enviar.id >> pacote_para_enviar.tipo_combustivel
                       >> pacote_para_enviar._tipo.raio_de_busca >> pacote_para_enviar.latitude
                       >> pacote_para_enviar.longitude;
                }else {
                    cout << "Mensagem nao reconhecida!" << endl;
                }

                tipo_ultimo_pacote_enviado = pacote_para_enviar.tipo_mensagem;

                // Envia o pacote pela primeira vez
                pacote_para_enviar.erro = ((double)rand() / RAND_MAX) > 0.5;
                //cout << "Enviando pacote ID " << pacote_para_enviar.id << " do tipo '" << tipo_ultimo_pacote_enviado << (pacote_para_enviar.erro ? "' COM erro..." : "' SEM erro...") << endl; // debug
                sendto(meu_socket, &pacote_para_enviar, sizeof(Pacote), 0, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr));

                // Muda para o estado de espera
                estado_atual = AGUARDANDO_CONFIRMACAO;
                break;
            }
            case AGUARDANDO_CONFIRMACAO: {
                //cout << "\n--- ESTADO: Aguardando ACK/NAK ---" << endl; // debug

                struct timeval tv;
                tv.tv_sec = 2; tv.tv_usec = 0;
                setsockopt(meu_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

                Pacote pacote_resposta;
                int len = recvfrom(meu_socket, &pacote_resposta, sizeof(Pacote), 0, NULL, NULL);

                if (len < 0) {
                    cout << "[EVENTO] Timeout! Retransmitindo pacote ID " << pacote_para_enviar.id << endl; // debug
                    pacote_para_enviar.erro = ((double)rand() / RAND_MAX) > 0.5;
                    sendto(meu_socket, &pacote_para_enviar, sizeof(Pacote), 0, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr));

                } else { // Pacote recebido
                    // NAK
                    if (pacote_resposta.tipo_mensagem == 'N' && pacote_resposta.id == pacote_para_enviar.id) {
                        cout << "[EVENTO] NAK recebido. Retransmitindo pacote ID " << pacote_para_enviar.id << endl; // debug
                        pacote_para_enviar.erro = ((double)rand() / RAND_MAX) > 0.5;
                        sendto(meu_socket, &pacote_para_enviar, sizeof(Pacote), 0, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr));
                    }
                    // recebeu ACK
                    else if (pacote_resposta.tipo_mensagem == 'A' && pacote_resposta.id == pacote_para_enviar.id) {
                        cout << "[EVENTO] ACK recebido para o pacote ID " << pacote_para_enviar.id << ". Sucesso na confirmação!" << endl; // debug

                        if (tipo_ultimo_pacote_enviado == 'D') {
                            estado_atual = AGUARDANDO_DADOS;
                        }
                        else if (tipo_ultimo_pacote_enviado == 'P') {
                            estado_atual = AGUARDANDO_RESULTADO_PESQUISA;
                        }
                    }
                    else { // recebeu o NAK
                        //cout << "[EVENTO] Pacote inesperado (tipo '" << pacote_resposta.tipo_mensagem << "') recebido enquanto aguardava ACK/NAK. Ignorando." << endl;
                        // O cliente nao faz nada e continua no mesmo estado, para continuar esperando pelo ACK correto
                    }
                }
                break;
            }
            case AGUARDANDO_RESULTADO_PESQUISA: {
                //cout << "\n--- ESTADO: Aguardando Resultado da Pesquisa ---" << endl; // debug

                struct timeval tv;
                tv.tv_sec = 10; tv.tv_usec = 0; // timeout para pesquisa
                setsockopt(meu_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

                Pacote pacote_resultado;
                int len = recvfrom(meu_socket, &pacote_resultado, sizeof(Pacote), 0, NULL, NULL);

                if (len < 0) {
                    cout << "[ERRO] Timeout esperando o resultado da pesquisa. Voltando ao menu." << endl;
                } else if (pacote_resultado.tipo_mensagem == 'R' && pacote_resultado.id == pacote_para_enviar.id) {
                    cout << "----------- RESULTADO DA PESQUISA -----------" << endl;
                    if (pacote_resultado._tipo.preco == -1) {
                        cout << "Nenhum posto encontrado com os critérios informados." << endl;
                    } else {
                        double preco_real = (double)pacote_resultado._tipo.preco / 1000.0;
                        cout << "Menor preço encontrado: R$ " << fixed << setprecision(4) << preco_real << endl;
                        cout << "Localização: Lat " << pacote_resultado.latitude << ", Lon " << pacote_resultado.longitude << endl;
                    }
                     cout << "------------------------------------------" << endl;
                }

                // transacao terminou
                estado_atual = AGUARDANDO_DADOS;
                break;
            }
        }
    }

    close(meu_socket);
    return 0;
}