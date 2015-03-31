#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <limits>
using namespace std;

/* 'w' representa la longitud de la linea especificada en la entrada y 'nPalabras' el número de palabras totales en todo el texto.*/
int w, nPalabras;
/* Guarda un identificador para cada palabra.*/
map<int,string> palabras;
/* Guarda la longitud de cada palabra*/
vector<int> longitudesPalabras;
/* Guarda, por cada 'i' y 'j', la longitud de una línea que incluya las palabras desde la palabra con índice 'i' hasta la palabra con índice 'j'
 * (donde i <= j).*/
vector<vector<int> > longitudesLineas;
/* Guarda, por cada 'i' y 'j', el coste de incluir en una misma línea las palabras desde la palabra con índice 'i' hasta la palabra con índice 'j'
 * (donde i <= j).*/
vector<vector<int> > costesLineas;
/* Guarda, por cada palabra representada por un índice 'i', el coste de distribuir las palabras en líneas desde la palabra con identificador 1, hasta la 
 * palabra con identificador 'i'.*/
vector<int> coste;
/* Guarda los identificadores de las palabras a partir de las cuales se realiza un salto de línea.*/
set<int> saltosDeLinea;
/* Ayuda para mostrar la salida correctamente. Por cada posición del vector (que representa una palabra del texto), indica hasta que palabra (en 
 * orden descendiente, teniendo en cuenta los índices) se forma una línea.*/
vector<int> rangos;
const int MAX = numeric_limits<int>::max();

void leer_texto(){
  cin >> w;
  string s;
  int i = 0;
  while (getline(cin, s)) {
    if(i > 0 and s.empty()) saltosDeLinea.insert(i);
    istringstream ss(s);
    string palabra;
    while (ss >> palabra){
      // Guardamos el identificador de la palabra.
      palabras[i+1] = palabra;
      // Guardamos la longitud de la palabra.
      longitudesPalabras.push_back(palabra.size());
      ++i;
    }
  }
  nPalabras = longitudesPalabras.size();
  /* Si se produce un salto de línea al final del texto, eliminamos la última palabra del conjunto para evitar equivocaciones a la hora de mostrar
   * la salida.*/
  saltosDeLinea.erase(nPalabras);
  // Sumamos 1 al número de palabras ya que usaremos identificadores desde 1 hasta nPalabras para referirnos a las palabras del texto.
  longitudesLineas = vector<vector<int> >(nPalabras+1,vector<int>(nPalabras+1));
  costesLineas = vector<vector<int> >(nPalabras+1,vector<int>(nPalabras+1,MAX));
  coste = vector<int>(nPalabras+1);
  rangos = vector<int>(nPalabras+1);
}

void wordwrap(int palabraInicio, int palabraFin){
  for (int i = palabraInicio; i <= palabraFin; i++){
    /* Inicializamos la longitud de la línea que va desde 'i' hasta 'i' con el tamaño de la palabra identificada por 'i', que se encuentra en 
     * longitudesPalabras[i-1] (ya que el vector longitudesPalabras es de tamaño nPalabras, y las palabras se identifican desde 0 hasta nPalabras).*/
    longitudesLineas[i][i] = longitudesPalabras[i-1];
    for (int j = i+1; j <= palabraFin; j++)
    /* La longitud de una línea que va desde 'i' hasta 'j' es igual a la longitud de la línea que va desde 'i' hasta 'j-1', mas la longitud de la
     * palabra representada por el índice 'j', más una unidad, que representa el espacio entre palabras.
     * Las líneas que se consideran a la hora de calcular su longitud son correctas, ya que siempre se cumple que i > j.*/
    longitudesLineas[i][j] = longitudesLineas[i][j-1] + longitudesPalabras[j-1] + 1;
  }
  /* Una vez que tenemos las longitudes de las distintas líneas que se puden crear, podemos calcular el coste que supone la creación de cada
   * una de ellas.*/ 
  for (int i = palabraInicio; i <= palabraFin; i++){
    /* Por cada palabra del párrafo delimitado por 'palabraInicio' y 'palabraFin', calculamos el coste que supondría ir añadiendo palabras en la
     * misma línea que ésta.
     * Costes líneas al salir de los dos bucles contendrá, por cada 'i' y 'j', el coste que supondría crear una línea con todas las palabras los
     * identificadores de las cuales se encuentren entre 'i' y 'j'.*/
    for (int j = i; j <= palabraFin; j++) costesLineas[i][j] = (longitudesLineas[i][j]-w)*(longitudesLineas[i][j]-w);
  }
  coste[0] = 0;
  for (int j = palabraInicio; j <= palabraFin; j++){
    // Inicializamos el coste con un valor lo suficientemente grande.
    coste[j] = MAX;
    // Calculamos el coste que supondría crear una línea desde 'i' hasta 'j'.
    for (int i = palabraInicio; i <= j; i++){
      /* Si el coste de la línea creada desde la palabra 1 hasta la palabra 'i-1' no es excesivamente elevado, y el coste de esa misma línea es
       * inferior que el coste de crear una línea desde la palabra 1 hasta la palabra 'j', entonces el coste de crear una línea desde la palabra 1
       * hasta la palabra 'j' será la suma del coste desde la palabra 1 hasta la palabra 'i-1', más el coste que supone crear una línea desde 
       * 'i' hasta 'j'. De esta manera, conseguimos guardar en 'coste[j]' el coste óptimo cada vez que realizamos la comparación mencionada.
       * Así pues, en este trozo de código se evalua si es más óptimo construir una línea de 1 a 'j', o de 'i' a 'j'.*/
      if (coste[i-1] != MAX && (coste[i-1] + costesLineas[i][j] < coste[j])){
	coste[j] = coste[i-1] + costesLineas[i][j];
	/* En 'rangos[j]' quedará guardada la última 'i' que se visite por una 'j' dada, y por lo tanto, por cada 'j' sabremos hasta que palabra
	* identificada por 'i' llegará la línea que contenga la palabra 'j', en orden inverso de lectura.*/
	rangos[j] = i;
      }
    }
  }
}

/* En el vector de rangos se especifica, por cada índice 'i', la palabra hasta la cual llega la línea que contenga la palabra identificada por 'i',
 * en orden inverso de lectura. Así pues, debemos visitar el vector de manera inversa y accediendo cada vez, a las casillas los índices de las
 * cuales vengan identificados por el contenido de la casilla 'rangos[PalabraFin].*/
void escribirLinea(int palabraFin){
  if(rangos[palabraFin] > 0){
    escribirLinea(rangos[palabraFin]-1);
    for(int i = rangos[palabraFin]; i <= palabraFin; ++i){
      cout << palabras[i];
      if(i < palabraFin) cout << " ";
    }
    cout << endl;
  }
}

/* El coste total del algoritmo para un párrafo de longitud N es de O(N^2), ya que las estructuras de datos de mayor longitud son de NxN casillas, 
 * y se recorren todas en los bucles definidos en la función wordwrap.*/
int main() {
   leer_texto();
   int tam = saltosDeLinea.size();
   int palabraInicio = 1, palabraFin = 0;
   int penalty;
   for(set<int>::iterator it = saltosDeLinea.begin(); it!= saltosDeLinea.end(); ++it){
     palabraFin = *it;
     wordwrap(palabraInicio,palabraFin);
     escribirLinea(palabraFin);
     penalty = coste[palabraFin];
     cout << "Penalty: " << penalty << endl << endl;
     palabraInicio = palabraFin + 1;
     coste = vector<int>(nPalabras+1);
     rangos = vector<int>(nPalabras+1); 
   }
   wordwrap(palabraFin+1,nPalabras);
   escribirLinea(nPalabras);
   penalty = coste[nPalabras];
   cout << "Penalty: " << penalty << endl;
}