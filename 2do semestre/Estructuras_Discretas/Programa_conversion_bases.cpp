#include <iostream>
using namespace std;
int main()  {
    int opcion;
    
    do {
        cout << "--------------------------------------------------------" << endl;
        cout << "Ingrese la base a la que desea convertir un numero " << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "1. Decimal a Binario" << endl;
        cout << "2. Binario a Decimal" << endl;
        cout << "3. Salir" << endl;
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                cout << "Convirtiendo de decimal a binario" << endl;
                double numero ;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                // Separar parte entera y fraccionaria
                int entero = numero;
                double fraccion = numero - entero;
                
                int binario_entero = 0;
                int multiplicador_entero = 1;
                int temporal_entero = entero;
                
                // Para la parte entera
                while (temporal_entero > 0) {
                    int bit = temporal_entero % 2;
                    temporal_entero = temporal_entero / 2;
                    binario_entero = (bit * multiplicador_entero) + binario_entero;
                    multiplicador_entero = multiplicador_entero * 10;
                }
                if (entero == 0) {
                    binario_entero = 0;
                }
                // Mostrar la parte entera
                cout << "Numero en binario: " << binario_entero;
                // Calcular y mostrar la parte fraccionaria
                if (fraccion > 0) {
                    cout << ".";
                    int limite_bits = 6; 
                    
                    while (limite_bits > 0 && fraccion > 0) {
                        fraccion = fraccion * 2;
                        int bit_fraccion = fraccion; 
                        cout << bit_fraccion;
                        
                        if (bit_fraccion == 1) {
                            fraccion = fraccion - 1; 
                        }
                        limite_bits--;
                    }
                }
                
                cout << endl;
                break;
            }
            case 2:
                cout << "Convirtiendo de binario a decimal" << endl;
                //falta implementar la conversion de binario a decimal
                break;
            case 3:
                cout << "salir" << endl;
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
                break;
        }
    } while (opcion != 3);
    return 0;
}