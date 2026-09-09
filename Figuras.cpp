#include <iostream>
#include <cmath>
using namespace std;

// === DECLARACIÓN DE CLASES BÁSICAS (SIN ENCAPSULAMIENTO) ===

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

    double getLado() {
        return lado;
    }

    double calcularArea() {
        return (sqrt(3) / 4) * lado * lado;
    }

    double calcularPerimetro() {
        return lado * 3;
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

class trapecio {
public:
    double baseMayor = 0;
    double baseMenor = 0;
    double altura = 0;

    double calcularArea() {
        return ((baseMayor + baseMenor) / 2) * altura;
    }

    double calcularPerimetro() {
        // Asumiendo que los lados no paralelos son iguales
        double ladoNoParalelo = sqrt(pow((baseMayor - baseMenor) / 2, 2) + pow(altura, 2));
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
// === FUNCIÓN PRINCIPAL ===

int main() {
    int opcionPrincipal;
    int opcion2D;
    int opcion3D;
    int opcionCuadrado;
    int opcionTriangulo; 
    int opcionCirculo;
    int opcionRectangulo;
    int opcionTrapecio;
    int opcionCubo;
    Cuadrado miCuadrado;
    Triangulo miTriangulo;
    Circulo miCirculo;
    Rectangulo miRectangulo;
    trapecio miTrapecio;
    Cubo miCubo;

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
                    cout << "5. Trapecio" << endl;
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
                                        
                                    case 3:
                                        if (miCuadrado.lado <= 0) {
                                            cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "\n";
                                            for (int i = 0; i < (miCuadrado.lado); i++) {
                                                for (int j = 0; j <(miCuadrado.lado); j++) {
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

                        case 2: // --- SUBMENÚ TRIÁNGULO ---
                            do {
                                cout << "\n--- MENU TRIANGULO ---" << endl;
                                cout << "1. Calcular y mostrar Area" << endl;
                                cout << "2. Calcular y mostrar Perimetro" << endl;
                                cout << "3. Dibujar" << endl;
                                cout << "4. Ingresar / Cambiar el lado y altura del triangulo" << endl;
                                cout << "5. Resumen (Opciones 1, 2 y 3)" << endl;
                                cout << "6. Regresar al submenu de Figuras 2D" << endl;
                                cout << "Selecciona una opcion: ";
                                cin >> opcionTriangulo;

                                switch (opcionTriangulo) {
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
                                            for (int i = 0; i < miTriangulo.lado; i++) {
                                                for (int j = 0; j <= i; j++) {
                                                    cout << "* ";
                                                }
                                                cout << endl;
                                            }
                                        }
                                        break;

                                    case 4:
                                        cout << "Introduce el valor del lado: ";
                                        cin >> miTriangulo.lado;
                                        cout << "Introduce el valor de la altura: ";
                                        cin >> miTriangulo.altura;
                                        cout << "¡Lado y altura actualizados con exito!" << endl;
                                        break;

                                    case 5:
                                        if (miTriangulo.lado <= 0) {
                                            cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "\n=== RESUMEN DEL TRIANGULO ===" << endl;
                                            cout << "Lado: " << miTriangulo.lado << endl;
                                            cout << "Altura: " << miTriangulo.altura << endl;
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
                                }
                            } while (opcionTriangulo != 6);
                            break;

                        case 3: // --- SUBMENÚ CÍRCULO ---
                            do {
                                cout << "\n--- MENU CIRCULO ---" << endl;
                                cout << "1. Calcular y mostrar Area" << endl;
                                cout << "2. Calcular y mostrar Perimetro" << endl;
                                cout << "3. Dibujar" << endl;
                                cout << "4. Ingresar / Cambiar el radio del circulo" << endl;
                                cout << "5. Resumen (Opciones 1, 2 y 3)" << endl;
                                cout << "6. Regresar al submenu de Figuras 2D" << endl;
                                cout << "Selecciona una opcion: ";
                                cin >> opcionCirculo;

                                switch (opcionCirculo) {
                                    case 1:
                                        if (miCirculo.radio <= 0) {
                                            cout << "Primero debes ingresar un radio valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "El Area del circulo es: " << miCirculo.calcularArea() << endl;
                                        }
                                        break;

                                    case 2:
                                        if (miCirculo.radio <= 0) {
                                            cout << "Primero debes ingresar un radio valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "El Perimetro del circulo es: " << miCirculo.calcularPerimetro() << endl;
                                        }
                                        break;

                                    case 3:
                                        if (miCirculo.radio <= 0) {
                                            cout << "Primero debes ingresar un radio valido (Opcion 4)." << endl;
                                        } else {
                                            // Dibujar círculo aproximado usando caracteres
                                            int r = (int) round(miCirculo.radio);
                                            for (int y = -r; y <= r; y++) {
                                                for (int x = -r; x <= r; x++) {
                                                    if (x * x + y * y <= r * r) {
                                                        cout << "* ";
                                                    } else {
                                                        cout << "  ";
                                                    }
                                                }
                                                cout << endl;
                                            }
                                        }
                                        break;

                                    case 4:
                                        cout << "Introduce el valor del radio: ";
                                        cin >> miCirculo.radio;
                                        cout << "¡Radio actualizado con exito!" << endl;
                                        break;

                                    case 5:
                                        if (miCirculo.radio <= 0) {
                                            cout << "Primero debes ingresar un radio valido (Opcion 4)." << endl;
                                        } else {
                                            cout << "\n=== RESUMEN DEL CIRCULO ===" << endl;
                                            cout << "Radio: " << miCirculo.radio << endl;
                                            cout << "Area: " << miCirculo.calcularArea() << endl;
                                            cout << "Perimetro: " << miCirculo.calcularPerimetro() << endl;
                                        }
                                        break;

                                    case 6:
                                        cout << "Regresando al submenu de Figuras 2D..." << endl;
                                        break;

                                    default:
                                        cout << "Opcion no valida. Intenta de nuevo." << endl;
                                        break;
                                }
                            } while (opcionCirculo != 6);
                            break;

                        case 4: // --- SUBMENÚ RECTÁNGULO ---
                            do {
                                cout << "\n--- MENU RECTANGULO ---" << endl;
                                cout << "1. Calcular y mostrar Area" << endl;
                                cout << "2. Calcular y mostrar Perimetro" << endl;
                                cout << "3. Dibujar" << endl;
                                cout << "4. Ingresar / Cambiar la base y altura del rectangulo" << endl;
                                cout << "5. Resumen (Opciones 1, 2 y 3)" << endl;
                                cout << "6. Regresar al submenu de Figuras 2D" << endl;
                                cout << "Selecciona una opcion: ";
                                cin >> opcionRectangulo;

                                switch (opcionRectangulo) {
                                    case 1:
                                        if (miRectangulo.base <= 0 || miRectangulo.altura <= 0) {
                                            cout << "Primero debes ingresar una base y altura validas (Opcion 4)." << endl;
                                        } else {
                                            cout << "El Area del rectangulo es: " << miRectangulo.calcularArea() << endl;
                                        }
                                        break;

                                    case 2:
                                        if (miRectangulo.base <= 0 || miRectangulo.altura <= 0) {
                                            cout << "Primero debes ingresar una base y altura validas (Opcion 4)." << endl;
                                        } else {
                                            cout << "El Perimetro del rectangulo es: " << miRectangulo.calcularPerimetro() << endl;
                                        }
                                        break;

                                    case 3:
                                        if (miRectangulo.base <= 0 || miRectangulo.altura <= 0) {
                                            cout << "Primero debes ingresar una base y altura validas (Opcion 4)." << endl;
                                        } else {
                                            cout << "\n";
                                            for (int i = 0; i < miRectangulo.altura; i++) {
                                                for (int j = 0; j < miRectangulo.base; j++) {
                                                    cout << "* ";
                                                }
                                                cout << endl;
                                            }
                                        }
                                        break;

                                    case 4:
                                        cout << "Introduce el valor de la base: ";
                                        cin >> miRectangulo.base;
                                        cout << "Introduce el valor de la altura: ";
                                        cin >> miRectangulo.altura;
                                        cout << "¡Base y altura actualizadas con exito!" << endl;
                                        break;

                                    case 5:
                                        if (miRectangulo.base <= 0 || miRectangulo.altura <= 0) {
                                            cout << "Primero debes ingresar una base y altura validas (Opcion 4)." << endl;
                                        } else {
                                            cout << "\n=== RESUMEN DEL RECTANGULO ===" << endl;
                                            cout << "Base: " << miRectangulo.base << endl;
                                            cout << "Altura: " << miRectangulo.altura << endl;
                                            cout << "Area: " << miRectangulo.calcularArea() << endl;
                                            cout << "Perimetro: " << miRectangulo.calcularPerimetro() << endl;
                                        }
                                        break;

                                    case 6:
                                        cout << "Regresando al submenu de Figuras 2D..." << endl;
                                        break;

                                    default:
                                        cout << "Opcion no valida. Intenta de nuevo." << endl;
                                        break;
                                }
                            } while (opcionRectangulo != 6);
                            break;
                        case 5: // --- TRAPECIO ---
                            do {
                                cout << "\n--- MENU TRAPECIO ---" << endl;
                                cout << "1. Calcular y mostrar Area" << endl;
                                cout << "2. Calcular y mostrar Perimetro" << endl;
                                cout << "3. Ingresar / Cambiar la base mayor, base menor y altura del trapecio" << endl;
                                cout << "4. Resumen (Opciones 1, 2)" << endl;
                                cout << "5. Regresar al submenu de Figuras 2D" << endl;
                                cout << "Selecciona una opcion: ";
                                cin >> opcionTrapecio;

                                switch (opcionTrapecio) {
                                    case 1:
                                        if (miTrapecio.baseMayor <= 0 || miTrapecio.baseMenor <= 0 || miTrapecio.altura <= 0) {
                                            cout << "Primero debes ingresar bases y altura validas (Opcion 3)." << endl;
                                        } else {
                                            cout << "El Area del trapecio es: " << miTrapecio.calcularArea() << endl;
                                        }
                                        break;

                                    case 2:
                                        if (miTrapecio.baseMayor <= 0 || miTrapecio.baseMenor <= 0 || miTrapecio.altura <= 0) {
                                            cout << "Primero debes ingresar bases y altura validas (Opcion 3)." << endl;
                                        } else {
                                            cout << "El Perimetro del trapecio es: " << miTrapecio.calcularPerimetro() << endl;
                                        }
                                        break;

                                    case 3:
                                        cout << "Introduce el valor de la base mayor: ";
                                        cin >> miTrapecio.baseMayor;
                                        cout << "Introduce el valor de la base menor: ";
                                        cin >> miTrapecio.baseMenor;
                                        cout << "Introduce el valor de la altura: ";
                                        cin >> miTrapecio.altura;
                                        cout << "¡Bases y altura actualizadas con exito!" << endl;
                                        break;

                                    case 4:
                                        if (miTrapecio.baseMayor <= 0 || miTrapecio.baseMenor <= 0 || miTrapecio.altura <= 0) {
                                            cout << "Primero debes ingresar bases y altura validas (Opcion 3)." << endl;
                                        } else {
                                            cout << "\n=== RESUMEN DEL TRAPECIO ===" << endl;
                                            cout << "Base Mayor: " << miTrapecio.baseMayor << endl;
                                            cout << "Base Menor: " << miTrapecio.baseMenor << endl;
                                            cout << "Altura: " << miTrapecio.altura << endl;
                                            cout << "Area: " << miTrapecio.calcularArea() << endl;
                                            cout << "Perimetro: " << miTrapecio.calcularPerimetro() << endl;
                                        }
                                        break;
                                    case 5:
                                        cout << "Regresando al submenu de Figuras 2D..." << endl;
                                        break;
                                }
                            }while (opcionTrapecio != 5);
                            break;        
                        default:
                            if (opcion2D != 0) {
                                cout << "Opcion no valida. Intenta de nuevo." << endl;
                            }
                            break;
                    } // cierra switch(opcion2D)
                } while (opcion2D != 0); // cierra do del menu 2D
                break;

            case 2: // --- FIGURAS 3D ---
                cout << "\n--- FIGURAS 3D ---" << endl;
                do {
                    cout << "\n--- FIGURAS 3D ---" << endl;
                    cout << "1. Cubo" << endl;
                    cout << "2. Esfera" << endl;
                    cin >> opcion3D;

                    switch (opcion3D) {
                        case 1: //--- SUBMENÚ CUBO ---
                            do {
                                cout << "\n--- MENU CUBO ---" << endl;
                                cout << "1. Calcular y mostrar Area" << endl;
                                cout << "2. Calcular y mostrar Volumen" << endl;
                                cout << "3. Ingresar / Cambiar el lado del cubo" << endl;
                                cout << "4. Resumen (Opciones 1,2,3)" << endl;
                                cout << "5. Regresar al menu 3D" << endl;
                                cout << "Introduce tu opcion: ";
                                cin >> opcionCubo;
                            switch (opcionCubo) {
                                case 1:
                                    if (miCubo.lado <= 0) {
                                        cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                    } else {
                                        cout << "El Area del cubo es: " << miCubo.calcularArea() << endl;
                                    }
                                    break;
                                case 2:
                                    if (miCubo.lado <= 0) {
                                        cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                    } else {
                                        cout << "El Volumen del cubo es: " << miCubo.calcularVolumen() << endl;
                                    }
                                    break;
                                case 3:
                                    cout << "Introduce el valor del lado del cubo: ";
                                    cin >> miCubo.lado;
                                    cout << "¡Lado del cubo actualizado con exito!" << endl;
                                    break;
                                case 4:
                                    if (miCubo.lado <= 0) {
                                        cout << "Primero debes ingresar un lado valido (Opcion 4)." << endl;
                                    } else {
                                        cout << "\n=== RESUMEN DEL CUBO ===" << endl;
                                        cout << "Lado: " << miCubo.lado << endl;
                                        cout << "Area: " << miCubo.calcularArea() << endl;
                                        cout << "Volumen: " << miCubo.calcularVolumen() << endl;
                                    }   
                                    break;
                                case 5:
                                    cout << "Regresando al menu de Figuras 3D..." << endl;
                                    break;
                                default:
                                    cout << "Opcion no valida. Intenta de nuevo." << endl;
                                    break;
                            }
                        } while (opcionCubo !=5 );
                        break;
                        case 2:
                            // Implementar opciones para la esfera
                            break;
                    }
                } while (opcion3D != 0); // cierra do del menu 3D
                break;

                    case constant expression:
                        /* code */
                        break;
                    
                    default:
                        break;
                    }
                }
                break;

            case 3: // --- SALIR ---
                cout << "¡Gracias por usar el programa! Saliendo..." << endl;
                break;

            default:
                cout << "Opcion no valida. Intenta de nuevo." << endl;
                break;
        } // cierra switch(opcionPrincipal)

    } while (opcionPrincipal != 3); // cierra do-while principal

    return 0;
}