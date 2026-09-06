#include <iostream>
using namespace std;

int main() {
    int n;

    cout << "Ingresa n: ";
    cin >> n;

    int* A = new int[n];

    for (int i = 0; i < n; i++) {
        A[i] = i + 1;
    }

    for (int i = 0; i < n; i++) {
        cout << A[i] << endl;
    }

    delete[] A;
    return 0;
}