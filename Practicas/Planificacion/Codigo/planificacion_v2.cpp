#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <sstream>
using namespace std;

typedef vector<vector<string> > Grafo;
struct Nodo{
  string nombre;
  int peso;
};
Grafo grafo;
vector<vector<int> > dependencias;
vector<Nodo> nodos;
vector<bool> visitados;
vector<int> caminos;
vector<bool> aparicionCaminoCritico;
vector<int> caminoCritico;
map<string,int> indices;

void leerGrafo(){
  int n;
  cin >> n;
  //n+2 porque guardamos las tareas START y END también.
  grafo = Grafo(n+2);
  dependencias = vector<vector<int> >(n);
  nodos = vector<Nodo>(n+1);
  visitados = vector<bool>(n,false);
  aparicionCaminoCritico = vector<bool>(n,false);
  caminoCritico = vector<int>(n,-1);
  //n+1 porque entra el camino hacia el END también.
  caminos = vector<int>(n+1,0);
  for(int i = 0; i < n; ++i){
    string s;
    int c;
    Nodo nodo;
    cin >> s >> c;
    nodo.nombre = s;
    nodo.peso = c;
    indices[s] = i;
    nodos[i] = nodo;
    cin >> s;
    while(s != "@"){
      grafo[i].push_back(s);
      cin >> s;
    }
  }
  indices["END"] = n;
  nodos[n].nombre = "END";
}

void escribirIndices(){
  map<string,int>::const_iterator it;
  for(it = indices.begin(); it != indices.end(); ++it){
    cout << it->first << ": " << it->second << endl;
  }
}

void escribirGrafo(){
  for(int i = 0; i < grafo.size(); ++i){
    cout << "i = " << i << ", nodos = ";
    for(int j = 0; j < grafo[i].size(); ++j){
      cout << grafo[i][j] << ",";
    }
    cout << endl;
  }
}

void guardarDependencias(){
  int n = grafo.size();
  for(int i = 0; i < n-2; ++i){
    int tam = grafo[i].size();
    for(int j = 0; j < tam; ++j){
      int indice = indices[grafo[i][j]];
      dependencias[indice].push_back(i);
    }
  }
  n = dependencias.size();
  for(int i = 0; i < n; ++i){
    if(dependencias[i].empty()) grafo[n+1].push_back(nodos[i].nombre);
    if(grafo[i].empty()){
      grafo[i].push_back("END");
      grafo[n].push_back(nodos[i].nombre);
    }
  }
}

void escribirDependencias(){
  for(int i = 0; i < dependencias.size(); ++i){
    cout << "i = " << i << ", dependencias = ";
    for(int j = 0; j < dependencias[i].size(); ++j){
      cout << dependencias[i][j] << ",";
    }
    cout << endl;
  }
}

void bfsCaminos(int nodo){
  cout << "Entra bfsCaminos con nodo " << nodo << endl;
  //visitados[nodo] = true;
  queue<int> q;
  q.push(nodo);
  int end = grafo.size()-2;
  cout << "end = " << end << endl;
  while(!q.empty()){
    int nodoActual = q.front();
    q.pop();
    if(nodoActual != end){
      cout << "nodoActual = " << nodoActual << ", hijos = ";
      int tam = grafo[nodoActual].size();
      //Hijos del nodo.
      for(int i = 0; i < tam; ++i){
	int indice = indices[grafo[nodoActual][i]];
	//if(!visitados[indice]){
	  //visitados[indice] = true;
	  q.push(indice);
	  int camino = caminos[nodoActual] + nodos[nodoActual].peso;
	  if(caminos[indice] < camino){
	    aparicionCaminoCritico[nodoActual] = true;
	    //if(caminoCritico[indice] != -1) aparicionCaminoCritico[caminoCritico[indice]] = false;
	    caminoCritico[indice] = nodoActual;
	    caminos[indice] = camino;
	  }
	  else if(caminos[indice] == camino) aparicionCaminoCritico[nodoActual] = true;
	  cout << "(" << indice << "," << caminos[indice] << "), ";
	//}
      }
    }
    cout << endl;
  }
}

void calcularCaminos(){
  int start = grafo.size()-1;
  int n = grafo[start].size();
  cout << "start = " << start << ", n = " << n << endl;
  for(int i = 0; i < n; ++i){
    int indice = indices[grafo[start][i]];
    bfsCaminos(indice);
  }
  int end = grafo.size()-2;
  n = grafo[end].size();
  int max = 0;
  for(int i = 0; i < n; ++i){
    int indice = indices[grafo[end][i]];
    int camino = caminos[indice] + nodos[indice].peso;
    if(camino > max) max = camino;
  }
  caminos[end] = max;
}

void escribirEspacios(int tam){
  int n = 6 - tam;
  for(int i = 0; i < n; ++i){
    cout << " ";
  }
}

int calcularLS(int nodo){
  //cout << "ENTRA!!" << endl;
  queue<int> q;
  q.push(nodo);
  int end = grafo.size()-2;
  vector<bool> vis(grafo.size()-2,false);
  vis[nodo] = true;
  vector<int> distancias(grafo.size()-2,0);
  while(!q.empty()){
    int nodoActual = q.front();
    q.pop();
    //cout << "nodoActual = " << nodoActual;
    if(nodoActual != end){
      int tam = grafo[nodoActual].size();
      //Hijos del nodo.
      for(int i = 0; i < tam; ++i){
	int indice = indices[grafo[nodoActual][i]];
	//cout << "indice = " << indice;
	if(!vis[indice]){
	  vis[indice] = true;
	  q.push(indice);
	  int camino = distancias[nodoActual] + nodos[nodoActual].peso;
	  //cout << ", camino = " << camino << ", caminos[] = " << caminos[indice] << endl;
	  if(caminos[indice] > camino) return (caminos[indice] - camino);
	  distancias[indice] = camino;
	}
      }
    }
  }
  return -1;
}

void escribirNodo(int nodo){
  int ES, EF, LS, LF;
  string cc;
  if(dependencias[nodo].empty()){
	ES = LS = 0;
	EF = LF = nodos[nodo].peso;
	cc = "*";
	if(!aparicionCaminoCritico[nodo]){
	  int aux = calcularLS(nodo);
	  if(aux != -1){
	    LS = aux;
	    LF = LS + nodos[nodo].peso;
	    cc = " ";
	  }
	}
  }
  else{
    int tam = dependencias[nodo].size();
    int caminoMax = caminos[nodo];
    ES = caminoMax;
    EF = caminoMax + nodos[nodo].peso;
    tam = grafo[nodo].size();
    int caminoDesdeNodo = caminoMax + nodos[nodo].peso;
    int caminoMaxTotal = caminoDesdeNodo;
    //cout << "nodo_" << nodo << ", caminoDesdeNodo = " << caminoMaxTotal << endl;
    //cout << "caminos dependencias = ";
    for(int j = 0; j < tam; ++j){
      int indice = indices[grafo[nodo][j]];
      int camino = caminos[indice];
      //cout << "(" << indice << "," << camino << "),";
      if(caminoMaxTotal < camino) caminoMaxTotal = camino;
    }
    //cout << endl << "caminoMaxTotal = " << caminoMaxTotal << endl;
    if(caminoMaxTotal == caminoDesdeNodo){
      LS = ES;
      LF = EF;
      cc = "*";
    }
    else{
      LS = caminoMaxTotal - nodos[nodo].peso;
      LF = LS + nodos[nodo].peso;
      cc = " ";
    }
  }
  cout << nodos[nodo].nombre;
  escribirEspacios(nodos[nodo].nombre.size());
  ostringstream s1;
  s1 << ES;
  cout << s1.str();
  escribirEspacios(s1.str().size());
  ostringstream s2;
  s2 << EF;
  cout << s2.str();
  escribirEspacios(s2.str().size());
  ostringstream s3;
  s3 << LS;
  cout << s3.str();
  escribirEspacios(s3.str().size());
  ostringstream s4;
  s4 << LF;
  cout << s4.str();
  escribirEspacios(s4.str().size());
  /*cout << s.str();
  escribirEspacios(s.str().size());
  s << EF;
  cout << s.str();
  escribirEspacios(s.str().size());
  s << LS;
  cout << s.str();
  escribirEspacios(s.str().size());
  s << LF;
  cout << s.str();
  escribirEspacios(s.str().size());*/
  cout << cc << endl;
}

void planificacion(){
  int n = grafo.size();
  cout << "START 0     0     0     0     *" << endl; 
  for(int i = 0; i < n-2; ++i){
    if(!visitados[i]){
      visitados[i] = true;
      //Mirar si depende de alguien.
      escribirNodo(i);
    }
      int tam = grafo[i].size();
      for(int j = 0; j < tam; ++j){
	int indice = indices[grafo[i][j]];
	if(indice != n-2 and !visitados[indice]){
	  visitados[indice] = true;
	  escribirNodo(indice);
	}
      }
    }
    cout << "END   " << caminos[n-2];
    ostringstream s;
    s << caminos[n-2];
    escribirEspacios(s.str().size());
    cout << s.str();
    escribirEspacios(s.str().size());
    cout << s.str();
    escribirEspacios(s.str().size());
    cout << s.str();
    escribirEspacios(s.str().size());
    cout << "*" << endl;
}

void escribirCaminos(){
  for(int i = 0; i < caminos.size(); ++i){
    cout << "i = " << i << ", camino = " << caminos[i] << endl;
  }
}
int main(){
  leerGrafo();
  /*escribirGrafo();
  cout << "indices:" << endl;
  escribirIndices();
  guardarDependencias();
  escribirDependencias();*/
  guardarDependencias();
  //escribirGrafo();
  calcularCaminos();
  planificacion();
  //escribirCaminos();
}