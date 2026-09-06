#include <iostream>
using namespace std;
int main()
{
    int numero,cosiente1,cosiente2,residuo1,residuo2,suma;
    cout<<"Ingrese numero de 3 digitos: ";
    cin>>numero;
    cosiente1=numero/100;
    residuo1=numero%100;
    cosiente2=residuo1/10;
    residuo2=residuo1%10;
    suma=cosiente1+cosiente2+residuo2;
    cout<<"Suma es: "<<suma<<endl;

    return 0;
}