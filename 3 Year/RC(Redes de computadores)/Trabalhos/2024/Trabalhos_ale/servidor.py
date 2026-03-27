import socket
import subprocess
import threading
import sys
import os

def compilar_e_executar(arquivo, conteudo):
    # Escrever conteúdo no arquivo
    with open(arquivo, 'w') as f:
        f.write(conteudo)

    # Verificar se o arquivo contém a função main
    with open(arquivo, 'r') as f:
        if 'int main' not in f.read():
            return "Erro: o arquivo não contém a função main.\n"

    try:
        # Compilar o arquivo
        compilacao = subprocess.run(['gcc', arquivo, '-o', 'temp'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if compilacao.returncode != 0:
            return f"Erro de compilação:\n{compilacao.stderr.decode()}"
        
        # Executar o arquivo compilado
        execucao = subprocess.run(['./temp'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        resultado = execucao.stdout.decode() + execucao.stderr.decode()
    except Exception as e:
        resultado = f"Erro durante a execução: {str(e)}"
    
    # Remover arquivo temporário
    if os.path.exists('temp'):
        os.remove('temp')
    
    return resultado

def lidar_com_conexao(conexao):
    try:
        mensagem = conexao.recv(4096).decode()
        partes = mensagem.split(' ', 1)
        arquivo = partes[0]
        conteudo = partes[1] if len(partes) > 1 else ""

        resultado = compilar_e_executar(arquivo, conteudo)
        conexao.send(resultado.encode())
    except Exception as e:
        print(f"Erro ao lidar com conexão: {str(e)}")
    finally:
        conexao.close()

def main():
    if len(sys.argv) < 3:
        print("Uso: python3 servidor.py <IP> <porta>")
        return

    servidor_ip = sys.argv[1]
    porta = int(sys.argv[2])

    servidor_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    servidor_socket.bind((servidor_ip, porta))
    servidor_socket.listen(5)

    while True:
        conexao, endereco = servidor_socket.accept()
        thread = threading.Thread(target=lidar_com_conexao, args=(conexao,))
        thread.start()

if __name__ == "__main__":
    main()

