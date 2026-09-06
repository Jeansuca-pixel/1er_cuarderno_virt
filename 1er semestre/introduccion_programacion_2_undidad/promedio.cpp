#include <iostream>
using namespace std;

int main()
{
    int A[5] = {10,20,50,40,30};
    int suma = 0;
    float promedio = 0;
    for(int i=0; i<5; i++)
    {
        suma += A[i];
    }
    promedio = suma / 5.0;
    cout << "El promedio es: " << promedio << endl;
    return 0;
}