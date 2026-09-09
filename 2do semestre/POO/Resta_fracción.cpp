#include <iostream>
#include <stdio.h>
using namespace std;

class Fraccion
{
    public:
    //atributos
        int numerador;
        int denominador;
    //metodos
    void imprimir()
    {
        cout<<"*************************************"<<endl;
        cout<<"* ";
        cout<<numerador<<"/"<<denominador<<endl;
        cout<<"*************************************"<<endl;
    }
    Fraccion Restar(Fraccion a)
    {
        Fraccion resultado = Fraccion();
        resultado.numerador = (numerador * a.denominador)-(denominador * a.numerador);
        resultado.denominador = denominador * a.denominador;
        return resultado;
    }
};


//multiplicación de fracciones
int main() {

    Fraccion fraccion_a = Fraccion();
    Fraccion fraccion_b = Fraccion();
    Fraccion resultado = Fraccion();

    cout<<"Ingrese el numerador de la primera fracción"<<endl;
    cin>>fraccion_a.numerador;
    cout<<"Ingrese el denominador de la primera fracción"<<endl;
    cin>>fraccion_a.denominador;
    cout<<"Ingrese el numerador de la segunda fracción"<<endl;
    cin>>fraccion_b.numerador;
    cout<<"Ingrese el denominador de la segunda fracción"<<endl;
    cin>>fraccion_b.denominador;

    resultado = fraccion_a.Restar(fraccion_b);

    fraccion_a.imprimir();
    fraccion_b.imprimir();
    resultado.imprimir();

    return 0;
}