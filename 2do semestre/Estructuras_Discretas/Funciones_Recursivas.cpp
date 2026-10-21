#include <iostream>
using namespace std;
// 1. El Factorial de un número n   n*n!
long long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
// 2. Números triangulares  n*(n+1)/2
int numeroTriangular(int k) {
    if (k == 1) {
        return 1;
    }
    return k + numeroTriangular(k - 1);
}
void imprimirTriangulares(int n, int actual = 1) {
    if (actual > n) return;
    cout << numeroTriangular(actual) << " ";
    imprimirTriangulares(n, actual + 1);
}
// 3. Números armónicos   1+(1/2)+(1/3)....+(1/n)
double numeroHarmonico(int k) {
    if (k == 1) {
        return 1.0;
    }
    return (1 / k) + numeroHarmonico(k - 1);
}
void imprimirHarmonicos(int n, int actual = 1) {
    if (actual > n) return;
    cout << numeroHarmonico(actual) << " ";
    imprimirHarmonicos(n, actual + 1);
}
// 4. Números Fibonacci   0,1,1,2,3,5.....
int fibonacci(int k) {
    if (k == 0) return 0;
    if (k == 1) return 1;
    return fibonacci(k - 1) + fibonacci(k - 2);
}
void imprimirFibonacci(int n, int actual = 0) {
    if (actual >= n) return;
    cout << fibonacci(actual) << " ";
    imprimirFibonacci(n, actual + 1);
}
// 5. Números Lucas a partir de números Fibonacci 2,1,3,4,7....
int numeroLucas(int k) {
    if (k == 0) return 2;
    if (k == 1) return 1;
    return fibonacci(k - 1) + fibonacci(k + 1);
}
void imprimirLucas(int n, int actual = 0) {
    if (actual >= n) return;
    cout << numeroLucas(actual) << " ";
    imprimirLucas(n, actual + 1);
}

int main() {
    int n = 4; 
    cout << "  EJERCICIOS DE RECURSIVIDAD EN C++"<< endl;
    // 1. Factorial
    cout << "1. Factorial de " << n << ":"<< endl;
    cout << n << "! = " << factorial(n) << endl;
    // 2. Números Triangulares
    cout << "2. Los " << n << " primeros numeros triangulares:"<< endl;
    imprimirTriangulares(n); cout<< endl;
    // 3. Números Armónicos
    cout << "3. Los " << n << " primeros numeros armonicos:"<<endl;
    imprimirHarmonicos(n); cout <<endl;
    // 4. Números Fibonacci
    cout << "4. Los " << n << " primeros numeros Fibonacci:"<< endl;
    imprimirFibonacci(n); cout << endl;
    // 5. Números Lucas
    cout << "5. Los " << n << " primeros numeros de Lucas a partir de Fibonacci:"<< endl;
    imprimirLucas(n); cout <<endl;
    return 0;
}