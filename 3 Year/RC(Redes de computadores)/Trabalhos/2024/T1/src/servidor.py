#!/usr/bin/env python3

import socket
import sys
import os
import subprocess
import tempfile

class Servidor:
    def __init__(self, ip, port) -> None:
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.ip = ip
        self.port = port
    
    # inicializa o servidor e começa a escutar por conexões do portal
    def connect(self):
        try:
            self.server_socket.bind((self.ip, int(self.port)))
            self.server_socket.listen(5)
        except socket.error as e:
            print(e)
            sys.exit(1)
        finally:
            #print(f'Servidor {self.ip}:{self.port}')
            pass
    
    # cria um arquivo temporário com o conteúdo recebido.
    def create_temp_file(self, file_content):
        try:
            temp_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
            temp_file.write(file_content.encode())
            temp_file.close()
            return temp_file.name
        except Exception as e:
            print(f"Error creating temporary file: {e}")
            return None
    
    # compila o arquivo .c recebido.
    def compile_file(self, file_to_compile):
        executable = file_to_compile.replace(".c", "")
        compile_command = f"gcc {file_to_compile} -o {executable}"
        try:
            subprocess.run(compile_command, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            return executable
        except subprocess.CalledProcessError as e:
            print(f"Compilation error: {e.stderr.decode()}")
            return None
    
    # executa o arquivo compilado
    def run_executable(self, executable):
        try:
            result = subprocess.run(executable, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            return result.stdout.decode()
        except subprocess.CalledProcessError as e:
            print(f"Runtime error: {e.stderr.decode()}")
            return None
    
    # lida com a comunicação entre o portal e o servidor, incluindo recepção, compilação, execução e envio de resposta.
    def handle_client(self, client_socket):
        try:
            while True:
                data = client_socket.recv(4096).decode()
                if not data:
                   break
                #print(f"Received data from portal: {data}") # debug
                lines = data.split("\r\n")
                file_content = "\n".join(lines[1:-2])
            
                temp_file = self.create_temp_file(file_content)
                if not temp_file:
                    client_socket.sendall(b"Error creating temporary file")
                    return
            
                executable = self.compile_file(temp_file)
                if not executable:
                    client_socket.sendall(b"Error compiling file")
                    os.remove(temp_file)
                    return
            
                result = self.run_executable(executable)
                if result is None:
                    client_socket.sendall(b"Error running executable")
                else:
                    client_socket.sendall(result.encode())
            
                os.remove(temp_file)
                os.remove(executable)
        except Exception as e:
            print(f"Error handling client: {e}")
            client_socket.sendall(b"Internal server error")
        finally:
            client_socket.close()
    
    # mantém o servidor em execução contínua, aceitando novas conexões do portal e delegando o processamento para processos filhos.
    def run(self):
        self.connect()
        while True:
            client_socket, addr = self.server_socket.accept()
            pid = os.fork()
            if pid == 0:
                self.server_socket.close()
                self.handle_client(client_socket)
                os._exit(0)
            else:
                client_socket.close()   

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print('servidor <ip/nome_maquina> <porta>')
        sys.exit(1)
        
    ip = sys.argv[1]
    port = sys.argv[2]
    servidor = Servidor(ip, port)
    servidor.run()