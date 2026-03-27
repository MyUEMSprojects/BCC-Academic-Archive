import networkx as nx
import matplotlib.pyplot as plt


# Criar um grafo vazios
G = nx.Graph()

# Adicionar nós
G.add_node(1)
G.add_nodes_from([2, 3, 4])

# Adicionar arestas
G.add_edge(1, 2)
G.add_edges_from([(2, 3), (3, 4), (4, 1)])

# Exibir as informações básicas do grafo
print("Nós:", G.nodes())
print("Arestas:", G.edges())

nx.draw(G, with_labels=True, node_color='lightblue', node_size=700, font_size=14)
plt.show()