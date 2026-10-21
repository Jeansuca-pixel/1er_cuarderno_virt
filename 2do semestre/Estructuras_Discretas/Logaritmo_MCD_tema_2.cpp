//Alumno:Jean Andre Sucari Chambi           Codigo:260238
//La IA me ayudo a comprender en un 30% ya que se me hiso un 
//poco complicado programar en el 1ero y 3ero, el 70% yo hice yo
#include <iostream>
using namespace std;
// 1. Exponenciación vía cuadrado repetido 
long long exp_via_cuadrado_repetido(long long a, long long n) {
    long long resultado = 1;
    long long x = a;
    while (n > 0) {
        if (n % 2 == 1) {
            resultado = resultado * x;
        }
        x = x * x;
        n = n / 2; // División entera equivalente a piso de n/2
    }
    return resultado;
}
// 2. Encuentra s y t tales que MCD(a, b) = sa + tb
long long mcdExtendido(long long a, long long b, long long &s, long long &t) {
    if (b == 0) {
        s = 1;
        t = 0;
        return a;
    }
    long long s1, t1;
    long long mcd = mcdExtendido(b, a % b, s1, t1);
    s = t1;
    t = s1 - (a / b) * t1;
    return mcd;
}
// 3. Cálculo del inverso de n mod phi
long long inversoModular(long long n, long long phi) {
    long long s, t;
    long long mcd = mcdExtendido(n, phi, s, t);
    if (mcd != 1) {
        return -1; // No existe inverso si no son coprimos
    }
    // Asegurar que el resultado sea positivo en el rango [0, phi - 1]
    return (s % phi + phi) % phi;
}
int main() {
    cout << "EJERCICIOS DE ALGORITMOS Y MATEMATICA" << endl;
    // Prueba 1: 
    long long a = 3, n_exp = 13;
    cout << "1. Exponenciacion rapida (Algoritmo 3):\n";
    cout << "   " << a << "^" << n_exp << " = " << exp_via_cuadrado_repetido(a, n_exp) << endl;
    // Prueba 2: 
    long long a_mcd = 30, b_mcd = 20;
    long long s, t;
    long long mcd = mcdExtendido(a_mcd, b_mcd, s, t);
    cout << "2. MCD Extendido para " << a_mcd << " y " << b_mcd << ":"<<endl;
    cout << "MCD = " << mcd << ", s = " << s << ", t = " << t << endl;
    cout << "Verificacion (" << s << "*" << a_mcd << " + " << t << "*" << b_mcd << ") = " 
              << (s * a_mcd + t * b_mcd) << endl;
    // Prueba 3: 
    long long n_inv = 3, phi = 11;
    long long inv = inversoModular(n_inv, phi);
    cout << "3. Inverso modular de " << n_inv << " mod " << phi << ":"<< endl;
    if (inv != -1) {
        cout << "Inverso = " << inv << " (Verificacion: (" << n_inv << " * " << inv << ") mod " 
                  << phi << " = " << (n_inv * inv) % phi << ")"<< endl;
    } else {
        cout << "No existe inverso modular porque no son coprimos."<< endl;
    }
    return 0;
}