#include <iostream>
using namespace std;

int main()
{
    int mayor=0, menor=9999999;
    int A[5] = {10,20,50,40,30};

    int posicion1 = -1;
    int posicion2 = -1;
    for (int i = 0; i < 5; i++)
    {
        if (A[i] > mayor)
        {
            mayor = A[i];
            posicion1 = i+1;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        if (A[i] < menor)
        {
            menor = A[i];
            posicion2 = i+1;
        }
    }

    cout << "El mayor esta en la posicion: " << posicion1<< " y es " << mayor << endl;

    cout << "El menor esta en la posicion: " << posicion2<< " y es " << menor << endl;
    cout << "la suma del mayor y el menor es: " << mayor + menor << endl;
    cout << "la resta del mayor y el menor es: " << mayor - menor << endl;

    return 0;
}