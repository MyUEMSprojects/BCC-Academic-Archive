#include <bits/stdc++.h>
using namespace std;

int tempo = 0;

// busca em profundidade

struct Vertice
{
  int value;
  char color;
  list<int> neighbors;
  int d;
  int pi;
  int f;
  Vertice(): value(), color('B'), pi(-1) {}
};

void dfs_visit(vector<Vertice> G, int u)
{
  tempo = tempo + 1;
  G[u].d = tempo;
  G[u].color = 'C';
  for(int v: G[u].neighbors)
  {
    if(G[v].color == 'B')
    {
      G[v].pi = u;
      dfs_visit(G, v);
    }
  }
  G[u].color = 'P';
  cout << "Visitando: " << G[u].value << endl;
  tempo = tempo + 1;
  G[u].f = tempo;
}

void dfs(vector<Vertice> G)
{
  tempo = 0;
  for(auto& u: G)
  {
    if(u.color == 'B')
    {
      dfs_visit(G, u.value);
    }
  }
}

int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n,x, y;
  cin >> n;
  vector<Vertice> G(n);

  while(cin >> x >> y)
  {
    G[x].value = x;
    G[x].neighbors.push_back(y);
  }

  dfs(G);
}