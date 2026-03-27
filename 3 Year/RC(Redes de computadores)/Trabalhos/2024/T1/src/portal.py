#!/usr/bin/env python3

import socket
import sys
import os

class Portal:
    def __init__(self, ip, port, servers_addrs) -> None:
        self.ip = ip
        self.port = port    
        self.portal_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.servers_addrs = servers_addrs
        self.current_server = 0
    
    # inicializa o portal e começa a escutar por conexões de clientes.
    def connect(self):
        try:
            self.portal_socket.bind((self.ip, int(self.port)))
            self.portal_socket.listen(5)
            #print(f'Portal listening on {self.ip}:{self.port}') # debug
        except socket.error as e:
            print(f"Connection error: {e}")
            sys.exit(1)
    
    # lida com a comunicação entre o cliente e o portal, e entre o portal e os servidores de processamento.
    def handle_client(self, client_socket):
        try:
            while True:
                data = client_socket.recv(4096)
                if not data:
                   break
                #print(f"Received data from client: {data.decode()}") # debug
            
                # escolhendo o servidor de processamento(Round-Robin)
                server_ip, server_port = self.servers_addrs[self.current_server]
                self.current_server = (self.current_server + 1) % len(self.servers_addrs)
            
                ack_received = False
                retries = 3  # número de tentativas
                while not ack_received and retries > 0:
                    try:
                        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
                            server_socket.connect((server_ip, server_port))
                            server_socket.sendall(data)
                        
                            response = server_socket.recv(4096)
                            #print(f"Received response from server: {response.decode()}") # debug
                            client_socket.sendall(response)
                            ack_received = True  # se receber uma resposta, assume que foi bem-sucedido
                    except socket.error as e:
                        print(f"Error handling client: {e}")
                        retries -= 1
                        if retries == 0:
                            client_socket.sendall(b"Failed to communicate with server after 3 retries") # debug
        except socket.error as e:
            print(f"Error handling client: {e}")
        finally:
            client_socket.close()
    
    # mantém o portal em execução contínua, aceitando novas conexões de clientes e delegando o processamento para processos filhos.
    def run(self):
        self.connect()
        while True:
            client_socket, addr = self.portal_socket.accept()
            pid = os.fork()
            if pid == 0:
                self.portal_socket.close() # fecha o socket do portal no processo filho
                self.handle_client(client_socket)
                sys.exit(0) 
            else:
                client_socket.close()

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print('portal <ip/nome_maquina> <porta>')
        sys.exit(1)
    
    # servers de processamento com ips e portas()
    servers_addrs = [("127.0.0.1", 9001), ("127.0.0.1", 9002), ("127.0.0.1", 9003)] # localhost debug
    
    ip = sys.argv[1]
    port = sys.argv[2]
    portal = Portal(ip, port, servers_addrs)
    portal.run()