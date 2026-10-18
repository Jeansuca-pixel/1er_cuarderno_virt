// Jean Andre Sucari Chambi------Codigo: 260238
// Antes de que lea el codigo, ingeniera buenos dias tarde o noches. 
#include <iostream>
using namespace std;
int main() {
    // n sera el numero que utilicemos para los 3 casos del for 
    int n;
    //cuando ingrese el numero aprete enter. perdon si es obvio pero me puse en el papel que tal es una perdona que no sabe nada de programacion y le explico todo xd
    cout << "Ingresa un numero entero positivo (n): ";
    cin >> n;
    // Los valores tienen que ser si o si mayor a cero
    if (n <= 0) {
        cout << "Por favor, ingresa un numero natural valido (mayor a 0)." << endl;
        return 0;
    }
    double suma1 = 0;
    double suma2 = 0;
    double suma3 = 0;
    // Este For es para la suma de lo numeros naturales 
    for (int i = 1; i <= n; i++) {
        suma1 += i;
        cout << i;

        // Si no es el ultimo numero, imprime el signo "+"
        if (i < n) {
            cout << " + ";
        }
        
    }
    cout << " = " << suma1 << endl;

    cout << "Suma de 1 las (n) veces" << endl;
    // Este For es para la suma de 1 las n veces que agregues 
    for (int i = 1; i <= n; i++){
        suma2 += 1;
        cout<<"1";
        
        if (i < n){
            cout << " + ";
        }
    }
    cout <<" = " << suma2 << endl;

    cout << "Suma de los cuadrados (n)" <<endl;
    // Este for es para la suma de los cuadrados
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