Nome: Felipe Echeverria Vilhalva RGM: 45611

Instruções:

usando o Make(src):

make --> gera os binarios do cliente,portal e servidores

make deploy-portal --> manda o binario do portal para a maquina-portal
make deploy-servers --> manda os binarios dos servidores para as maquinas-processadoras
make deploy-clients --> manda os binarios dos clientes para as maquinas clientes

----------------------------------------------------------------------------------------
para inicializar o servidores(cliente,portal,processamento):

python3 -u <nome.py> <ip> <porta>

exemplos:

python3 -u servidor.py 127.0.0.1 9001
python3 -u portal.py 127.0.0.1 9000
python3 -u cliente.py 127.0.0.1 9000