#include <iostream>
using namespace std;

int main() {
    int n;
    int A[100];  // tamaño máximo 100

    cout << "Ingresa n: ";
    cin >> n;

    if (n > 100) {3
        cout << "n es demasiado grande" << endl;
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        A[i] = i;
    }

    for (int j = 0; j < 4; j++) {
        cout << A[j] << ",";
    }

    return 0;
}