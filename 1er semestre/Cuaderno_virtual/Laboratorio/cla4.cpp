#include <iostream>
using namespace std;
int main()
{
    int numero,cosiente1,cosiente2,cosiente3,residuo1,residuo2,residuo3,n,suma;
    cout<<"Ingrese numero de 4 digitos: ";
    cin>>numero;
    cout<<"Ingrese numero n: ";
    cin>>n;
    cosiente1=numero/1000;
    residuo1=numero%1000;
    cosiente2=residuo1/100;
    residuo2=residuo1%100;
    cosiente3=residuo2/10;
    residuo3=residuo2%10;
    suma=(cosiente1*10000)+(cosiente2*1000)+(cosiente3*100)+(n*10)+(residuo3);
    cout<<suma<<endl;
}