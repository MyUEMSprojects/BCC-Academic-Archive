# Define os nomes dos arquivos de saída
CLIENTE_BINARY = cliente
PORTAL_BINARY = portal
SERVIDOR_BINARY = servidor

# Define os endereços das máquinas
PORTAL_MACHINE = rgm4561@l4m16u
CLIENT1_MACHINE = rgm45611@l4m18u
CLIENT2_MACHINE = rgm45611@l4m14u
SERVER1_MACHINE = rgm45611@l4m17u
SERVER2_MACHINE = rgm45611@l4m20u
SERVER3_MACHINE = rgm45611@l4m22u

# Regra padrão para chamar todas as regras de compilação
all: $(CLIENTE_BINARY) $(PORTAL_BINARY) $(SERVIDOR_BINARY)

# Regra para compilar e mover o binário do cliente
$(CLIENTE_BINARY): cliente.py
	cp $< $(CLIENTE_BINARY)
	chmod +x $(CLIENTE_BINARY)

# Regra para tornar o portal.py executável e mover para a pasta build
$(PORTAL_BINARY): portal.py
	cp $< $(PORTAL_BINARY)
	chmod +x $(PORTAL_BINARY)

# Regra para compilar e mover o binário do servidor
$(SERVIDOR_BINARY): servidor.py
	cp $< $(SERVIDOR_BINARY)
	chmod +x $(SERVIDOR_BINARY)

# Regra para enviar o binário do portal para a máquina do portal
.PHONY: deploy-portal
deploy-portal: $(PORTAL_BINARY)
	scp $(PORTAL_BINARY) $(PORTAL_MACHINE):~/portal

# Regra para enviar o binário do client1 para a máquina do client1
.PHONY: deploy-client1
deploy-client1: $(CLIENTE_BINARY)
	scp $(CLIENTE_BINARY) $(CLIENT1_MACHINE):~/cliente

# Regra para enviar o binário do client1 para a máquina do client1
.PHONY: deploy-client2
deploy-client2: $(CLIENTE_BINARY)
	scp $(CLIENTE_BINARY) $(CLIENT2_MACHINE):~/cliente

# Regra para enviar todos os binários de cliente
.PHONY: deploy-clients
deploy-clients: deploy-client1 deploy-client2

# Regras para enviar os binários do servidor para as máquinas dos servidores
.PHONY: deploy-server1
deploy-server1: $(SERVIDOR_BINARY)
	scp $(SERVIDOR_BINARY) $(SERVER1_MACHINE):~/servidor

.PHONY: deploy-server2
deploy-server2: $(SERVIDOR_BINARY)
	scp $(SERVIDOR_BINARY) $(SERVER2_MACHINE):~/servidor

.PHONY: deploy-server3
deploy-server3: $(SERVIDOR_BINARY)
	scp $(SERVIDOR_BINARY) $(SERVER3_MACHINE):~/servidor

# Regra para enviar todos os binários de servidor
.PHONY: deploy-servers
deploy-servers: deploy-server1 deploy-server2 deploy-server3

# Regra para limpar arquivos gerados
.PHONY: clean
clean:
	rm -f $(CLIENTE_BINARY) $(PORTAL_BINARY) $(SERVIDOR_BINARY)