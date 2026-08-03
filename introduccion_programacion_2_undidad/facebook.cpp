#include <iostream>
using namespace std;  
int main() {
    int n,cont;
    cout << "Ingrese el limite: ";
    cin >> n;
    int Ar[n];
    for (int i = 0; i < n; i++) {
        cout << "Ingrese los valores del array: ";
        cin >> Ar[i];
    }
    cout << "la frecuencia de cada elemento es:" << endl;
    for (int i = 0; i < n; i++)
    {
        cont = 1;
        if (Ar[i] != -1){
        for (int j = i+1; j < n; j++)
        {
            if (Ar[i] == Ar[j])
            {
                cont++;
                Ar[j] = -1;
            }
            
        }
        cout << "El elemento " << Ar[i] << " aparece " << cont << " veces." << endl;
    }
       
    }
    return 0;
}