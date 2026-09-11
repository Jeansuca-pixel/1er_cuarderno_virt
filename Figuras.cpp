#include <iostream>
#include <cmath>
using namespace std;

// === DECLARACIÓN DE CLASES BÁSICAS ===

class Cuadrado {
public:
    double lado = 0;

    double calcularArea() {
        return lado * lado;
    }

    double calcularPerimetro() {
        return lado * 4;
    }
};

class Triangulo {
public:
    double lado = 0;
    double altura = 0;

    double calcularArea() {
        return (lado * altura) / 2.0; // Corregido para usar lado (base) y altura
    }

    double calcularPerimetro() {
        return lado * 3; // Asumiendo triángulo equilátero
    }
};

class Circulo {
public:
    double radio = 0;
    const double PI = 3.14159265358979323846;

    double calcularArea() {
        return PI * radio * radio;
    }

    double calcularPerimetro() {
        return 2 * PI * radio;
    }
};

class Rectangulo {
public:
    double base = 0;
    double altura = 0;

    double calcularArea() {
        return base * altura;
    }

    double calcularPerimetro() {
        return 2 * (base + altura);
    }
};

class Trapecio {
public:
    double baseMayor = 0;
    double baseMenor = 0;
    double altura = 0;

    double calcularArea() {
        return ((baseMayor + baseMenor) / 2.0) * altura;
    }

    double calcularPerimetro() {
        double ladoNoParalelo = sqrt(pow((baseMayor - baseMenor) / 2.0, 2) + pow(altura, 2));
        return baseMayor + baseMenor + 2 * ladoNoParalelo;
    }
};

class Cubo {
public:
    double lado = 0;

    double calcularArea() {
        return 6 * lado * lado;
    }

    double calcularVolumen() {
        return lado * lado * lado;
    }
};
 class Piramide{
 public:
    double base = 0;
    double altura = 0;

    double calcularArea() {
        double apotema = sqrt(pow((base / 2.0), 2) + pow(altura, 2));
        return (base * base) + (2 * base * apotema);
    }

    double calcularVolumen() {
        return (base * base * altura) / 3.0;
    }
 };2
// === FUNCIÓN PRINCIPAL ===

int main() {
    int opcionPrincipal;
    int opcion2D;
    int opcion3D;
    int opcionCuadrado, opcionTriangulo, opcionCirculo, opcionRectangulo, opcionTrapecio, opcionCubo, opcionPiramide;

    Cuadrado miCuadrado;
    Triangulo miTriangulo;
    Circulo miCirculo;
    Rectangulo miRectangulo;
    Trapecio miTrapecio;
    Cubo miCubo;
    Piramide miPiramide;
    do {
        cout << "\n=== MENU PRINCIPAL ===" << endl;
        cout << "1. Figuras 2D" << endl;
        cout << "2. Figuras 3D" << endl;
        cout << "3. Salir" << endl;
        cout << "Selecciona una opcion: ";
        cin >> opcionPrincipal;

        switch (opcionPrincipal) {
            case 1: // --- FIGURAS 2D ---
                do {
                    cout << "\n--- FIGURAS 2D ---" << endl;
                    cout << "1. Cuadrado" << endl;
                    cout << "2. Triangulo" << endl;
                    cout << "3. Circulo" << endl;
                    cout << "4. Rectangulo" << endl;
                    cout << "5. Trapecio" << endl;
                    cout << "0. Regresar al Menu Principal" << endl;
                    cout << "Selecciona una opcion: ";
                    cin >> opcion2D;

                    switch (opcion2D) {
                        case 1: // CUADRADO
                            do {
                                cout << "\n--- MENU CUADRADO ---" << endl;
                                cout << "1. Calcular Area\n2. Calcular Perimetro\n3. Dibujar\n4. Ingresar Lado\n5. Resumen\n6. Regresar\nOpcion: ";
                                cin >> opcionCuadrado;
                                switch (opcionCuadrado) {
                                    case 1: cout << (miCuadrado.lado <= 0 ? "Lado invalido." : "Area: " + to_string(miCuadrado.calcularArea())) << endl; break;
                                    case 2: cout << (miCuadrado.lado <= 0 ? "Lado invalido." : "Perimetro: " + to_string(miCuadrado.calcularPerimetro())) << endl; break;
                                    case 3:
                                        if (miCuadrado.lado <= 0) cout << "Lado invalido." << endl;
                                        else {
                                            for (int i = 0; i < (int)miCuadrado.lado; i++) {
                                                for (int j = 0; j < (int)miCuadrado.lado; j++) cout << "* ";
                                                cout << endl;
                                            }
                                        }
                                        break;
                                    case 4: cout << "Ingrese lado: "; cin >> miCuadrado.lado; break;
                                    case 5: cout << "Lado: " << miCuadrado.lado << " | Area: " << miCuadrado.calcularArea() << " | Perimetro: " << miCuadrado.calcularPerimetro() << endl; break;
                                }
                            } while (opcionCuadrado != 6);
                            break;

                        case 2: // TRIANGULO
                            do {
                                cout << "\n--- MENU TRIANGULO ---\n1. Area\n2. Perimetro\n3. Dibujar\n4. Ingresar Datos\n5. Resumen\n6. Regresar\nOpcion: ";
                                cin >> opcionTriangulo;
                                switch (opcionTriangulo) {
                                    case 1: cout << "Area: " << miTriangulo.calcularArea() << endl; break;
                                    case 2: cout << "Perimetro: " << miTriangulo.calcularPerimetro() << endl; break;
                                    case 3:
                                        for (int i = 0; i < (int)miTriangulo.lado; i++) {
                                            for (int j = 0; j <= i; j++) cout << "* ";
                                            cout << endl;
                                        }
                                        break;
                                    case 4: 
                                        cout << "Base/Lado: "; cin >> miTriangulo.lado;
                                        cout << "Altura: "; cin >> miTriangulo.altura;
                                        break;
                                    case 5: cout << "Area: " << miTriangulo.calcularArea() << " | Perimetro: " << miTriangulo.calcularPerimetro() << endl; break;
                                }
                            } while (opcionTriangulo != 6);
                            break;

                        case 3: // CÍRCULO
                            do {
                                cout << "\n--- MENU CIRCULO ---\n1. Area\n2. Perimetro\n3. Dibujar\n4. Ingresar Radio\n5. Resumen\n6. Regresar\nOpcion: ";
                                cin >> opcionCirculo;
                                switch (opcionCirculo) {
                                    case 1: cout << "Area: " << miCirculo.calcularArea() << endl; break;
                                    case 2: cout << "Perimetro: " << miCirculo.calcularPerimetro() << endl; break;
                                    case 3: {
                                        int r = (int)round(miCirculo.radio);
                                        for (int y = -r; y <= r; y++) {
                                            for (int x = -r; x <= r; x++) {
                                                cout << (x*x + y*y <= r*r ? "* " : "  ");
                                            }
                                            cout << endl;
                                        }
                                        break;
                                    }
                                    case 4: cout << "Radio: "; cin >> miCirculo.radio; break;
                                    case 5: cout << "Area: " << miCirculo.calcularArea() << " | Perimetro: " << miCirculo.calcularPerimetro() << endl; break;
                                }
                            } while (opcionCirculo != 6);
                            break;

                        case 4: // RECTÁNGULO
                            do {
                                cout << "\n--- MENU RECTANGULO ---\n1. Area\n2. Perimetro\n3. Dibujar\n4. Ingresar Datos\n5. Resumen\n6. Regresar\nOpcion: ";
                                cin >> opcionRectangulo;
                                switch (opcionRectangulo) {
                                    case 1: cout << "Area: " << miRectangulo.calcularArea() << endl; break;
                                    case 2: cout << "Perimetro: " << miRectangulo.calcularPerimetro() << endl; break;
                                    case 3:
                                        for (int i = 0; i < (int)miRectangulo.altura; i++) {
                                            for (int j = 0; j < (int)miRectangulo.base; j++) cout << "* ";
                                            cout << endl;
                                        }
                                        break;
                                    case 4:
                                        cout << "Base: "; cin >> miRectangulo.base;
                                        cout << "Altura: "; cin >> miRectangulo.altura;
                                        break;
                                    case 5: cout << "Area: " << miRectangulo.calcularArea() << " | Perimetro: " << miRectangulo.calcularPerimetro() << endl; break;
                                }
                            } while (opcionRectangulo != 6);
                            break;

                        case 5: // TRAPECIO
                            do {
                                cout << "\n--- MENU TRAPECIO ---\n1. Area\n2. Perimetro\n3. Ingresar Datos\n4. Resumen\n5. Regresar\nOpcion: ";
                                cin >> opcionTrapecio;
                                switch (opcionTrapecio) {
                                    case 1: cout << "Area: " << miTrapecio.calcularArea() << endl; break;
                                    case 2: cout << "Perimetro: " << miTrapecio.calcularPerimetro() << endl; break;
                                    case 3:
                                        cout << "Base Mayor: "; cin >> miTrapecio.baseMayor;
                                        cout << "Base Menor: "; cin >> miTrapecio.baseMenor;
                                        cout << "Altura: "; cin >> miTrapecio.altura;
                                        break;
                                    case 4: cout << "Area: " << miTrapecio.calcularArea() << " | Perimetro: " << miTrapecio.calcularPerimetro() << endl; break;
                                }
                            } while (opcionTrapecio != 5);
                            break;
                    }
                } while (opcion2D != 0);
                break;

            case 2: // --- FIGURAS 3D ---
                do {
                    cout << "\n--- FIGURAS 3D ---\n1. Cubo\n2. Piramide\n0. Regresar\nOpcion: ";
                    cin >> opcion3D;
                    if (opcion3D == 1) {
                        do {
                            cout << "\n--- MENU CUBO ---\n1. Area Superficial\n2. Volumen\n3. Ingresar Lado\n4. Resumen\n5. Regresar\nOpcion: ";
                            cin >> opcionCubo;
                            switch (opcionCubo) {
                                case 1: cout << "Area: " << miCubo.calcularArea() << endl; break;
                                case 2: cout << "Volumen: " << miCubo.calcularVolumen() << endl; break;
                                case 3: cout << "Lado: "; cin >> miCubo.lado; break;
                                case 4: cout << "Area: " << miCubo.calcularArea() << " | Volumen: " << miCubo.calcularVolumen() << endl; break;
                            }
                        } while (opcionCubo != 5);
                    }
                    else if (opcion3D == 2) {
                        do {
                            cout << "\n--- MENU PIRAMIDE ---\n1. Area Superficial\n2. Volumen\n3. Ingresar Datos\n4. Resumen\n5. Regresar\nOpcion: ";
                            cin >> opcionPiramide;
                            switch (opcionPiramide) {
                                case 1: cout << "Area: " << miPiramide.calcularArea() << endl; break;
                                case 2: cout << "Volumen: " << miPiramide.calcularVolumen() << endl; break;
                                case 3: cout << "Base: "; cin >> miPiramide.base; cout << "Altura: "; cin >> miPiramide.altura; break;
                                case 4: cout << "Area: " << miPiramide.calcularArea() << " | Volumen: " << miPiramide.calcularVolumen() << endl; break;
                            }
                        } while (opcionPiramide != 5);
                    }
                } while (opcion3D != 0);
                break;

            case 3:
                cout << "Saliendo del programa..." << endl;
                break;

            default:
                cout << "Opcion invalida." << endl;
                break;
        }
    } while (opcionPrincipal != 3);

    return 0;
}