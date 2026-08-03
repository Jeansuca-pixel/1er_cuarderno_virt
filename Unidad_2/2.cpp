#include <iostream>
using namespace std;

int main() {
    int A[5] = {10, 20, 50, 40, 30};
    int suma;
    int Mayor = A[0];
   int Menor = A[0];
    for (int i = 1; i < 5; i++) {
        if (Mayor < A[i]) {
            Mayor = A[i];
        }
    }
     for (int i = 1; i < 5; i++) {
        if (Menor > A[i]) {
            Menor = A[i];
        }
    }
    cout << "Menor = " << Mayor<< endl;
    cout << "Mayor = " << Menor<<endl;
    suma= Mayor+Menor;
    cout <<"Menor + Mayor = "<< suma;

    return 0;
}