#include <iostream>
#include <map>
#include <list>
#include <sstream>
#include <stdlib.h>

using namespace std;

struct Nodo {
    string clave;				//Clave de un nodo
    int tamano;            			//Tamaño del subarbol del cual es la raíz
    Nodo* izquierdo;				//Subarbol izquierdo
    Nodo* derecho;           			//Subarbol derecho
    Nodo(): clave(), tamano(0), izquierdo(NULL), derecho(NULL){};
};

map<string,Nodo*> mapaArboles;
list<string> l;


void listarElementos(Nodo* raiz) {
  if (raiz != NULL) {
    listarElementos(raiz->izquierdo);
    l.push_back(raiz->clave);
    listarElementos(raiz->derecho);
  }
}

void escribirElementos() {
  list<string>::iterator it;
  bool primero = true;
  cout << "[";
  for (it = l.begin(); it != l.end(); it++) {
    if (*it != "") {
      if (!primero) cout << ",";
      primero = false;
      cout << *it;
    }
  }
  cout << "]" << endl;
}

int recalcular_tamano(Nodo* raiz){
  if(raiz == NULL) return 0;
  int tamI = 0, tamD = 0;
  if(raiz->izquierdo != NULL) tamI = recalcular_tamano(raiz->izquierdo);
  if(raiz->derecho != NULL) tamD = recalcular_tamano(raiz->derecho);
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
  //cout << "clave = " << clave << endl;
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
  //if(I != NULL) tamI = I->tamano;
  /*raiz->tamano = recalcular_tamano(raiz);*/
  //if(D != NULL) tamD = D->tamano;
  //cout << "I = ";
  //l.clear();
  //listarElementos(I);
  //escribirElementos();
  //cout << "D = ";
  //l.clear();
  //listarElementos(D);
  //escribirElementos();
  //raiz->tamano = tamI + tamD + 1;
  return raiz;
}

Nodo* ins (string clave, Nodo* raiz, bool& insertado) {
  int n, r;
  if(raiz == NULL) n = 0;
  else n = raiz->tamano;
  //if (raiz != NULL) cout << "ins raiz->clave = " << raiz->clave << ", tam = " << n << endl;
  if (raiz != NULL and clave == raiz->clave) insertado = false;
  else {
    if (n == 0) r = 0;
    else r = rand()%(n+1);
    if (r == n) return ins_en_raiz(clave,raiz);
    if (clave < raiz->clave) raiz->izquierdo = ins(clave,raiz->izquierdo,insertado);
    else if (clave > raiz->clave) raiz->derecho = ins(clave,raiz->derecho,insertado);
    //cout << "insertado = " << insertado << endl;
    if (insertado) raiz->tamano++;
  }
  return raiz;
}

Nodo* join (Nodo* I, Nodo* D) {
  int m = 0, n = 0, r, total;
  if(I != NULL) m = I->tamano;
  if(D != NULL) n = D->tamano;
  total = m + n;
  if (total == 0) return NULL;
  //if (total - 1 == 0) r = 0;
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

bool split_without_copy (string clave, Nodo* raiz, Nodo** I, Nodo** D) {
  bool ret;
  if (raiz == NULL) {
    //cout << "entra raiz == NULL" << endl;
    *I = *D = NULL;
    ret = false;
  }
  else if (clave < raiz->clave) {
    //cout << "Entra clave < raiz->clave | raiz->clave = " << raiz->clave << endl;
    *D = raiz;
    ret = split_without_copy(clave,raiz->izquierdo,I,&((*D)->izquierdo));
  }
  else if (clave > raiz->clave) {
    //cout << "Entra clave > raiz->clave | raiz->clave = " << raiz->clave << endl;
    *I = raiz;
    ret = split_without_copy(clave,raiz->derecho,&((*I)->derecho),D);
  }
  else {    
    //cout << "Entra clave == raiz->clave | raiz->clave = " << raiz->clave << endl;
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
  //Nodo* Ai, *Ad, *Bi, *Bd;
  int m = 0, n = 0, u, total, rep;
  if (A != NULL) m = A->tamano;
  if (B != NULL) n = B->tamano;
  total = m + n;
  //cout << "m = " << m << ", n = " << n << endl;
  if (total == 0) return NULL;
  //if(total-1 == 0) u = 1;
  //else 
  u = rand()%total + 1;
  //cout << "m = " << m << ", n = " << n <<", total = " << total << ", u = " << u << endl;
  if (u <= m) {
    //cout << "entra u<=m con clave = " << A->clave << endl;
    split_without_copy(A->clave,B,&Bi,&Bd);
    //split(A->clave,B,&Bi,&Bd);
    //cout << "Bi->tamano = " << Bi->tamano << endl;
    //cout << "Bd->tamano = " << Bd->tamano << endl;
    //Nodo* aux = merge(A->izquierdo,Bi);
    A->izquierdo = merge(A->izquierdo,Bi);
    //A->izquierdo = aux;
    //cout << "A->izquierdo = " << A->izquierdo->clave << endl;
    //aux = merge(A->derecho,Bd);
    //A->derecho = aux;
    A->derecho = merge(A->derecho,Bd);
    return A;
  }
  else {
    //cout << "entra u>m con claveA = " << A->clave << endl;
    //cout << "entra u>m con claveB = " << B->clave << endl;
    rep = split_without_copy(B->clave,A,&Ai,&Ad);
    //split(B->clave,A,&Ai,&Ad);
    //cout << "clave = " << B->clave << ", rep = " << rep << endl;
    //cout << "Ai->tamano = " << Ai->tamano << endl;
    //cout << "Ad->tamano = " << Ad->tamano << endl;
    B->izquierdo = merge(B->izquierdo,Ai);
    //cout << "sale izquierdo " << endl;
    B->derecho = merge(B->derecho,Ad);
    //cout << "sale derecho" << endl;
    if (rep) return push_down(B);
    else return B;
  }
}

bool cont (Nodo *raiz, string clave) {
  if (raiz == NULL) return false;
  if (clave < raiz->clave) return cont(raiz->izquierdo,clave);
  if (clave > raiz->clave) return cont(raiz->derecho,clave);
  return true;
}

int card (Nodo *raiz) {
    if (raiz == NULL) return 0; 
    return raiz->tamano;
}

int leq (Nodo *raiz, string elem) {
    if (raiz == NULL) return 0;
    int s1 = leq(raiz->izquierdo,elem);
    int s2 = leq(raiz->derecho,elem);
    if (raiz->clave <= elem) return s1+s2+1;
    return s1+s2;
}

int gt (Nodo *raiz, string elem) {
    if (raiz == NULL) return 0;
    int s1 = gt(raiz->izquierdo,elem);
    int s2 = gt(raiz->derecho,elem);
    if (raiz->clave > elem) return s1+s2+1;
    return s1+s2;
}

string max (Nodo* raiz) {
  if (raiz->derecho != NULL) return max(raiz->derecho);
  return raiz->clave;
}

string min (Nodo* raiz) {
  if (raiz->izquierdo != NULL) return min(raiz->izquierdo);
  return raiz->clave;
}

void between (Nodo* raiz, string elem1, string elem2) {
  if (raiz != NULL) {
    if (raiz->clave >= elem1 and raiz->clave <= elem2) l.push_back(raiz->clave);
    between(raiz->derecho,elem1,elem2);
    between(raiz->izquierdo,elem1,elem2);
  }
}

string nth (int i) {
  int cont = 1;
  list<string>::iterator it;
  for (it = l.begin(); it != l.end(); it++) {
    if (cont == i) break;
    cont++;
  }
  return *it;
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
	/*Nodo *n = new Nodo;
	mapaArboles[aux2] = n;*/
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
	l.clear();
	listarElementos(mapaArboles[aux]);
	if (l.size() < i) cout << "ERROR" << endl;
	else cout << nth(i) << endl;
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
	  l.clear();
	  between(mapaArboles[aux],aux2,aux3);
	  escribirElementos();
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
	//if (mapaArboles[aux]->clave == "") cout << "ERROR" << endl;
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
	//if (mapaArboles[aux]->clave == "") cout << "ERROR" << endl;
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
	l.clear();
	listarElementos(mapaArboles[aux]);
	escribirElementos();
      }
      else cout << "ERROR" << endl;
    }
  }
}