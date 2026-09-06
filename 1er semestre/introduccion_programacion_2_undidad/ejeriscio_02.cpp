#include<iostream>
using namespace std;

int main(){
int numeros[100],numeros2[100],n;

cout << "Ingrese el tamaño del vector: ";
cin >> n;
for(int i=0;i<n;i++){
    cin >> numeros[i];
    numeros2[i] = numeros[i]*2;
}
for(int i=0;i<n;i++){ 
    cout <<numeros2[i] << ",";
}
return 0;

}