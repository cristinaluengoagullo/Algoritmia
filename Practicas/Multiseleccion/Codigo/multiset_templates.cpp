#include <iostream>
#include <map>
#include <vector>
using namespace std;

/*
template <typename T>
class TemplateMap{
  typedef typename map<T> TMap;
  TMap map;
public:
  typedef typename TMap::iterator iterator;
  iterator begin() { return map.begin();}
  iterator end() {return map.end();}
};
*/

map<int,int> datos;
vector<int> rangos;
int nDatos;
int nRangos;

void leerDatos(){
  cin >> nDatos >> nRangos;
  rangos = vector<int>(nRangos);
  for(int i = 0; i < nRangos; ++i){
    cin >> rangos[i];
  }
  for(int i = 0; i < nDatos; ++i){
    int dato;
    cin >> dato;
    datos[dato]++;
  }
}

void imprimirMins(){
  for(int i = 0; i < nRangos; ++i){
    int rango = rangos[i];
    map<int,int>::iterator it = datos.begin();
    int apariciones;
    while(it != datos.end() and rango > 0){
      apariciones = it->second;
      if(apariciones == 1){
	--rango;
	if(rango > 0) ++it;
      }
      else{
	for(int i = 0; i < apariciones; ++i) --rango;
	if(rango > 0) ++it;
      }
    }
    cout << it->first;
    if(i < nRangos-1) cout << " ";
  }
  cout << endl;
}

int main(){
  leerDatos();
  imprimirMins();
}