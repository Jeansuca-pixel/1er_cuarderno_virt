#include <iostream>
#include <numeric> // Para std::gcd (C++17)
using namespace std;

class Fraccion
{
public:
    // Atributos
    int numerador;
    int denominador;

    // Constructor por defecto
    Fraccion(int num = 0, int den = 1)
    {
        numerador = num;
        denominador = (den == 0) ? 1 : den; 
    }

    // Función para calcular el MCD manualmente
    int calcularMCD(int a, int b) {
        a = (a < 0) ? -a : a; // Convierte a positivo
        b = (b < 0) ? -b : b;
        while (b != 0) {
            int temporal = b;
            b = a % b;
            a = temporal;
        }
        return a;
    }

    // Método para simplificar la fracción
    void simplificar()
    {
        int mcd = calcularMCD(numerador, denominador);
        if (mcd > 0) {
            numerador /= mcd;
            denominador /= mcd;
        }

        // Mantener el signo negativo en el numerador
        if (denominador < 0) {
            numerador = -numerador;
            denominador = -denominador;
        }
    }

    // Imprimir
    void imprimir()
    {
        cout << "*************************************" << endl;
        cout << "* " << numerador << "/" << denominador << endl;
        cout << "*************************************" << endl;
    }

    // Suma: (a/b) + (c/d) = (a*d + b*c) / (b*d)
    Fraccion sumar(Fraccion a)
    {
        Fraccion resultado;
        resultado.numerador = (numerador * a.denominador) + (denominador * a.numerador);
        resultado.denominador = denominador * a.denominador;
        resultado.simplificar();
        return resultado;
    }

    // Resta: (a/b) - (c/d) = (a*d - b*c) / (b*d)
    Fraccion restar(Fraccion a)
    {
        Fraccion resultado;
        resultado.numerador = (numerador * a.denominador) - (denominador * a.numerador);
        resultado.denominador = denominador * a.denominador;
        resultado.simplificar();
        return resultado;
    }

    // Multiplicación: (a/b) * (c/d) = (a*c) / (b*d)
    Fraccion multiplicar(Fraccion a)
    {
        Fraccion resultado;
        resultado.numerador = numerador * a.numerador;
        resultado.denominador = denominador * a.denominador;
        resultado.simplificar();
        return resultado;
    }

    // División: (a/b) / (c/d) = (a*d) / (b*c)
    Fraccion dividir(Fraccion a)
    {
        Fraccion resultado;
        if (a.numerador == 0) {
            cout << "Error: No se puede dividir entre una fracción con numerador 0." << endl;
            return resultado;
        }
        resultado.numerador = numerador * a.denominador;
        resultado.denominador = denominador * a.numerador;
        resultado.simplificar();
        return resultado;
    }
};

int main() {
    Fraccion fraccion_a, fraccion_b;

    cout << "Ingrese el numerador de la primera fraccion: ";
    cin >> fraccion_a.numerador;
    cout << "Ingrese el denominador de la primera fraccion: ";
    cin >> fraccion_a.denominador;

    cout << "Ingrese el numerador de la segunda fraccion: ";
    cin >> fraccion_b.numerador;
    cout << "Ingrese el denominador de la segunda fraccion: ";
    cin >> fraccion_b.denominador;

    // Validación básica de denominadores
    if (fraccion_a.denominador == 0 || fraccion_b.denominador == 0) {
        cout << "El denominador no puede ser 0." << endl;
        return 1;
    }

    cout << "\n--- Fracciones Ingresadas ---" << endl;
    fraccion_a.imprimir();
    fraccion_b.imprimir();

    cout << "\n--- Resultados ---" << endl;
    
    cout << "Suma:";
    Fraccion res_suma = fraccion_a.sumar(fraccion_b);
    res_suma.imprimir();

    cout << "Resta:";
    Fraccion res_resta = fraccion_a.restar(fraccion_b);
    res_resta.imprimir();

    cout << "Multiplicacion:";
    Fraccion res_mult = fraccion_a.multiplicar(fraccion_b);
    res_mult.imprimir();

    cout << "Division:";
    Fraccion res_div = fraccion_a.dividir(fraccion_b);
    res_div.imprimir();

    return 0;
}