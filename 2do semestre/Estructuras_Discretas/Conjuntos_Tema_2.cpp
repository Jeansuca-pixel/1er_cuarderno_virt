//Alumno:Jean Andre Sucari Chambi           Codigo:260238
//Hice el 80% la IA me ayudo con un 20% en la comprension 
#include <iostream>
using namespace std;
const int TAM = 8;
// Función para imprimir un arreglo de ceros y unos y sus elementos correspondientes
void imprimirConjunto(const int conjunto[], const char* nombres[]) {
    cout << "[ ";
    for (int i = 0; i < TAM; i++) {
        cout << conjunto[i] << " ";
    }
    cout << "]  ->  { ";
    
    bool primero = true;
    for (int i = 0; i < TAM; i++) {
        if (conjunto[i] == 1) {
            if (!primero) cout << ", ";
            cout << nombres[i];
            primero = false;
        }
    }
    if (primero) cout << "vacio";
    cout << " }\n";
}
int main() {
    // Definición de las etiquetas del universo U
    const char* U_nombres[TAM] = {
        "FORTRAN", "PASCAL", "ADA", "COBOL", 
        "LISP", "BASIC", "C++", "FORTH"
    };
    // U = {FORTRAN, PASCAL, ADA, COBOL, LISP, BASIC, C++, FORTH}
    int B[TAM] = {0, 0, 1, 0, 0, 1, 1, 0}; // {C++, BASIC, ADA}
    int C[TAM] = {0, 1, 1, 0, 1, 0, 1, 0}; // {PASCAL, ADA, LISP, C++}
    int D[TAM] = {1, 1, 1, 0, 0, 1, 0, 1}; // {FORTRAN, PASCAL, ADA, BASIC, FORTH}
    int E[TAM] = {0, 1, 1, 1, 1, 0, 1, 0}; // {PASCAL, ADA, COBOL, LISP, C++}
    // Arreglos auxiliares para almacenar los resultados
    int res1[TAM];
    int res2[TAM];
    int res3[TAM];
    int res4[TAM];
    // 1. B U C
    for (int i = 0; i < TAM; i++) {
        res1[i] = (B[i] || C[i]) ? 1 : 0;
    }
    // 2. B ∩ (D ∩ E)
    for (int i = 0; i < TAM; i++) {
        int D_inter_E = (D[i] && E[i]) ? 1 : 0;
        res2[i] = (B[i] && D_inter_E) ? 1 : 0;
    }
    // 3. complemento(B) U E
    for (int i = 0; i < TAM; i++) {
        int B_comp = 1 - B[i];
        res3[i] = (B_comp || E[i]) ? 1 : 0;
    }
    // 4. complemento(C) ∩ (B U E)
    for (int i = 0; i < TAM; i++) {
        int C_comp = 1 - C[i];
        int B_union_E = (B[i] || E[i]) ? 1 : 0;
        res4[i] = (C_comp && B_union_E) ? 1 : 0;
    }
    cout << "  REPRESENTACION DE CONJUNTOS POR BITS (0s y 1s)"<< endl;
    cout << "1. B U C:"<<endl;
    imprimirConjunto(res1, U_nombres);
    cout << endl;
    cout << "2. B n (D n E):"<< endl;
    imprimirConjunto(res2, U_nombres);
    cout << endl;
    cout << "3. Complemento(B) U E:"<< endl;
    imprimirConjunto(res3, U_nombres);
    cout << endl;
    cout << "4. Complemento(C) n (B U E):"<<endl;
    imprimirConjunto(res4, U_nombres);
    cout << endl;
    return 0;
}