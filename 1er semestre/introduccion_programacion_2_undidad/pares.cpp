#include <iostream>
using namespace std;

int main()
{
    int A[5] = {3,6,2,4,7};
    int P[5], I[5],Pp[5], Pi[5];
    int pares = 0;
    int impares = 0;
    for(int i=0; i<5; i++)
    {
        if(A[i]%2==0)
        {
            Pp[pares] = i;
            P[pares] = A[i];
            pares++;
        }
        else
        {
            Pi[impares] = i;
            I[impares] = A[i];
            impares++;
        }
    }
    cout << "Cantidad de pares: " << pares << endl;
    cout << "Cantidad de impares: " << impares << endl;
    cout << "Pares:"<< endl;
    for(int i=0; i<pares; i++)
    {
        cout <<"el numero: " << P[i] << " esta en la posicion: " << Pp[i]+1 << endl;
    }
    cout << endl;
    cout << "Impares:" << endl;
    for(int i=0; i<impares; i++)
    {
        cout <<"el numero: " << I[i] << " esta en la posicion: " << Pi[i]+1 << endl;
    }
    cout << endl<<endl;
    return 0;
}