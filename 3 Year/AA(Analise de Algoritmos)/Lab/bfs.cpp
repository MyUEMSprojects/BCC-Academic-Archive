#include <bits/stdc++.h>
using namespace std;

//using int = long long int;
//using si = short int;

struct Vertice
{
  int value;
  char color;
  list<int> neighbors;
  int d;
  int pi;
  Vertice(): value(), color('B'), d(INT_MAX), pi(-1) {}
};

void bfs(vector<Vertice> G, int s)
{
  G[s].color = 'C';
  G[s].d = 0;

  queue<int> Q;
  Q.push(s);
  while(!Q.empty())
  {
    int u_i = Q.front();
    Q.pop();

    for(int v: G[u_i].neighbors)
    {
      if(G[v].color == 'B'){
        G[v].color = 'C';
        G[v].d = G[u_i].d + 1;
        G[v].pi = u_i;
        Q.push(v);
      }
    }

    cout << "Visitando: " << G[u_i].value << endl;

    G[u_i].color = 'P';
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

  bfs(G, 1);

  /*
  for(auto& j: G){
    cout << j.value << " neighbors: ";
    for(auto&k: j.neighbors)
    {
      cout << k << " ";
    }
    cout << "-> ";
  }
  */
}