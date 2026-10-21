#include <iostream>
using namespace std;
int main()  {
    int opcion;
    char simbolos[] = "0123456789ABCDEF";

    do {
        cout << "Ingrese la base a la que desea convertir un numero " << endl;
        cout << "1. Decimal a Base 2" << endl;
        cout << "2. Decimal a Base 3" << endl;
        cout << "3. Decimal a Base 4" << endl;
        cout << "4. Decimal a Base 5" << endl;
        cout << "5. Decimal a Base 6" << endl;
        cout << "6. Decimal a Base 7" << endl;
        cout << "7. Decimal a Base 8" << endl;
        cout << "8. Decimal a Base 9" << endl;
        cout << "9. Decimal a Base 11" << endl;
        cout << "10. Decimal a Base 12" << endl;
        cout << "11. Decimal a Base 13" << endl;
        cout << "12. Decimal a Base 14" << endl;
        cout << "13. Decimal a Base 15" << endl;
        cout << "14. Decimal a Base 16" << endl;
        cout << "0. Para salir " << endl;
        cin >> opcion;
        if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        opcion = -1;
}
        
        switch (opcion) {
            case 0:{    
                cout<<"saliendo ..." << endl; 
                break;
            }
            case 1: {
                cout << "Convirtiendo decimal a base 2" << endl;
                double numero ;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                // Separar parte entera y fraccionaria
                int entero = numero;
                double fraccion = numero - entero;
                
                int base2_entero = 0;
                int multiplicador_entero = 1;
                int temporal_entero = entero;
                
                // Para la parte entera
                while (temporal_entero > 0) {
                    int bit = temporal_entero % 2;
                    temporal_entero = temporal_entero / 2;
                    base2_entero = (bit * multiplicador_entero) + base2_entero;
                    multiplicador_entero = multiplicador_entero * 10;
                }
                if (entero == 0) {
                    base2_entero = 0;
                }

                cout << "Numero en base 2: " << base2_entero;
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
            case 2:{
                cout << "Convirtiendo decimal a base 3 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;  

                int entero = numero;
                double fraccion = numero - entero;

                int base3_entero = 0;
                int multiplicador_entero = 1;
                int temporal_entero = entero;

                while (temporal_entero > 0) {
                    int digito = temporal_entero % 3;
                    temporal_entero = temporal_entero / 3;
                    base3_entero = (digito * multiplicador_entero) + base3_entero;
                    multiplicador_entero = multiplicador_entero * 10;
                }
                if (entero == 0) {
                    base3_entero = 0;
                }

                cout << "Numero en base 3: " << base3_entero;
                if (fraccion > 0) {
                    cout << ".";
                    int limite_digitos = 6; 
                    
                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 3;
                        int digito_fraccion = fraccion; 
                        cout << digito_fraccion;
                        
                        if (digito_fraccion == 1 || digito_fraccion == 2) {
                            fraccion = fraccion - digito_fraccion; 
                        }
                        limite_digitos--;
                    }
                }
                cout << endl;
                break;
            }
            case 3:{
                cout << "Convirtiendo decimal a base 4 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;

                int base4_entero = 0;
                int multiplicador_entero = 1;
                int temporal_entero = entero;

                while (temporal_entero > 0) {
                    int digito = temporal_entero % 4;
                    temporal_entero = temporal_entero / 4;
                    base4_entero = (digito * multiplicador_entero) + base4_entero;
                    multiplicador_entero = multiplicador_entero * 10;
                }
                if (entero == 0) {
                    base4_entero = 0;
                }

                cout << "Numero en base 4: " << base4_entero;
                if (fraccion > 0) {
                    cout << ".";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 4;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion;

                        if (digito_fraccion == 1 || digito_fraccion == 2 || digito_fraccion == 3) {
                            fraccion = fraccion - digito_fraccion;
                        }
                        limite_digitos--;
                    }
                }
                cout << endl;
                break;
            }    
            case 4:{
                cout << "Convirtiendo decimal a base 5 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;

                int base5_entero = 0;
                int multiplicador_entero = 1;   
                int temporal_entero = entero;

                while (temporal_entero > 0) {
                    int digito = temporal_entero % 5;
                    temporal_entero = temporal_entero / 5;
                    base5_entero = (digito * multiplicador_entero) + base5_entero;
                    multiplicador_entero = multiplicador_entero * 10;
                }
                cout << "Numero en base 5: " << base5_entero;
                if (fraccion > 0) {
                    cout << ".";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 5;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion;

                        if (digito_fraccion == 1 || digito_fraccion == 2 || digito_fraccion == 3 || digito_fraccion == 4) {
                            fraccion = fraccion - digito_fraccion;
                        }
                        limite_digitos--;
                    }
                }
                cout << endl;
                break;
            }
            case 5:{
                cout << "Convirtiendo decimal a base 6 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;
                int base6_entero = 0;
                int multiplicador_entero = 1;
                int temporal_entero = entero;
                while (temporal_entero > 0) {
                    int digito = temporal_entero % 6;
                    temporal_entero = temporal_entero / 6;
                    base6_entero = (digito * multiplicador_entero) + base6_entero;
                    multiplicador_entero = multiplicador_entero * 10;
                }
                cout << "Numero en base 6: " << base6_entero;
                if (fraccion > 0) {
                    cout << ".";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 6;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion;

                        if (digito_fraccion == 1 || digito_fraccion == 2 || digito_fraccion == 3 || digito_fraccion == 4 || digito_fraccion == 5) {
                            fraccion = fraccion - digito_fraccion;
                        }
                        limite_digitos--;
                    }
                }
                cout << endl;
            }    break;
            case 6:{
                cout << "Convirtiendo de decimal a base 7 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;
                int base7_entero = 0;
                int multiplicador_entero = 1;
                int temporal_entero = entero;
                while (temporal_entero > 0) {
                    int digito = temporal_entero % 7;
                    temporal_entero = temporal_entero / 7;
                    base7_entero = (digito * multiplicador_entero) + base7_entero;
                    multiplicador_entero = multiplicador_entero * 10;
                }
                cout << "Numero en base 7: " << base7_entero;
                if (fraccion > 0) {
                    cout << ".";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 7;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion;

                        if (digito_fraccion == 1 || digito_fraccion == 2 || digito_fraccion == 3 || digito_fraccion == 4 || digito_fraccion == 5 || digito_fraccion == 6) {
                            fraccion = fraccion - digito_fraccion;
                        }
                        limite_digitos--;
                    }
                }
                cout << endl;
            }    break;
            case 7:{
                cout << "Convirtiendo de decimal a base 8 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;
                int base8_entero = 0;
                int multiplicador_entero = 1;
                int temporal_entero = entero;
                while (temporal_entero > 0) {
                    int digito = temporal_entero % 8;
                    temporal_entero = temporal_entero / 8;
                    base8_entero = (digito * multiplicador_entero) + base8_entero;
                    multiplicador_entero = multiplicador_entero * 10;
                }
                cout << "Numero en base 8: " << base8_entero;
                if (fraccion > 0) {
                    cout << ".";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 8;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion;

                        if (digito_fraccion == 1 || digito_fraccion == 2 || digito_fraccion == 3 || digito_fraccion == 4 || digito_fraccion == 5 || digito_fraccion == 6 || digito_fraccion == 7) {
                            fraccion = fraccion - digito_fraccion;
                        }
                        limite_digitos--;
                    }
                }
                cout << endl;
            }    break;
            case 8:{
                cout << "Convirtiendo de decimal a base 9 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;
                int base9_entero = 0;
                int multiplicador_entero = 1;
                int temporal_entero = entero;
                while (temporal_entero > 0) {
                    int digito = temporal_entero % 9;
                    temporal_entero = temporal_entero / 9;
                    base9_entero = (digito * multiplicador_entero) + base9_entero;
                    multiplicador_entero = multiplicador_entero * 10;
                }
                cout << "Numero en base 9: " << base9_entero;
                if (fraccion > 0) {
                    cout << ".";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 9;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion;

                        if (digito_fraccion == 1 || digito_fraccion == 2 || digito_fraccion == 3 || digito_fraccion == 4 || digito_fraccion == 5 || digito_fraccion == 6 || digito_fraccion == 7 || digito_fraccion == 8) {
                            fraccion = fraccion - digito_fraccion;
                        }
                        limite_digitos--;
                    }
                }
                cout << endl;
            }    break;
            case 9:{
                cout << "Convirtiendo de decimal a base 11 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;


                int digitos_entero[15];
                int cantidad = 0;                
                int temporal_entero = entero;    

                if (entero == 0) {
                    digitos_entero[cantidad] = 0;
                    cantidad++;
                }

                while (temporal_entero > 0) {
                    int digito = temporal_entero % 11;
                    temporal_entero = temporal_entero / 11;
                    digitos_entero[cantidad] = digito;
                    cantidad++;
                }

                cout << "Numero en base 11: ";
                for (int i = cantidad - 1; i >= 0; i--) {
                    cout << digitos_entero[i] << " ";
                }
                if (fraccion > 0) {
                    cout << ".";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 11;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion << " ";
                        fraccion = fraccion - digito_fraccion;
                        limite_digitos--;
                    }
                }
                cout << endl;
                break;
            }
            case 10:{
                cout << "Convirtiendo de decimal a base 12 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;

                int digitos_entero[15];
                int cantidad = 0;
                int temporal_entero = entero;

                if (entero == 0) {
                    digitos_entero[cantidad] = 0;
                    cantidad++;
                }
                while (temporal_entero > 0) {
                    int digito = temporal_entero % 12;
                    temporal_entero = temporal_entero / 12;
                    digitos_entero[cantidad] = digito;
                    cantidad++;
                }

                cout << "Numero en base 12: ";
                for (int i = cantidad - 1; i >= 0; i--) {
                    cout << digitos_entero[i] << " ";
                }

                if (fraccion > 0) {
                    cout << ". ";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 12;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion << " ";
                        fraccion = fraccion - digito_fraccion;
                        limite_digitos--;
                    }
                }
                cout << endl;
                break;
            }
            case 11:{
                cout << "Convirtiendo de decimal a base 13 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;

                int digitos_entero[15];
                int cantidad = 0;
                int temporal_entero = entero;

                if (entero == 0) {
                    digitos_entero[cantidad] = 0;
                    cantidad++;
                }
                while (temporal_entero > 0) {
                    int digito = temporal_entero % 13;
                    temporal_entero = temporal_entero / 13;
                    digitos_entero[cantidad] = digito;
                    cantidad++;
                }

                cout << "Numero en base 13: ";
                for (int i = cantidad - 1; i >= 0; i--) {
                    cout << digitos_entero[i] << " ";
                }

                if (fraccion > 0) {
                    cout << ". ";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 13;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion << " ";
                        fraccion = fraccion - digito_fraccion;
                        limite_digitos--;
                    }
                }
                cout << endl;
                break;
            }
            case 12:{
                cout << "Convirtiendo de decimal a base 14 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;

                int digitos_entero[15];
                int cantidad = 0;
                int temporal_entero = entero;

                if (entero == 0) {
                    digitos_entero[cantidad] = 0;
                    cantidad++;
                }
                while (temporal_entero > 0) {
                    int digito = temporal_entero % 14;
                    temporal_entero = temporal_entero / 14;
                    digitos_entero[cantidad] = digito;
                    cantidad++;
                }

                cout << "Numero en base 14: ";
                for (int i = cantidad - 1; i >= 0; i--) {
                    cout << digitos_entero[i] << " ";
                }

                if (fraccion > 0) {
                    cout << ". ";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 14;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion << " ";
                        fraccion = fraccion - digito_fraccion;
                        limite_digitos--;
                    }
                }
                cout << endl;
                break;
            }
            case 13:{
                cout << "Convirtiendo de decimal a base 15 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;

                int digitos_entero[15];
                int cantidad = 0;
                int temporal_entero = entero;

                if (entero == 0) {
                    digitos_entero[cantidad] = 0;
                    cantidad++;
                }
                while (temporal_entero > 0) {
                    int digito = temporal_entero % 15;
                    temporal_entero = temporal_entero / 15;
                    digitos_entero[cantidad] = digito;
                    cantidad++;
                }

                cout << "Numero en base 15: ";
                for (int i = cantidad - 1; i >= 0; i--) {
                    cout << digitos_entero[i] << " ";
                }

                if (fraccion > 0) {
                    cout << ". ";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 15;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion << " ";
                        fraccion = fraccion - digito_fraccion;
                        limite_digitos--;
                    }
                }
                cout << endl;
                break;
            }
            case 14:{
                cout << "Convirtiendo de decimal a base 16 " << endl;
                double numero;
                cout << "Ingrese un numero decimal: ";
                cin >> numero;

                int entero = numero;
                double fraccion = numero - entero;

                int digitos_entero[15];
                int cantidad = 0;
                int temporal_entero = entero;

                if (entero == 0) {
                    digitos_entero[cantidad] = 0;
                    cantidad++;
                }
                while (temporal_entero > 0) {
                    int digito = temporal_entero % 16;
                    temporal_entero = temporal_entero / 16;
                    digitos_entero[cantidad] = digito;
                    cantidad++;
                }

                cout << "Numero en base 16: ";
                for (int i = cantidad - 1; i >= 0; i--) {
                    cout << digitos_entero[i] << " ";
                }

                if (fraccion > 0) {
                    cout << ". ";
                    int limite_digitos = 6;

                    while (limite_digitos > 0 && fraccion > 0) {
                        fraccion = fraccion * 16;
                        int digito_fraccion = fraccion;
                        cout << digito_fraccion << " ";
                        fraccion = fraccion - digito_fraccion;
                        limite_digitos--;
                    }
                }
                cout << endl;
                break;
            }
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
                break;
        }
    } while (opcion != 0);
    return 0;
}