import socket
import threading
import random
import sys

# Lista dos servidores de processamento
servidores = []

# Verifica se os argumentos foram passados corretamente
if len(sys.argv) < 5 or sys.argv[1] not in ['round-robin', 'aleatorio']:
    print("Uso: ./portal <modo> <ip_portal> <porta_portal> <ip_servidor_1> <porta_servidor_1> ...")
    print("Modos suportados: 'round-robin' e 'aleatorio'")
    sys.exit(1)

modo = sys.argv[1]
portal_ip = sys.argv[2]
portal_porta = int(sys.argv[3])

# Preenche a lista de servidores com os argumentos passados
for i in range(4, len(sys.argv), 2):
    servidor_ip = sys.argv[i]
    servidor_porta = int(sys.argv[i + 1])
    servidores.append((servidor_ip, servidor_porta))

indice_servidor = 0
lock = threading.Lock()

def escolher_servidor():
    global indice_servidor
    with lock:
        if modo == 'round-robin':
            servidor = servidores[indice_servidor]
            indice_servidor = (indice_servidor + 1) % len(servidores)
        elif modo == 'aleatorio':
            servidor = random.choice(servidores)
        return servidor

def lidar_com_cliente(cliente_socket):
    try:
        while True:
            comando = cliente_socket.recv(4096).decode()
            if comando.startswith("S [") and comando.endswith("]"):
                arquivos_str = comando[3:-1]
                arquivos = arquivos_str.split(', ')
                
                respostas = []
                
                for arquivo in arquivos:
                    try:
                        # Ler o conteúdo do arquivo
                        with open(arquivo, 'r') as f:
                            conteudo = f.read()

                        # Escolher servidor
                        servidor_ip, servidor_porta = escolher_servidor()

                        print(f"Enviando arquivo '{arquivo}' para {servidor_ip}:{servidor_porta}")

                        # Enviar arquivo e conteúdo para o servidor escolhido
                        servidor_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                        servidor_socket.connect((servidor_ip, servidor_porta))
                        servidor_socket.send(f"{arquivo} {conteudo}".encode())

                        # Receber resposta do servidor
                        resposta = servidor_socket.recv(4096).decode()
                        respostas.append(f"{resposta}")
                        servidor_socket.close()

                    except Exception as e:
                        print(f"Erro ao enviar arquivo '{arquivo}': {str(e)}")

                # Enviar todas as respostas de volta para o cliente
                cliente_socket.send('\n'.join(respostas).encode())

            elif comando == "L":
                # Implementar listagem de arquivos no diretório
                pass

    except Exception as e:
        print(f"Erro ao lidar com cliente: {str(e)}")

    finally:
        cliente_socket.close()

def main():
    portal_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    portal_socket.bind((portal_ip, portal_porta))
    portal_socket.listen(5)

    while True:
        cliente_socket, cliente_endereco = portal_socket.accept()
        thread = threading.Thread(target=lidar_com_cliente, args=(cliente_socket,))
        thread.start()

if __name__ == "__main__":
    main()

