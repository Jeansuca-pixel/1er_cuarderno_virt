#include <iostream>
#include <cmath>
using namespace std;

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
    double base = 0;
    double altura = 0;

    double calcularArea() {
        return (base * altura) / 2;
    }

    double calcularPerimetro() {
        return base * 3; // Triángulo equilátero
    }
};

class Circulo {
public:
    double radio = 0;

    double calcularArea() {
        return 3.1415 * radio * radio;
    }

    double calcularPerimetro() {
        return 2 * 3.1415 * radio;
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

class Pentagono {
public:
    double lado = 0;
    double apotema = 0;

    double calcularArea() {
        return (5 * lado * apotema) / 2;
    }

    double calcularPerimetro() {
        return 5 * lado;
    }
};

class Hexagono {
public:
    double lado = 0;

    double calcularArea() {
        return (3 * sqrt(3) * lado * lado) / 2;
    }

    double calcularPerimetro() {
        return 6 * lado;
    }
};

class Heptagono {
public:
    double lado = 0;
    double apotema = 0;

    double calcularArea() {
        return (7 * lado * apotema) / 2;
    }

    double calcularPerimetro() {
        return 7 * lado;
    }
};

class Octagono {
public:
    double lado = 0;

    double calcularArea() {
        return 2 * (1 + sqrt(2)) * lado * lado;
    }

    double calcularPerimetro() {
        return 8 * lado;
    }
}; 
class Nonagono {
public:
    double lado = 0;
    double apotema = 0;

    double calcularArea() {
        return (9 * lado * apotema) / 2;
    }

    double calcularPerimetro() {
        return 9 * lado;
    }
};

class Decagono {
public:
    double lado = 0;
    double apotema = 0;

    double calcularArea() {
        return (10 * lado * apotema) / 2;
    }

    double calcularPerimetro() {
        return 10 * lado;
    }
};

class Icosagono {
public:
    double lado = 0;
    double apotema = 0;

    double calcularArea() {
        return (20 * lado * apotema) / 2;
    }

    double calcularPerimetro() {
        return 20 * lado;
    }
};
int main() {
    int opcionPrincipal;
    int opcion2D;
    int opcionCuadrado;
    int opcionTriangulo; 

    Cuadrado miCuadrado;
    Triangulo miTriangulo;
    Circulo miCirculo;
    Rectangulo miRectangulo;
    Pentagono miPentagono;
    Hexagono miHexagono;
    Heptagono miHeptagono;
    Octagono miOctagono;
    Nonagono miNonagono;
    Decagono miDecagono;
    Icosagono miIcosagono;
    do {
        // --- MENÚ PRINCIPAL ---
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
                    cout << "5. Pentagono" << endl;
                    cout << "6. Hexagono" << endl;
                    cout << "7. Heptagono" << endl;
                    cout << "8. Octagono" << endl;
                    cout << "9. Nonagono" << endl;
                    cout << "10. Decagono" << endl;
                    cout << "11. Icosagono" << endl;
                    cout << "0. Regresar al Menu Principal" << endl;
                    cout << "Selecciona una opcion: ";
                    cin >> opcion2D;

                    switch (opcion2D) {
                        case 1: // --- SUBMENÚ CUADRADO ---
                            do {
                                cout << "\n--- MENU CUADRADO ---" << endl;
                                cout << "1. Calcular y mostrar Area" << endl;
                                cout << "2. Calcular y mostrar Perimetro" << endl;
                                cout << "3. Dibujar" << endl;
                                cout << "4. Ingresar / Cambiar el lado del cuadrado" << endl;
                                cout << "5. Resumen (Opciones 1, 2 y 3)" << endl;
                                cout << "6. Regresar al submenu de Figuras 2D" << endl;
                                cout << "Selecciona una opcion: ";
                                cin >> opcionCuadrado;

                                switch (opcionCuadrado) {
                                    case 1:
                                        if (miCuadrado.lado <= 0) {
                                            cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "El Area del cuadrado es: " << miCuadrado.calcularArea() << endl;
                                        }
                                        break;
                                        
                                    case 2:
                                        if (miCuadrado.lado <= 0) {
                                            cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "El Perimetro del cuadrado es: " << miCuadrado.calcularPerimetro() << endl;
                                        }
                                        break;
                                        
                                    case 3: // Dibujar
                                        if (miCuadrado.lado <= 0) {
                                            cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "\n";
                                            for (int i = 0; i < static_cast<int>(miCuadrado.lado); i++) {   //busca que es static_cast
                                                for (int j = 0; j < static_cast<int>(miCuadrado.lado); j++) {
                                                    cout << "* ";
                                                }
                                                cout << endl;
                                            }
                                        }
                                        break;
                                    
                                    case 4:
                                            cout << "Introduce el valor del lado: ";
                                            cin >> miCuadrado.lado;
                                            cout << "¡Lado actualizado con exito!" << endl;
                                            break;
                                        
                                    case 5:
                                        if (miCuadrado.lado <= 0) {
                                            cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "\n=== RESUMEN DEL CUADRADO ===" << endl;
                                            cout << "Lado: " << miCuadrado.lado << endl;
                                            cout << "Area: " << miCuadrado.calcularArea() << endl;
                                            cout << "Perimetro: " << miCuadrado.calcularPerimetro() << endl;
                                        }
                                        break;
                                    
                                    case 6:
                                        cout << "Regresando al submenu de Figuras 2D..." << endl;
                                        break;
                                    
                                    default:
                                        cout << "Opcion no valida. Intenta de nuevo." << endl;
                                        break;
                                }
                            } while (opcionCuadrado != 6);
                            break;

                        case 2: 
                            do { // --- TRIÁNGULO ---
                                cout << "\n--- MENU TRIANGULO ---" << endl;
                                cout << "1. Calcular y mostrar Area" << endl;
                                cout << "2. Calcular y mostrar Perimetro" << endl;
                                cout << "3. Dibujar" << endl;
                                cout << "4. Ingresar / Cambiar el lado del triangulo" << endl;
                                cout << "5. Resumen (Opciones 1, 2 y 3)" << endl;
                                cout << "6. Regresar al submenu de Figuras 2D" << endl;
                                cout << "Selecciona una opcion: ";
                                cin >> opcionTriangulo;
                            } while (opcionTriangulo != 6);
                            break;
                            switch (opcionTriangulo)
                            {
                                    case 1:
                                    if (miTriangulo.lado <= 0) {
                                            cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "El Area del triangulo es: " << miTriangulo.calcularArea() << endl;
                                        }
                                        break;
                                        
                                    case 2:
                                        if (miTriangulo.lado <= 0) {
                                            cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "El Perimetro del triangulo es: " << miTriangulo.calcularPerimetro() << endl;
                                        }
                                        break;
                                    case 3:
                                        if (miTriangulo.lado <= 0) {
                                            cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "\n";
                                            for (int i = 0; i < static_cast<int>(miTriangulo.lado); i++) {
                                                for (int j = 0; j < static_cast<int>(miTriangulo.lado); j++) {
                                                    cout << "* ";
                                                }
                                                cout << endl;
                                            }
                                        }
                                        break;
                                    case 4:
                                        cout << "Introduce el valor del lado: ";
                                        cin >> miTriangulo.lado;
                                        cout << "¡Lado actualizado con exito!" << endl;
                                        break;
                                    case 5:
                                        if (miTriangulo.lado <= 0) {
                                            cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "\n=== RESUMEN DEL TRIANGULO ===" << endl;
                                            cout << "Lado: " << miTriangulo.lado << endl;
                                            cout << "Area: " << miTriangulo.calcularArea() << endl;
                                            cout << "Perimetro: " << miTriangulo.calcularPerimetro() << endl;
                                        }
                                        break;
                                    case 6:
                                        cout << "Regresando al submenu de Figuras 2D..." << endl;
                                        break;
                                    
                                    default:
                                        cout << "Opcion no valida. Intenta de nuevo." << endl;
                                        break;    

                                    default:
                                        break;
                                    }
                        case 3: { // --- CÍRCULO ---
                            Circulo miCirculo;
                            cout << "\nIntroduce el radio del circulo: ";
                            cin >> miCirculo.radio;
                            cout << "Area: " << miCirculo.calcularArea() << endl;
                            cout << "Perimetro: " << miCirculo.calcularPerimetro() << endl;
                            break;
                        }

                        case 4: { // --- RECTÁNGULO ---
                            Rectangulo miRectangulo;
                            cout << "\nIntroduce la base: ";
                            cin >> miRectangulo.base;
                            cout << "Introduce la altura: ";
                            cin >> miRectangulo.altura;
                            cout << "Area: " << miRectangulo.calcularArea() << endl;
                            cout << "Perimetro: " << miRectangulo.calcularPerimetro() << endl;
                            break;
                        }

                        case 5: { // --- PENTÁGONO ---
                            Pentagono miPentagono;
                            cout << "\nIntroduce el lado: ";
                            cin >> miPentagono.lado;
                            cout << "Introduce la apotema: ";
                            cin >> miPentagono.apotema;
                            cout << "Area: " << miPentagono.calcularArea() << endl;
                            cout << "Perimetro: " << miPentagono.calcularPerimetro() << endl;
                            break;
                        }

                        case 6: { // --- HEXÁGONO ---
                            Hexagono miHexagono;
                            cout << "\nIntroduce el lado: ";
                            cin >> miHexagono.lado;
                            cout << "Area: " << miHexagono.calcularArea() << endl;
                            cout << "Perimetro: " << miHexagono.calcularPerimetro() << endl;
                            break;
                        }

                        case 7: { // --- HEPTÁGONO ---
                            Heptagono miHeptagono;
                            cout << "\nIntroduce el lado: ";
                            cin >> miHeptagono.lado;
                            cout << "Introduce la apotema: ";
                            cin >> miHeptagono.apotema;
                            cout << "Area: " << miHeptagono.calcularArea() << endl;
                            cout << "Perimetro: " << miHeptagono.calcularPerimetro() << endl;
                            break;
                        }

                        case 0:
                            cout << "Regresando al Menu Principal..." << endl;
                            break;

                        default:
                            cout << "Opcion no valida. Intenta de nuevo." << endl;
                            break;
                    }
                } while (opcion2D != 0); // Modificado a 0 para coincidir con la opción del menú
                break;

            case 2: // --- FIGURAS 3D ---
                cout << "\n--- FIGURAS 3D ---" << endl;
                cout << "Seccion en construccion..." << endl;
                break;

            case 3:
                cout << "¡Gracias por usar el programa! Saliendo..." << endl;
                break;

            default:
                cout << "Opcion no valida. Intenta de nuevo." << endl;
                break;
        }

    } while (opcionPrincipal != 3);

    return 0;
}