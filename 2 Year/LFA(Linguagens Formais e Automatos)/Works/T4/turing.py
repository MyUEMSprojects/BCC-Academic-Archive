# implementação da máquina de turing
# entrada: estados, alfabeto, transicoes, estado_inicial, estados_de_aceitação 
# entrega: 25/11/2024
# Nome: Felipe Echeverria Vilhalva RGM: 45611


class TuringMachine:
    def __init__(self, estados=None, alfabeto=None, transicoes=None, estado_inicial=None, estados_de_aceitacao=None):
        self.estados = estados
        self.alfabeto = alfabeto if alfabeto is not None else []
        self.transicoes = transicoes if transicoes is not None else {}
        self.estado_inicial = estado_inicial
        self.estados_de_aceitacao = estados_de_aceitacao
        self.pilha = []  # Pilha para os símbolos da cabeça
        self.indice_atual_pilha = 0
        self.tamanho_pilha = 0
        self.estado_atual = self.estado_inicial
        self.simbolo_atual = None
        self.palavra = None
    
    def main(self):
        self.ler_entradas_usuario()
        opcao = 'SIM'
        while opcao != 'NAO':
            print("\n")
            self.resetar()
            self.palavra = list(input("Informe a palavra a ser verificada: "))  # Solicita a palavra para verificação
            self.coloca_palavra_pilha()

            print('\nFita da maquina de turing: ')
            resposta = self.ler_fita()

            if resposta:
                print(f"A maquina de turing aceitou a palavra {''.join(self.palavra)}")
            else:
                print(f"A maquina de turing rejeitou a palavra {''.join(self.palavra)}")
            
            opcao = input('Deseja realizar mais testes a mesma MT(SIM OU NAO)?')
        
    
    def getPalavra(self):
        return self.palavra

    def ler_entradas_usuario(self):
        self.estados = input("Informe os estados (separados por vírgula): ").split(",")
        self.alfabeto = input("Informe o alfabeto (separados por vírgula): ").split(",")
        self.transicoes = {}

        print("Informe as transições no formato 'prox_estado,novo_simbolo,direcao' (ex: 'q1,1,D'). Use '_' para manter o mesmo símbolo:")
        for estado in self.estados:
            for simbolo in self.alfabeto:
                entrada = input(f"D({estado},{simbolo}): ").strip()
                if entrada:  # Apenas adiciona a transição se houver entrada
                    try:
                        prox_estado, novo_simbolo, direcao = entrada.split(',')
                        self.transicoes[(estado, simbolo)] = (prox_estado, novo_simbolo, direcao)
                    except ValueError:
                        print(f"Erro: Transição inválida para D({estado},{simbolo}). Ignorada.")
                else:
                    # Transições vazias não são registradas
                    continue

        self.estado_inicial = input("Informe o estado inicial: ").strip()
        self.estados_de_aceitacao = input("Informe o(s) estado(s) de aceitação (separados por vírgula): ").split(",")

    def resetar(self):
        self.pilha = []  # Pilha para os símbolos da cabeça
        self.indice_atual_pilha = 0
        self.tamanho_pilha = 0
        self.estado_atual = self.estado_inicial
        self.simbolo_atual = None
        self.palavra = None
    
    def coloca_palavra_pilha(self):
        for letra in self.palavra:
            self.pilha.append(letra)
        self.pilha.append('L|') # 'L|' simboliza o fim da fita
    
    def mover_direita(self):
            if self.indice_atual_pilha + 1 < len(self.pilha):
                self.pilha[self.indice_atual_pilha] = self.simbolo_atual
                self.indice_atual_pilha = self.indice_atual_pilha + 1 # vai para direita do array
                self.simbolo_atual = self.pilha[self.indice_atual_pilha] # proximo simbolo da palavra

    def mover_esquerda(self):
            if self.indice_atual_pilha - 1 >= 0:
                self.pilha[self.indice_atual_pilha] = self.simbolo_atual
                self.indice_atual_pilha = self.indice_atual_pilha - 1 # vai para esquerda do array
                self.simbolo_atual = self.pilha[self.indice_atual_pilha] # proximo simbolo da palavra

    def ler_fita(self) -> bool:
        # Lê o primeiro símbolo antes do loop
        self.simbolo_atual = self.pilha[0]
        self.estado_atual = self.estado_inicial

        while True:
                
                if self.indice_atual_pilha < 0 or self.indice_atual_pilha >= len(self.pilha):
                    return False # Máquina rejeita a palavra

                # Verifica se o estado atual é de aceitação
                if self.estado_atual in self.estados_de_aceitacao:
                    # Máquina aceitou a entrada!
                    return True

                # Verifica se há transição válida
                if (self.estado_atual, self.simbolo_atual) not in self.transicoes:
                    # Máquina rejeitou a entrada! Não há transição válida.
                    return False

                # Executa a transição
                prox_estado, novo_simbolo, direcao = self.transicoes[(self.estado_atual, self.simbolo_atual)]
                print(f"Transição: ({self.estado_atual}, {self.simbolo_atual}) => {prox_estado}, {novo_simbolo}, {direcao}")
                print(f"{self}")
                print()
            
                self.estado_atual = prox_estado # seta o estado atual como o proximo estado
                if novo_simbolo != '_':
                   self.simbolo_atual = novo_simbolo
            
                if direcao == 'D':
                    self.mover_direita()
                elif direcao == 'E':
                    self.mover_esquerda()

    def __str__(self):
        return f"Simbolo atual: {self.simbolo_atual}, Pilha: {"".join(map(str, self.pilha))}"

turing = TuringMachine()
turing.main()