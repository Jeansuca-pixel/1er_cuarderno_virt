#include <iostream>
using namespace std;

int main()
{
    int n, x;
    cin >> n;
    int A[1000];
    for (int i = 1; i <= n; i++)
    {
        A[i] = i * 10;
    }
    for (int i = 1; i <= n; i++)
    {
        cout << A[i] << " ";
    }

    cout <<endl<< "ingrese el valor que busca: ";
    cin >> x;
    int posicion = -1;
    for (int k = 1; k <= n; k++)
    {
        
        if (x == A[k])
        {
            posicion = k;
            break;
        }
    }

    if (posicion != -1)
        cout << "esta en la posicion: " << posicion;
    else
        cout << "no encontrado";

    return 0;
}