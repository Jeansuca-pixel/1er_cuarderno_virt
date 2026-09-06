#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace std::chrono;

const int TAM = 10;

// ---------------------- Funciones auxiliares ----------------------

void generarArrayAleatorio(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000; // numeros entre 0 y 999
    }
}

void copiarArray(int origen[], int destino[], int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origen[i];
    }
}

void imprimirArray(int arr[], int n) {
    cout << "[ ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << "]" << endl;
}

// ---------------------- 1. Bubble Sort ----------------------

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ---------------------- 2. Insertion Sort ----------------------

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int clave = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > clave) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = clave;
    }
}

// ---------------------- 3. Selection Sort ----------------------

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int indiceMinimo = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[indiceMinimo]) {
                indiceMinimo = j;
            }
        }
        if (indiceMinimo != i) {
            swap(arr[i], arr[indiceMinimo]);
        }
    }
}

// ---------------------- 4. Merge Sort ----------------------

void merge(int arr[], int inicio, int medio, int fin) {
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;

    vector<int> izquierda(n1);
    vector<int> derecha(n2);

    for (int i = 0; i < n1; i++)
        izquierda[i] = arr[inicio + i];
    for (int j = 0; j < n2; j++)
        derecha[j] = arr[medio + 1 + j];

    int i = 0, j = 0, k = inicio;

    while (i < n1 && j < n2) {
        if (izquierda[i] <= derecha[j]) {
            arr[k] = izquierda[i];
            i++;
        } else {
            arr[k] = derecha[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = izquierda[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = derecha[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int inicio, int fin) {
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;
        mergeSort(arr, inicio, medio);
        mergeSort(arr, medio + 1, fin);
        merge(arr, inicio, medio, fin);
    }
}

// ---------------------- 5. Quick Sort ----------------------

int particion(int arr[], int bajo, int alto) {
    int pivote = arr[alto];
    int i = bajo - 1;

    for (int j = bajo; j < alto; j++) {
        if (arr[j] < pivote) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[alto]);
    return i + 1;
}

void quickSort(int arr[], int bajo, int alto) {
    if (bajo < alto) {
        int pi = particion(arr, bajo, alto);
        quickSort(arr, bajo, pi - 1);
        quickSort(arr, pi + 1, alto);
    }
}

// ---------------------- 6. Counting Sort ----------------------

void countingSort(int arr[], int n) {
    if (n == 0) return;

    int maximo = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maximo) maximo = arr[i];
    }

    vector<int> conteo(maximo + 1, 0);
    for (int i = 0; i < n; i++) {
        conteo[arr[i]]++;
    }

    int indice = 0;
    for (int valor = 0; valor <= maximo; valor++) {
        while (conteo[valor] > 0) {
            arr[indice] = valor;
            indice++;
            conteo[valor]--;
        }
    }
}

// ---------------------- Medición de tiempo ----------------------

template <typename Func>
double medirTiempo(Func funcionOrdenamiento, int arrOriginal[], int n) {
    int arr[TAM];
    copiarArray(arrOriginal, arr, n);

    auto inicio = high_resolution_clock::now();
    funcionOrdenamiento(arr, n);
    auto fin = high_resolution_clock::now();

    duration<double, nano> duracion = fin - inicio;
    return duracion.count(); // tiempo en nanosegundos
}

// ---------------------- Programa principal ----------------------

void mostrarMenu() {
    cout << "\n===== COMPARADOR DE METODOS DE ORDENAMIENTO =====\n";
    cout << "1. Bubble Sort\n";
    cout << "2. Insertion Sort\n";
    cout << "3. Selection Sort\n";
    cout << "4. Merge Sort\n";
    cout << "5. Quick Sort\n";
    cout << "6. Counting Sort\n";
    cout << "7. Todos los metodos\n";
    cout << "0. Salir\n";
    cout << "Elige una opcion: ";
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int arrOriginal[TAM];
    generarArrayAleatorio(arrOriginal, TAM);

    cout << "Array original generado aleatoriamente:\n";
    imprimirArray(arrOriginal, TAM);

    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;

        int arrPrueba[TAM];
        double tiempo;

        switch (opcion) {
            case 1:
                tiempo = medirTiempo(bubbleSort, arrOriginal, TAM);
                cout << fixed << setprecision(2);
                cout << "Tiempo Bubble Sort: " << tiempo << " ns\n";
                break;

            case 2:
                tiempo = medirTiempo(insertionSort, arrOriginal, TAM);
                cout << fixed << setprecision(2);
                cout << "Tiempo Insertion Sort: " << tiempo << " ns\n";
                break;

            case 3:
                tiempo = medirTiempo(selectionSort, arrOriginal, TAM);
                cout << fixed << setprecision(2);
                cout << "Tiempo Selection Sort: " << tiempo << " ns\n";
                break;

            case 4: {
                copiarArray(arrOriginal, arrPrueba, TAM);
                auto inicio = high_resolution_clock::now();
                mergeSort(arrPrueba, 0, TAM - 1);
                auto fin = high_resolution_clock::now();
                duration<double, nano> duracion = fin - inicio;
                cout << fixed << setprecision(2);
                cout << "Tiempo Merge Sort: " << duracion.count() << " ns\n";
                break;
            }

            case 5: {
                copiarArray(arrOriginal, arrPrueba, TAM);
                auto inicio = high_resolution_clock::now();
                quickSort(arrPrueba, 0, TAM - 1);
                auto fin = high_resolution_clock::now();
                duration<double, nano> duracion = fin - inicio;
                cout << fixed << setprecision(2);
                cout << "Tiempo Quick Sort: " << duracion.count() << " ns\n";
                break;
            }

            case 6:
                tiempo = medirTiempo(countingSort, arrOriginal, TAM);
                cout << fixed << setprecision(2);
                cout << "Tiempo Counting Sort: " << tiempo << " ns\n";
                break;

            case 7: {
                cout << "\n--- Comparacion de todos los metodos ---\n";
                cout << fixed << setprecision(2);

                double tBubble = medirTiempo(bubbleSort, arrOriginal, TAM);
                cout << "Bubble Sort:    " << tBubble << " ns\n";

                double tInsertion = medirTiempo(insertionSort, arrOriginal, TAM);
                cout << "Insertion Sort: " << tInsertion << " ns\n";

                double tSelection = medirTiempo(selectionSort, arrOriginal, TAM);
                cout << "Selection Sort: " << tSelection << " ns\n";

                int arrMerge[TAM];
                copiarArray(arrOriginal, arrMerge, TAM);
                auto i1 = high_resolution_clock::now();
                mergeSort(arrMerge, 0, TAM - 1);
                auto f1 = high_resolution_clock::now();
                double tMerge = duration<double, nano>(f1 - i1).count();
                cout << "Merge Sort:     " << tMerge << " ns\n";

                int arrQuick[TAM];
                copiarArray(arrOriginal, arrQuick, TAM);
                auto i2 = high_resolution_clock::now();
                quickSort(arrQuick, 0, TAM - 1);
                auto f2 = high_resolution_clock::now();
                double tQuick = duration<double, nano>(f2 - i2).count();
                cout << "Quick Sort:     " << tQuick << " ns\n";

                double tCounting = medirTiempo(countingSort, arrOriginal, TAM);
                cout << "Counting Sort:  " << tCounting << " ns\n";

                cout << "\nArray ordenado de ejemplo (con Quick Sort):\n";
                imprimirArray(arrQuick, TAM);
                break;
            }

            case 0:
                cout << "Saliendo del programa...\n";
                break;

            default:
                cout << "Opcion invalida. Intenta de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}