//programa para saber que nota minima debes sacar para aprobar un curso xd//
#include <iostream>
using namespace std;

int main() {
    int n,nota_minima;
    int i;
    cout << "Ingrese la nota de la unidad 1: "<<endl;
    cin >> n;
    if(n<0 || n>20){
        cout << "Nota no valida, ingrese una nota entre 0 y 20." << endl;
        return 1;
    }
    else{
    if(n%2==0)
        i=n/2;
    else
        i=(n+1)/2;
    nota_minima=11-i;
    cout << "La nota mínima necesaria en la segunda unidad para aprobar\nel semestre con 11 es: " <<2*nota_minima-1 << endl;
    }
return 0;
}   