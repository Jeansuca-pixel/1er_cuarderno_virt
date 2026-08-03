#include<iostream>
using namespace std;

int main(){
int numeros[100],n,mayor=0;
int suma=0;
cout << "Ingrese el tamaño del vector: ";
cin >> n;

for(int i=0;i<n;i++){
cin >> numeros[i];
suma += numeros[i];
if (numeros[i] > mayor){
mayor = numeros[i];
}
}
if(mayor==suma-mayor){
    cout << "El mayor es igual a la suma de los demas elementos" << endl;
}  
else{
    cout << "El mayor no es igual a la suma de los demas elementos" << endl;
}
return 0;

}