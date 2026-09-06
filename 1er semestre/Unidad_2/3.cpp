#include <iostream>
using namespace std;

int main() {
int V[5]={7,12,5,31,4}, Elemento, i;
cout << "Ingrese el elemento a buscar:";
cin >> Elemento;
bool Hallado = false;
      for(i = 0; i < 5; i++)
          if(Elemento == V[i]){
          Hallado = true;
          break;
        }
            if (Hallado)
                cout<<Elemento<< "Hallado en posicion"<<i<<endl;
            else
            cout<<Elemento<< "no está en el array \n";
    
}