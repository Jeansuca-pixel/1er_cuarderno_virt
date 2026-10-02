#include <iostream>

using namespace std;

int main() {
    int n;

    cout << "Ingresa un numero entero positivo (n): ";
    cin >> n;

    if (n <= 0) {
        cout << "Por favor, ingresa un numero natural valido (mayor a 0)." << endl;
        return 0;
    }

    long long suma1 = 0;
    long long suma2 = 0;
    long long suma3 = 0;
    // Imprimir el inicio de la serie
    cout << "Resultado: ";

    for (int i = 1; i <= n; i++) {
        suma1 += i;
        cout << i;

        // Si no es el ultimo numero, imprime el signo "+"
        if (i < n) {
            cout << " + ";
        }
        
    }
    // Al finalizar el bucle, imprimimos el resultado acumulado
    cout << " = " << suma1 << endl;
    cout << "Suma de 1 las (n) veces" << endl; 
    for (int i = 1; i <= n; i++){
        suma2 += 1;
        cout<<"1";
        
        if (i < n){
            cout << " + ";
        }
    }
    cout <<" = " << suma2 << endl;
    
    cout << "Suma de los cuadrados (n)" <<endl;
    for (int i = 1;i <= n; i++){
        suma3 +=i*i;
        cout <<i<<"^2";
        
        if (i < n){
            cout << " + ";
        }
    }
    cout <<" = " << suma3 << endl;
    return 0;
}