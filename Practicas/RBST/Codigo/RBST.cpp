/* PRUEBA JUTGE 
 * PRACTICA 4 ALGORÍSMIA: RBST
 * POR: CRISTINA LUENGO AGULLÓ
 * 	JAVIER NAVARRO GONZÁLEZ */

#include <iostream>
#include <map>
#include <sstream>
#include <stdlib.h>

using namespace std;

struct Nodo {
  string clave;					//Clave de un nodo
  int tamano;            			//Tamaño del subarbol del cual es la raíz
  Nodo* izquierdo;				//Subarbol izquierdo
  Nodo* derecho;           			//Subarbol derecho
  Nodo(): clave(), tamano(0), izquierdo(NULL), derecho(NULL){};
};

map<string,Nodo*> mapaArboles;

int recalcular_tamano (Nodo* raiz) {
  if (raiz == NULL) return 0;
  int tamI = 0, tamD = 0;
  if (raiz->izquierdo != NULL) tamI = recalcular_tamano(raiz->izquierdo);
  if (raiz->derecho != NULL) tamD = recalcular_tamano(raiz->derecho);
  raiz->tamano = tamI + tamD + 1;
  return raiz->tamano;
}

void split (string clave, Nodo* raiz, Nodo** I, Nodo** D) {
  if (raiz == NULL) {
    *I = *D = NULL;
    return;
  }
  if (clave < raiz->clave) {
    *D = raiz;
    split(clave,raiz->izquierdo,I,&((*D)->izquierdo));
  }
  else {
    *I = raiz;
    split(clave,raiz->derecho,&((*I)->derecho),D);
  }
  return;
}

Nodo* ins_en_raiz (string clave, Nodo* raiz) {
  Nodo* I = new Nodo;
  Nodo* D = new Nodo;
  split(clave,raiz,&I,&D);
  raiz = new Nodo;
  raiz->clave = clave;
  raiz->derecho = D;
  raiz->izquierdo = I;
  int tamIzq = 0, tamDer = 0;
  if (I != NULL) {
    int tamI = 0, tamD = 0;
    if(I->izquierdo != NULL) tamI = I->izquierdo->tamano;
    if(I->derecho != NULL) tamD = I->derecho->tamano;
    I->tamano = tamD + tamI + 1;
    tamIzq = I->tamano;
  }
  if (D != NULL) tamDer = D->tamano;
  raiz->tamano = tamIzq + tamDer + 1;
  return raiz;
}

Nodo* join (Nodo* I, Nodo* D) {
  int m = 0, n = 0, r, total;
  if (I != NULL) m = I->tamano;
  if (D != NULL) n = D->tamano;
  total = m + n;
  if (total == 0) return NULL;
  else r = rand()%total;
  if (r < m) {
    I->derecho = join(I->derecho,D);
    return I;
  }
  else {
    D->izquierdo = join(I,D->izquierdo);
    return D;
  }
}

Nodo* push_down (Nodo* raiz) {
  Nodo* aux;
  int m = 0, n = 0, r, total;
  if (raiz->izquierdo != NULL) m = raiz->izquierdo->tamano;
  if (raiz->derecho != NULL) n = raiz->derecho->tamano;
  total = m + n;
  r = rand()%(total+1);
  if (r < m) {
    aux = raiz->izquierdo;
    raiz->izquierdo = raiz->izquierdo->derecho;
    aux->derecho = push_down(raiz);
    return aux;
  }
  else if (r < total) {
    aux = raiz->derecho;
    raiz->derecho = raiz->derecho->izquierdo;
    aux->izquierdo = push_down(raiz);
    return aux;
  }
  return raiz;
}

Nodo* ins (string clave, Nodo* raiz, bool& insertado) {
  int n, r;
  if (raiz == NULL) n = 0;
  else n = raiz->tamano;
  if (raiz != NULL and clave == raiz->clave) insertado = false;
  else {
    if (n == 0) r = 0;
    else r = rand()%(n+1);
    if (r == n) return ins_en_raiz(clave,raiz);
    if (clave < raiz->clave) raiz->izquierdo = ins(clave,raiz->izquierdo,insertado);
    else if (clave > raiz->clave) raiz->derecho = ins(clave,raiz->derecho,insertado);
    if (insertado) raiz->tamano++;
  }
  return raiz;
}

Nodo* del (string clave, Nodo* raiz) {
  Nodo* aux;
  if (raiz == NULL) return NULL;
  if (clave == raiz->clave) {
    aux = join(raiz->izquierdo,raiz->derecho);
    delete raiz;
    raiz = aux;
  }
  else {
    if (clave < raiz->clave) raiz->izquierdo = del(clave,raiz->izquierdo);
    else if (clave > raiz->clave) raiz->derecho = del(clave, raiz->derecho);
    int tamI = 0, tamD = 0;
    if (raiz->derecho != NULL) tamD = raiz->derecho->tamano;
    if (raiz->izquierdo != NULL) tamI = raiz->izquierdo->tamano; 
    raiz->tamano = tamI + tamD + 1;
  }
  return raiz;
}

bool cont (Nodo *raiz, string clave) {
  if (raiz == NULL) return false;
  if (clave < raiz->clave) return cont(raiz->izquierdo,clave);
  if (clave > raiz->clave) return cont(raiz->derecho,clave);
  return true;
}

bool split_without_copy (string clave, Nodo* raiz, Nodo** I, Nodo** D) {
  bool ret;
  if (raiz == NULL) {
    *I = *D = NULL;
    ret = false;
  }
  else if (clave < raiz->clave) {
    *D = raiz;
    ret = split_without_copy(clave,raiz->izquierdo,I,&((*D)->izquierdo));
  }
  else if (clave > raiz->clave) {
    *I = raiz;
    ret = split_without_copy(clave,raiz->derecho,&((*I)->derecho),D);
  }
  else {    
    raiz = del(clave,raiz);
    ret = true;
  }
  return ret;
}

Nodo* merge (Nodo* A, Nodo* B) {
  Nodo* Ai = new Nodo;
  Nodo* Ad = new Nodo;
  Nodo* Bi = new Nodo;
  Nodo* Bd = new Nodo; 
  int m = 0, n = 0, u, total, rep;
  if (A != NULL) m = A->tamano;
  if (B != NULL) n = B->tamano;
  total = m + n;
  if (total == 0) return NULL;
  u = rand()%total + 1;
  if (u <= m) {
    split_without_copy(A->clave,B,&Bi,&Bd);
    A->izquierdo = merge(A->izquierdo,Bi);
    A->derecho = merge(A->derecho,Bd);
    return A;
  }
  else {
    rep = split_without_copy(B->clave,A,&Ai,&Ad);
    B->izquierdo = merge(B->izquierdo,Ai);
    B->derecho = merge(B->derecho,Ad);
    if (rep) return push_down(B);
    else return B;
  }
}

int card (Nodo *raiz) {
  if (raiz == NULL) return 0; 
  return raiz->tamano;
}

string nth (Nodo* raiz, int i) {
  int pivote;
  if ((raiz->izquierdo == NULL and (raiz->derecho == NULL)) and (i == 1)) return raiz->clave;
  else {
    if (raiz->izquierdo != NULL) pivote = raiz->izquierdo->tamano + 1;
    else pivote = 1;
    if (i < pivote and raiz->izquierdo != NULL) return nth(raiz->izquierdo,i);
    else if (i == pivote) return raiz->clave;
    else if (raiz->derecho != NULL) return nth(raiz->derecho,i-pivote);
  }
}

int leq (Nodo *raiz, string elem) {
  if (raiz == NULL) return 0;
  int s1 = 0, s2 = 0;
  //if (raiz->izquierdo != NULL and raiz->izquierdo->clave <= elem) s1 = leq(raiz->izquierdo,elem);
  //if (raiz->derecho != NULL and raiz->derecho->clave <= elem) s2 = leq(raiz->derecho,elem);
  if (raiz->clave > elem) s1 = leq(raiz->izquierdo,elem);
  else {
    s1 = leq(raiz->izquierdo,elem);
    s2 = leq(raiz->derecho,elem);
  }
  if (raiz->clave <= elem) return s1+s2+1;
  return s1+s2;
}
/*int leq (Nodo *raiz, string elem) {
  if (raiz == NULL) return 0;
  int s1 = leq(raiz->izquierdo,elem);
  int s2 = leq(raiz->derecho,elem);
  if (raiz->clave <= elem) return s1+s2+1;
  return s1+s2;
}

int gt (Nodo *raiz, string elem) {
  if (raiz == NULL) return 0;
  int s1, s2;
  if (raiz->izquierdo != NULL and raiz->izquierdo->clave > elem) s1 = gt(raiz->izquierdo,elem);
  if (raiz->derecho != NULL and raiz->derecho->clave > elem) s2 = gt(raiz->derecho,elem);
  if (raiz->clave > elem) return s1+s2+1;
  return s1+s2;
}*/

int gt (Nodo *raiz, string elem) {
  if (raiz == NULL) return 0;
  int s1 = gt(raiz->izquierdo,elem);
  int s2 = gt(raiz->derecho,elem);
  if (raiz->clave > elem) return s1+s2+1;
  return s1+s2;
}

void between (Nodo* raiz, string elem1, string elem2, bool &coma) {
  if (raiz != NULL) {
    between(raiz->izquierdo,elem1,elem2,coma);
    if (raiz->clave >= elem1 and raiz->clave <= elem2) {
      if (coma) cout << ",";
      cout << raiz->clave;
      coma = true;
    }
    between(raiz->derecho,elem1,elem2,coma);
  }
}

string min (Nodo* raiz) {
  if (raiz->izquierdo != NULL) return min(raiz->izquierdo);
  return raiz->clave;
}

string max (Nodo* raiz) {
  if (raiz->derecho != NULL) return max(raiz->derecho);
  return raiz->clave;
}

void all (Nodo* raiz, bool &coma2) {
  if (raiz != NULL) {
    all(raiz->izquierdo,coma2);
    if (coma2) cout << ",";
    cout << raiz->clave;
    coma2 = true;
    all(raiz->derecho,coma2);
  }
}
 
int main () {
  string s, aux, aux2;
  while (getline(cin,s)) {
    istringstream ss(s);
    ss >> aux;
    if (aux == "init") {
      cout << "> " << s << endl;
      ss >> aux;
      mapaArboles[aux] = NULL;
      cout << "OK" << endl;
    }
    else if (aux == "ins") {
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) {
	ss >> aux2;
	bool insertado = true;
	mapaArboles[aux] = ins(aux2,mapaArboles[aux],insertado);
	cout << "OK" << endl;
      }
      else cout << "ERROR" << endl;
    }
    else if (aux == "del") {
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) {
	ss >> aux2;
	mapaArboles[aux] = del(aux2,mapaArboles[aux]);	    
	cout << "OK" << endl;
      }
      else cout << "ERROR" << endl;
    }
    else if (aux == "cont") {
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) {
	ss >> aux2;
	if (cont(mapaArboles[aux],aux2)) cout << "true" << endl;
	else cout << "false" << endl;
      }
      else cout << "ERROR" << endl;
    }
    else if (aux == "merge") {
      cout << "> " << s << endl;
      ss >> aux;
      ss >> aux2;
      map<string,Nodo*>::iterator it, it2;
      it = mapaArboles.find(aux);
      it2 = mapaArboles.find(aux2);
      if (it != mapaArboles.end() and it2 != mapaArboles.end()) {
	mapaArboles[aux] = merge(mapaArboles[aux],mapaArboles[aux2]);
	recalcular_tamano(mapaArboles[aux]);
	mapaArboles[aux2] = NULL;
	cout << "OK" << endl;
      }
      else cout << "ERROR" << endl;
    }
    else if (aux == "card") {
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) cout << card(mapaArboles[aux]) << endl;
      else cout << "ERROR" << endl;
    }
    else if (aux == "nth") {
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) {
	int i;
	ss >> i;
	if (mapaArboles[aux]->tamano < i) cout << "ERROR" << endl;
	else cout << nth(mapaArboles[aux],i) << endl;
      }
      else cout << "ERROR" << endl;
    }
    else if (aux == "leq") {
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) {
	ss >> aux2;
	cout << leq(mapaArboles[aux],aux2) << endl;
      }
      else cout << "ERROR" << endl;
    }
    else if (aux == "gt") {	
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) {
	ss >> aux2;
	cout << gt(mapaArboles[aux],aux2) << endl;
      }
      else cout << "ERROR" << endl;
    }
    else if (aux == "between") {
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) {
	string aux3;
	ss >> aux2;
	ss >> aux3;
	if (aux2 > aux3) cout << "[]" << endl;
	else {
	  bool coma = false;
	  cout << "[";
	  between(mapaArboles[aux],aux2,aux3,coma);
	  cout << "]" << endl;
	}
      }
      else cout << "ERROR" << endl;
    }
    else if (aux == "min") {
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) {
	if (mapaArboles[aux] == NULL) cout << "ERROR" << endl;
	else cout << min(mapaArboles[aux]) << endl;
      }
      else cout << "ERROR" << endl;
    }
    else if (aux == "max") {
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) {
	if (mapaArboles[aux] == NULL) cout << "ERROR" << endl;
	else cout << max(mapaArboles[aux]) << endl;
      }
      else cout << "ERROR" << endl;
    }
    else if (aux == "all") {
      cout << "> " << s << endl;
      ss >> aux;
      map<string,Nodo*>::iterator it;
      it = mapaArboles.find(aux);
      if (it != mapaArboles.end()) {
	bool coma = false;
	cout << "[";
	all(mapaArboles[aux],coma);
	cout << "]" << endl;
      }
      else cout << "ERROR" << endl;
    }
  }
}