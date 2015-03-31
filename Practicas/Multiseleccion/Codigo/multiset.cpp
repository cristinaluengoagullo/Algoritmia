#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
using namespace std;

/* Guarda los diferentes rangos que se piden.*/
vector<int> rangos;
/* Guardan el número de elementos que tiene el conjunto y el número de rangos que se piden, respectivamente.*/
int nDatos;
int nRangos;

/* Lee los datos de entrada e inicializa todas las estructuras que dependen de datos de entrada (por ejemplo, vectores que necesitan ser
 * redimensionados al conocer el número de elementos del conjunto, etc).Pasamos el vector 'datos' como referencia para poder definirlo 
 * con el tipo deseado posteriormente (en el programa se usan enteros, por ejemplo).*/
template <typename T>
void leerDatos(vector<T>& datos){
  cin >> nDatos >> nRangos;
  rangos = vector<int>(nRangos);
  datos = vector<T>(nDatos);
  for(int i = 0; i < nRangos; ++i) cin >> rangos[i];
  for(int i = 0; i < nDatos; ++i) cin >> datos[i];
}

/* Intercambia los valores contenidos en 'x' y en 'y'.*/
int swap(int& x, int& y){
  int temp;
  temp = x;
  x = y;
  y = temp;
}

/* Define una partición del vector en la que, dado un elemento 'p' perteneciente a éste, se sitúan en la parte izquierda del vector los elementos
 * menores o iguales que 'p', y en la parte derecha los elementos mayores que 'p'. 
 * PRE: l <= r, p pertenece al vector v. v.size() > 0.
 * POST: El vector 'v' se encuentra reordenado de manera que a la izquierda del elemento 'p' se encuentren los elementos menores o iguales que 
 * éste, y a la derecha se encuentren los elementos mayores que éste. Se devuelve el índice donde está situado 'p' en el vector 'v'.*/
template <typename T>
int particion(vector<T>& v, int l, int r, T p){
  /* Empezamos tomando como índices la posición anterior a 'l' y posterior a 'r', para que cuando se realice el primer ++i y --j se acceda a las
   * posiciones iniciales correspondientes.*/
  int i = l-1, j = r+1;
  /* El bucle acabará interrumpiéndose ya que sabemos que l <= r por la PRE.*/
  while(1){
    /* La 'i' va aumentando de valor hasta que se encuentra con un elemento mayor que 'p'.*/
    while(v[++i] <= p);
    /* La 'j' va disminuyendo de valor hasta que se encuentra con un elemento menor que 'p'.*/
    while(v[--j] > p);
    /* Si inicialmente i != j, entonces llegará un momento en el que i >= j, ya que 'i' y 'j' van aumentando y disminuyendo en cada iteracion.*/
    if(i>=j) return j;
    /* Si i < j, entonces intercambiamos v[i] y v[j], ya que si se llega a esta parte del código significa que se ha encontrado un v[i] > 'p'
     * y un v[j] < 'p', y que se deben intercambiar los valores para que se pueda cumplir la POST.*/
    swap(v[i],v[j]);
  }
}

/* Devuelve del vector 'v', el k-ésimo.
 * PRE: l <= k <= r.
 * POST: El vector 'v' queda modificado (parcial o totalmente ordenado). Se devuelve el k-ésimo elemento de 'v'*/
template <typename T>
int select(vector<T>& v, int l, int r, int k){
  /* Si l == r entonces el vector tiene un único elemento, y es el que devolvemos, porque se corresponderá con el k-ésimo elemento indicado por 
   * k, ya que por la PRE sabemos que l <= k <= r.*/
  if(l >= r) return v[l];
  /* Definimos como 'pivote' de la partición el último elemento del vector, y llamamos a 'particion' para que reordene el vector de manera
   * que todos los elementos situados a la izquierda del pivote sean menores o iguales que él, y los elementos de la derecha sean mayores
   * que él. Podemos realizar la llamada ya que en esta parte del código se cumple que l < r, y v[r] pertenece a v. De este modo, sabemos que
   * se cumple la PRE de 'particion'.*/
  int part = particion(v,l,r,v[r]);
  /*'rango' indica el rango que representa part en el vector.*/
  int rango = part-l+1;
  /* Si 'rango' y 'k' coinciden, entonces significa que v[part] es el elemento que estamos buscando.*/
  if(rango == k) return v[part];
  /* Si 'k' es más pequeña que 'rango', entonces significa que el elemento que estamos buscando se encuentra en la parte izquierda del vector,
   * que es la que contiene los elementos más pequeños o iguales que v[part]. Así, en esa parte se encontrará cualquier k-ésimo elemento menor 
   * que el rango-ésimo elemento del vector. Podemos realizar la llamada ya que sabemos que k < rango y por lo tanto k < part-l+1.
   * De este modo, k será un valor que se encuentre entre 'l' (porque por la PRE sabemos que l <= k) y part-1, ya que k < part-l+1 y part-1 es 
   * mayor que part-l+1.*/
  else if(k < rango) return select(v,l,part-1,k);
  /* Si 'k' es más grande que 'rango', entonces significa que el elemento que estamos buscando se encuentra en la parte derecha del vector,
   * que es la que contiene los elementos más grandes que v[part]. Así, en esa parte se encontrará cualquier k-ésimo elemento mayor 
   * que el rango-ésimo elemento del vector. Podemos realizar la llamada ya que sabemos que k > rango y por lo tanto k > part-l+1.
   * De este modo, k-rango será un valor que se encuentre entre part+1 (ya que k > part-l+1 y por lo tanto k-(part-l+1) > part+1) y 'r'
   * (porque por la PRE sabemos que k <= 'r').*/
  else return select(v,part+1,r,k-rango);
}

/* Guarda en el vector salida los diferentes valores de los k-ésimos elementos del vector 'v', en función de los valores que indica el vector 'r'
 * (indica el rango que se pide cada vez).
 * PRE: lv <= rv. Por todo rango ri que pertenece a 'r', se cumple que lv <= ri <= rv. el vector 'r' está ordenado.
 * POST: Quedan guardados en el vector 'salida' los valores de los k-ésimos elementos que indica el vector 'r'.*/
template <typename T>
void multiselect(vector<T>& v, vector<int>& r, vector<T>& salida, int lv, int rv, int lr, int rr){
  /* Si aún podemos explorar algun elemento de 'r':*/
  if(lr <= rr){
      int tam = (lr+rr)/2;
      /* Escojemos el elemento medio del vector de rangos para poder dividir las llamadas recursivas.*/
      int k = r[tam];
      /* Obtenemos el elemento de 'v' que se corresponde con el rango indicado por 'k'. Podemos realizar la llamada ya que sabemos por la PRE que 
       * lv <= rv y que todo rango indicado en 'r' se encuentra entre lv y rv.*/
      int s = select(v,lv,rv,k-lv);
      /* Guardamos el elemento seleccionado en el vector de salida.*/
      salida[tam] = s;
      /* Realizamos una partición del vector 'v' tomando como elemento 'pivote' a 's'. Podemos realizar la llamada ya que por la PRE sabemos que 
       * lv <= rv y sabemos que 's' pertenece a 'v'.*/ 
      int part = particion(v,lv,rv,s);
      /* Realizamos una llamada recursiva sobre los elementos que se encuentran a la izquierda de 's' (más pequeños o iguales que éste) y 
       * los elementos de 'r' más pequeños que 'k' (ya que por la PRE sabemos que el vector 'r' está ordenado y que los elementos que se 
       * encuentren a la izquierda de 'k' en 'r' serán más pequeños que éste). Así pues, podemos asegurar que los rangos que se pedirán en 
       * 'v' estarán contenidos entre lv y part-1, ya que 'k' estaba contenido entre lv y rv.*/
      multiselect(v,r,salida,lv,part-1,lr,tam-1);
      /* Realizamos otra llamada recursiva sobre los elementos que se encuentran a la derecha de 's' (mayores que éste) y 
       * los elementos de 'r' más grandes que 'k' (ya que por la PRE sabemos que el vector 'r' está ordenado y que los elementos que se 
       * encuentren a la derecha de 'k' en 'r' serán más grandes que éste). Así pues, podemos asegurar que los rangos que se pedirán en 
       * 'v' estarán contenidos entre part y rv, ya que 'k' estaba contenido entre lv y rv.*/
      multiselect(v,r,salida,part,rv,tam+1,rr);
  }
}

/* Escribe la salida por pantalla. Pasamos el vector 'salida' como referencia para poder definirlo 
 * con el tipo deseado posteriormente (en el programa se usan enteros, por ejemplo).*/
template <typename T>
void escribirSalida(const vector<T>& salida){                          
  for(int i = 0; i < nRangos; ++i){
    cout << salida[i];
    if(i < nRangos-1) cout << " ";
  }
  cout << endl;
}

int main(){
  vector<int> datos;
  /* Leemos los datos de entrada.*/
  leerDatos<int>(datos);
  vector<int> salida(nRangos,0);
  /* Llamamos a la función multiselect sobre datos, rangos y salida, tomando como límites el primer y último índice de cada vector.
   * Podemos realizar la llamada ya que se cumple que los límites para 'datos' son válidos (0 < nDatos-1) y para 'rangos' también (0 < nRangos-1).
   * Además, sabemos por el enunciado que la información referente a los rangos se introduce de forma creciente, y por lo tanto el vector 'r'
   * estará ordenado.*/
  multiselect<int>(datos,rangos,salida,0,nDatos-1,0,nRangos-1);
  /* Ordenamos la salida (añade un coste O(nRangos·log(nRangos)), pero no afecta al coste total ya que nRangos·log(nRangos) <= nDatos·log(nRangos),
   * y por lo tanto el coste seria O(nRangos·log(nRangos) + nDatos·log(nRangos)) = O(nDatos·log(nRangos)).*/
  sort(salida.begin(),salida.end());
  escribirSalida<int>(salida);
}