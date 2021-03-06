#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <sstream>
using namespace std;

/* Peso máximo de una tarea.*/
const int MAX = 1000;
/* Estructura que representa una tarea, que en el programa se identificará como un nodo del grafo. Guarda el nombre de la tarea
 * (que identificará el nodo correspondiente del grafo), el peso o coste de realizarla, y las tareas dependientes de ésta
 * (que en el grafo se representarán como nodos adyacientes).*/
struct Nodo{
  string nombre;
  int peso;
  vector<string> adyacientes;
};
/* Grafo que guarda las diferentes tareas. Cada una de ellas se representa como un nodo que contiene la información descrita 
 * en la estructura 'Nodo'.*/
vector<Nodo> grafo;
/* Guarda los diferentes nodos que dependen del nodo inicial (nodo START).*/
vector<int> dependientesSTART;
/* Guarda los caminos máximos hacia cada nodo del grafo. Un camino es máximo si es el único que existe hacia cierto nodo, o si 
 * es el que suma más peso de todos los caminos que van hacia un mismo nodo (suma de los costes de las tareas).*/
vector<int> caminos;
/* Indica si un nodo se encuentra en algun camino crítico o no.*/
vector<bool> caminoCritico;
/* Indica si un nodo depende de algun otro nodo o no (si una tarea necesita que finalice otra antes para poderse ejecutar).*/
vector<bool> dependencias;
/* Guarda el LS de cada nodo.*/
vector<int> ls;
/* Guarda el indice correspondiente a cada nodo del grafo. En el grafo, los diferentes nodos se representan con la estructura 'nodo', 
 * pero para facilitar la comodidad de acceso a esos nodos, y para poder observar las dependencias entre ellos,
 * se cuenta con la estructura 'indices', que mapea cada nombre de un nodo con su correspondiente índice en el grafo (el grafo se
 * representa con un vector de Nodos, por lo tanto, se guarda en 'indices' el índice de ese vector por cada nodo).*/
map<string,int> indices;
/* Guarda el número de tareas.*/
int nTareas;

/* Lee el grafo de entrada e inicializa todas las estructuras que dependen de datos de entrada (por ejemplo, vectores que necesitan ser
 * redimensionados al conocer el número de tareas, etc).
 * COSTE: O(n+m) -> Leemos el grafo y lo representamos como una lista de adyaciencias (vector de vectores con diferentes tamaños y con 
 * información adicional (por ejemplo: nombre, peso,ect). Además insertamos en el mapa 'indices' un elemento cada vez y por lo tanto, 
 * el último elemento a insertar producirá un coste O(log(n)), que no influye en el coste total de la función ya que O(log(n))) < O(n+m).*/
void leerGrafo(){
  cin >> nTareas;
  grafo = vector<Nodo>(nTareas);
  caminoCritico = vector<bool>(nTareas,false);
  /* nTareas+1 porque guardamos el camino máximo hacia el nodo END también.*/
  caminos = vector<int>(nTareas+1,0);
  /* Inicializamos a falso todos los nodos para luego sólo modificar los que sean verdaderos (dependen de algun nodo).*/
  dependencias = vector<bool>(nTareas,false);
  /* nTareas+1 porque guardamos el LS de END también.*/
  ls = vector<int>(nTareas+1,0);
  for(int i = 0; i < nTareas; ++i){
    string s;
    int p;
    cin >> s >> p;
    /* Introducimos el nodo en el grafo.*/
    grafo[i].nombre = s;
    grafo[i].peso = p;
    /* Guardamos el mapeo correspondiente del nodo en el grafo (el índice en el cual se encuentra en el grafo).*/
    indices[s] = i;
    /* Miramos si el nodo actual tiene nodos adyacientes o no, para poder indicar después (en caso afirmativo), que el nodo END depende de él.*/
    bool tieneAdyacientes = false;
    cin >> s;
    /* Leemos los nodos adyacientes al nodo actual.*/
    while(s != "@"){
      tieneAdyacientes = true;
      grafo[i].adyacientes.push_back(s);
      cin >> s;
    }
    /* Guardamos los nodos de los que depende END.*/
    if(!tieneAdyacientes) grafo[i].adyacientes.push_back("END");
  }
  /* Guardamos el mapeo del nodo END en la estructura 'indices'. Supondremos que el nodo END se mapea en la última posición fictícia del 
   * grafo (una posición más de las que contiene). La representación es simbóloca (el nodo END no se guarda realmente en el grafo),
   * y se utiliza en funciones posteriores cuando se accede a nodos que no tienen tareas dependientes (END depende de ellos), y 
   * se requiere consultar sus nodos adyacientes (el nodo END).*/
  indices["END"] = nTareas;
}

/* Mira si un grafo contiene ciclos o no. Nodo se representa con un entero, que se corresponde con el índice del vector grafo donde se 
 * encuentra ubicado. Esta función se llamará desde cada nodo dependiente de START
 * COSTE: O(n+m) -> Se trata de un DFS que recorre todo el grafo.*/
bool contieneCiclo(int nodo, vector<bool> visitados){
  /* Si el nodo no es el nodo END (el nodo END se representa con el entero nTareas).*/
  if(nodo != nTareas){
    /* Marcamos nodo como visitado y consultamos su nodos adyacientes.*/
    int tam = grafo[nodo].adyacientes.size();
    visitados[nodo] = true;
    for(int i = 0; i < tam; ++i){
      /*Guardamos el índice correspondiente al nodo adyaciente actual.*/
      int indice = indices[grafo[nodo].adyacientes[i]];
      /* Si el nodo ya ha sido visitado anteriormente por otro nodo, significa que existe algun ciclo en el grafo. La idea se basa en el 
       * recorrido en profundidad de un grafo (DFS). Si no existen ciclos, entonces se van recorriendo los hijos de cada nodo en profundidad 
       *(se visita un hijo, de este hijo se visita un hijo suyo, etc), hasta llegar a un nodo que no tiene hijos (que se corresponderá con un
       * nodo del que depende END), i entra en marcha la recursividad. Si por contra, existe algun ciclo, entonces llegará un momento
       * en el que al acceder a uno de los hijos de algun nodo éste ya habrá sido visitado por otro nodo anteriormente al hacer el recorrido.
       * Así pues, el grafo contiene ciclos si el nodo actual que se está consultando en el bucle ha sido visitado, o si el subgrafo que se 
       * define a partir de este nodo contiene ciclos.*/
      if(visitados[indice]) return true;
      else if(contieneCiclo(indice,visitados)) return true;
    }
  }
  return false;
}

/* Indica si el grafo es aciclico o no. Ejecuta, por cada nodo dependiente de START la función contieneCiclo, de manera que se cubran 
 * todas las posibilidades de encontrar un ciclo en el grafo (si sólo ejecutásemos la función desde un sólo nodo podrían existir
 * ciclos en otras areas del grafo que no se cubran en el recorrido (ya que el grafo es dirigido).
 * COSTE: O(tam·(n+m)) = O(n+m) -> Ejecuta la función contieneCiclo de coste O(n+m) por cada nodo dependiente de START. Como tam es constante,
 * se elimina.*/
bool grafoCiclico(){
  int tam = dependientesSTART.size();
  bool ciclo = false;
  for(int i = 0; i < tam and !ciclo; ++i){
    vector<bool> visitados(nTareas+1,false);
    ciclo = contieneCiclo(i,visitados);
  }
  /* Si existe algun ciclo, la función devuelve true (el grafo es cíclico).*/
  return ciclo;
}

/* Guarda los nodos que son dependientes del nodo START y marca en el vector de dependencias aquellos nodos que dependen de otro nodo. 
 * Se guardan sus representaciones enteras (el índice que ocupan en el vector grafo)
 * COSTE: O(n+m) + O(n) = O(n+m) -> recorre todo el grafo, que está representado como una lista de adyacencias (vector de vectores con 
 * diferentes tamaños), y luego recorre el vector dependencias, que tiene como tamaño el número de vértices del grafo.*/ 
void guardarDependientesSTART(){
  /* Marcamos en el vector de dependencias aquellos nodos que dependen de algun otro (recorremos los nodos adyacientes de cada nodo del 
   * grafo y ponemos la posición correspondiente del vector de dependencias a true).*/
  for(int i = 0; i < nTareas; ++i){
    int tam = grafo[i].adyacientes.size();
    for(int j = 0; j < tam; ++j){
      int indice = indices[grafo[i].adyacientes[j]];
      dependencias[indice] = true;
    }
  }
  /* Añadimos en el vector 'dependientesSTART' los nodos que no sean dependientes de ningun otro nodo.*/
  for(int i = 0; i < nTareas; ++i){
    if(!dependencias[i]) dependientesSTART.push_back(i);
  }
}

/* Guarda el camino máximo hacia cada nodo, desde un nodo inicial dado. Se realiza un recorrido en anchura (BFS) que va guardando, 
 * por cada nodo, el camino máximo hacia él consultando el camino actual existente ya hacia el nodo.
 * COSTE: O(n+m) -> Se trata de un BFS que recorre todo el grafo.*/
void bfsCaminosMaximos(int nodo){
  queue<int> q;
  q.push(nodo);
  while(!q.empty()){
    int nodoActual = q.front();
    q.pop();
    /* Visitamos los nodos adyacientes al nodo actual. No controlamos el hecho de que ya hayan sido visitados o no, porque nos interesa
     * que, de las diferentes veces que se visite un mismo nodo, nos quedemos con el camino máximo que se defina.*/
    int tam = grafo[nodoActual].adyacientes.size();
    for(int i = 0; i < tam; ++i){
      /* Guardamos el indice que representa al nodo hijo.*/
      int indice = indices[grafo[nodoActual].adyacientes[i]];
      /* 'coste' es el peso que tiene el nodo actual.*/
      int coste = grafo[nodoActual].peso;
      /* 'camino' es el coste del camino que hay desde el nodoActual hacia el hijo que se está visitando en el bucle.*/
      int camino = caminos[nodoActual] + coste;
      /* Controlamos que el nodo no sea END.*/
      if(indice != nTareas) q.push(indice);
      /* Guardamos el camino mayor encontrado hasta ahora en la posición del vector de caminos correspondiente. Si el camino que habia 
       * guardado hasta ahora hacia el nodo hijo actual tiene menos coste que el camino hasta el nodo actual, más el peso del nodo, 
       * entonces ese es el nuevo camino máximo hacia el nodo hijo.*/
      if(caminos[indice] < camino) caminos[indice] = camino;
    }
  }
}

/* Indica si un nodo pertenece a algun camino crítico o no. Se realiza un recorrido en anchura en el grafo.
 * COSTE. O(n+m) -> Se trata de un BFS que recorre todo el grafo.*/
bool perteneceCaminoCritico(int nodo){
  /* El nodo END pertenece siempre al camino crítico.*/
  if(nodo == nTareas) {
    caminoCritico[nodo] = true;
    return true;
  }
  int tam = grafo[nodo].adyacientes.size();
  /* Visitamos los nodos adyacientes al nodo actual.*/
  for(int i = 0; i < tam; ++i){
    /* Guardamos el índice que representa al hijo actual que se está visitando en el bucle.*/
    int indice = indices[grafo[nodo].adyacientes[i]];
    /*'camino' es el peso del camino que hay desde el nodo actual hacia el hijo que se está visitando en el bucle.*/
    int camino = caminos[nodo] + grafo[nodo].peso;
    /* Si 'camino' se corresponde con el peso del camino guardado para el hijo actual en el vector 'caminos', entonces eso significa
     * que el camino desde el nodo actual hacia el hijo actual es máximo. Esto de todos modos, no significa que el nodo actual se 
     * encuentre en un camino crítico, ya que puede que el camino desde él hacia el hijo actual sea máximo porque sólo tenga ese hijo, 
     * por ejemplo. Así pues, debemos comprobar que el hijo actual pertenece a algun camino crítico para poder afirmar que el nodo actual 
     * pertenece también.*/
    if(caminos[indice] == camino){
      bool cc = perteneceCaminoCritico(indice);
      if(cc){
	caminoCritico[nodo] = true;
	return true;
      }
    }
  }
  /* Si ninguno de los hijos del nodo actual pertenece a algun camino crítico, entonces el nodo actual tampoco 
   * pertenece a ningun camino crítico.*/
  caminoCritico[nodo] = false;
  return false;
}

/* Calculamos los caminos máximos hacia cada nodo del grafo. Ejecutamos, por cada nodo dependiente de START, la función 'bfsCaminosMaximos',
 * para poder cubrir todo el grafo (ya que se trata de un grafo dirigido, y debemos ejecutar la función en cada nodo dependiente de START
 * para poder cubrir todos los nodos del grafo).
 * COSTE: O(tam·(n+m)) = O(n+m) -> Ejecuta la función bfsCaminosMaximos de coste O(n+m) por cada nodo dependiente de START. Como tam es constante,
 * se elimina.*/
void calcularCaminosMaximos(){
  int tam = dependientesSTART.size();
  for(int i = 0; i < tam; ++i){
   bfsCaminosMaximos(dependientesSTART[i]); 
  }
}

/* Indicamos los nodos del grafo que pertenecen a algun camino crítico. Ejecutamos, por cada nodo dependiente de START, la función 
 * 'perteneceCaminoCritico', para poder cubrir todo el grafo (ya que se trata de un grafo dirigido, y debemos ejecutar la función 
 * en cada nodo dependiente de START para poder cubrir todos los nodos del grafo).
 * COSTE: O(tam·(n+m)) = O(n+m) -> Ejecuta la función caminoCritico de coste O(n+m) por cada nodo dependiente de START. Como tam es constante,
 * se elimina.*/
void calcularcaminosCriticos(){
  int tam = dependientesSTART.size();
  for(int i = 0; i < tam; ++i){
    int nodo = dependientesSTART[i];
    caminoCritico[nodo] = perteneceCaminoCritico(nodo); 
  }
}

/* Calcula el LS de cada nodo empezando desde un nodo inicial. Por cada nodo, su LS es igual al LS mínimo de sus hijos, menos el peso 
 * del nodo. El LS mínimo indica el nodo hijo que debe empezar a ejecutarse antes y, por lo tanto, el que requiere que el nodo actual 
 * se ejecute antes.
 * COSTE: O(n+m) -> Se recorre todo el grafo.*/
int calcularLSNodo(int nodo){
  int LS;
  /* El LS del nodo END es igual al camino máximo hacia él.*/
  if(nodo == nTareas){
    LS = caminos[nodo];
    ls[nodo] = LS;
    return LS;
  }
  int tam = grafo[nodo].adyacientes.size();
  /* lsMin representa el LS mínimo de los hijos del nodo actual.*/
  int lsMin = MAX*nTareas;
  for(int i = 0; i < tam; ++i){
    int indice = indices[grafo[nodo].adyacientes[i]];
    /* Calculamos el LS del hijo actual.*/
    int lsHijo = calcularLSNodo(indice);
    if(lsHijo < lsMin) lsMin = lsHijo;
  }
  /* El LS del nodo actual es igual al LS mínimo encontrado menos su peso, ya que eso define el instante más temprano en el que se puede ejecutar
   * (no podría ejecutarse más tarde porque sino el hijo que tiene el LS mínimo (empieza antes a ejecutarse) se ejecutaría con retraso).*/
  LS = lsMin-grafo[nodo].peso;
  ls[nodo] = LS;
  return LS;
}

/* Guarda el LS de cada nodo del grafo. Ejecuta la función 'calcularLSNodo' por cada nodo dependiente de START para poder cubrir todos los 
 * nodos del grafo, ya que se trata de un grafo dirigido.
 * COSTE: O(tam·(n+m)) = O(n+m) -> Ejecuta la función calcularLSNodo de coste O(n+m) por cada nodo dependiente de START. Como tam es constante,
 * se elimina.*/
void calcularLS(){
  int tam = dependientesSTART.size();
  for(int i = 0; i < tam; ++i){
    int nodo = dependientesSTART[i];
    ls[nodo] = calcularLSNodo(nodo); 
  }
}

/* Escribe el número de espacios correspondiente en la salida, para que se cumplan las restricciones del enunciado (columnas de 6 carácteres).*/
void escribirEspacios(int tam){
  int n = 6 - tam;
  for(int i = 0; i < n; ++i){
    cout << " ";
  }
}

/* Escribe el ES, EF, LS y LF del nodo actual.
 * COSTE: constante.*/
void escribirNodo(int nodo){
  int ES, EF, LS, LF, coste;
  string cc, nombre;
  coste = grafo[nodo].peso;
  nombre = grafo[nodo].nombre;
  /* Si el nodo pertenece a algun camino crítico, entonces ES = LS y EF = LF, ya que la tarea no se puede ejecutar más tarde que ES porque  
   * sino, al encontrarse en un camino crítico retrasaría todo el proyecto.*/
  if(caminoCritico[nodo]){
    LS = ls[nodo];
    ES = LS;
    cc = "*";
  }
  /* Si el nodo no pertenece a algun camino crítico, entonces ES es igual al camino máximo hacia ese nodo (que sería cuando la tarea de más
   * coste de la que depende terminara) y LS se calcularía igual que para los demás nodos.*/
  else{
    ES = caminos[nodo];
    LS = ls[nodo];
    cc = "";
  }
  /* EF y LF se calculan sumando el coste de la tarea que representa el nodo actual a ES y LS, respectivamente.*/
  EF = ES + coste;
  LF = LS + coste;
  /* Escribimos la salida por pantalla.*/
  cout << nombre;
  escribirEspacios(nombre.size());
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
  cout << cc << endl;
}

/* Escribe el ES, EF, LS y LF de cada nodo del grafo.
 * COSTE: O(n+m) -> Recorre todo el grafo.*/
void planificacion(){
  /* Indica los nodos que ya han sido visitados (ya han la información sobre ellos ya ha sido impresa por pantalla).*/
  vector<bool> visitados(nTareas,false);
  cout << "START 0     0     0     0     *" << endl;
  for(int i = 0; i < nTareas; ++i){
    if(!visitados[i]){
      escribirNodo(i);
      visitados[i] = true;
    }
    int tam = grafo[i].adyacientes.size();
    for(int j = 0; j < tam; ++j){
      int indice = indices[grafo[i].adyacientes[j]];
      if(indice != nTareas and !visitados[indice]){
	escribirNodo(indice);
	visitados[indice] = true;
      }
    }
  }
  int aux = ls[nTareas];
  ostringstream s1;
  s1 << aux;
  cout << "END   " << aux;
  escribirEspacios(s1.str().size());
  cout << aux;
  escribirEspacios(s1.str().size());
  cout << aux;
  escribirEspacios(s1.str().size());
  cout << aux;
  escribirEspacios(s1.str().size());
  cout << "*" << endl;
}

/*COSTE: La suma de los costes de todas las funciones empleadas da un coste total de O*(n+m).*/
int main(){
  leerGrafo();
  guardarDependientesSTART();
  /* Si no se introducen tareas:*/
  if(nTareas == 0){
    cout << "START 0     0     0     0     *" << endl;
    cout << "END   0     0     0     0     *" << endl;
  }
  /* Si no hay ningun nodo que dependa de START, entonces hay algun ciclo.*/
  else if(dependientesSTART.size() == 0) cout << "Proyecto contiene ciclos" << endl;
  /*si hay algun nodo que depende de START, miramos si hay algun ciclo en el subgrafo que forman sus hijos.*/
  else if(grafoCiclico()) cout << "Proyecto contiene ciclos" << endl;
  else{
    /* Guardamos los caminos máximos hacia cada nodo y los caminos críticos del grafo en los correspondientes vectores 'caminos'
     * y 'caminoCritico', respectivamente.*/
    calcularCaminosMaximos();
    calcularcaminosCriticos();
    /* Guardamos el LS de cada nodo.*/
    calcularLS();
    /* Imprimimos la información de los nodos por pantalla.*/
    planificacion();
  }
}