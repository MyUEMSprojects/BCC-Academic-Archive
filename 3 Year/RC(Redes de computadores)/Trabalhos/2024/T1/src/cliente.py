#!/usr/bin/env python3

import socket
import sys
import os

class Cliente:
    def __init__(self, ip, port) -> None:
        self.cliente_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.ip = ip
        self.port = port
        
    # estabelece a conexão com o portal
    def connect(self):
        try:
            self.cliente_socket.connect((self.ip, int(self.port)))
            #print(f'Cliente {self.ip}:{self.port}')
        except socket.error as e:
            print(f"Connection error: {e}")
            sys.exit(1)
    
    # envia os protocolos formatados para o portal e recebe a resposta
    def send(self, protocols):
        for protocol in protocols:
            ack_received = False
            retries = 3  # número de tentativas
            while not ack_received and retries > 0:
                try:
                   self.cliente_socket.sendall(protocol.encode())
                   response = self.cliente_socket.recv(4096).decode()
                   print(response)
                   ack_received = True  # se receber uma resposta, assume que foi bem-sucedido
                except socket.error as e:
                   print(f"Connection error while sending protocol: {e}")
                   retries -= 1
                   if retries == 0:
                      print("Failed to send protocol after 3 retries")
                      sys.exit(1)
    
    # le o arquivo
    def read_file(self, file_name):
        try:
            with open(file_name, 'r') as file:
                return file.read()
        except FileNotFoundError:
            print(f"File {file_name} not found")
            return ""
    
    # formata o protocolo com o conteúdo do arquivo.
    def create_protocol(self, metodo, file_name):
        file_content = self.read_file(file_name)
        if not file_content:
            return ""
        return f'{metodo} [{file_name}] {self.ip}:{self.port}\r\n{file_content}\r\n\r\n'
    
    # solicita o comando do usuário para enviar arquivos ou listar os arquivos disponíveis.
    def req(self):
        command = input('')
        if command.startswith('S[') and command.endswith(']'):
            files = command[2:-1].split(',')
            protocols = [self.create_protocol('S', file.strip()) for file in files]
            protocols = [protocol for protocol in protocols if protocol] # remove protocolos vazios
            if protocols:
               self.send(protocols)
        elif command == 'L':
            self.list_files()
        else:
            print("Invalid command")
            # pass
    
    # verifica se existe arquivos .c 
    def verify_files(self, dir=None):
        dir = dir or '.'
        return [file for file in os.listdir(dir) if file.endswith('.c')]
    
    # lista os arquivos(se existirem)
    def list_files(self):
        files_c = self.verify_files()
        
        if files_c:
            for file in files_c:
                print(file)
        else:
            print('L 0')
    
    # mantém o cliente em execução contínua, aguardando comandos do usuário
    def run(self):
        try:
            while True:
                self.req()
        except KeyboardInterrupt:
            self.cliente_socket.close()
            sys.exit(0)
            
if __name__ == "__main__":
    if len(sys.argv) < 3:
        print('cliente <ip/nome_maquina> <porta>')
        sys.exit(1)
    
    ip = sys.argv[1]
    port = sys.argv[2]
    
    cliente = Cliente(ip, port)
    cliente.connect()
    cliente.run()