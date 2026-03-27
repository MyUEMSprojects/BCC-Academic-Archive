import socket
import os
import sys

def enviar_arquivos_portal(arquivos, portal_ip, portal_porta):
    try:
        cliente_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        cliente_socket.connect((portal_ip, portal_porta))
        
        arquivos_str = ', '.join(arquivos)
        comando = f"S [{arquivos_str}]"
        cliente_socket.send(comando.encode())
        
        resposta = cliente_socket.recv(4096).decode()
        
        # Processando a resposta para exibir apenas o conteúdo sem o nome do arquivo
        respostas = resposta.splitlines()
        for resposta in respostas:
            if resposta.startswith("[") and "]: " in resposta:
                index = resposta.index("]: ") + len("]: ")
                print(resposta[index:])
            else:
                print(resposta)
        
    except Exception as e:
        print(f"Erro ao enviar arquivos para o portal: {str(e)}")

    finally:
        cliente_socket.close()

def listar_arquivos():
    files = [f for f in os.listdir('.') if f.endswith('.c')]
    if files:
        for file in files:
            print(f"{file}")
    else:
        print("L 0")

def main():
    if len(sys.argv) != 3:
        print("Uso: python3 cliente.py <portal_ip> <portal_porta>")
        return
    
    portal_ip = sys.argv[1]
    portal_porta = int(sys.argv[2])

    while True:
        comando = input()

        if comando.startswith("S [") and comando.endswith("]"):
            arquivos = comando[3:-1].split(', ')
            enviar_arquivos_portal(arquivos, portal_ip, portal_porta)
        elif comando == "L":
            listar_arquivos()
        else:
            print("Comando inválido. Tente novamente.")

if __name__ == "__main__":
    main()

