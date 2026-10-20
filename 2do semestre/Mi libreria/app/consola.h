#pragma once
#include <iostream>
#include <string>
#include <cmath>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <techstore/utilidades.h>
#include <techstore/fecha.h>

using namespace std;
using namespace techstore;

// ---------------------------------------------------------------------------
//  Colores y excepcion de salida
// ---------------------------------------------------------------------------
namespace Color { // codigos ANSI para colorear la consola
    const string ROJO = "\033[31m";
    const string VERDE = "\033[32m";
    const string AMARILLO = "\033[33m";
    const string CIAN = "\033[36m";
    const string RESET = "\033[0m";
}

// Excepcion para cerrar el programa de forma ordenada (reemplaza a exit()).
struct SalirPrograma { string mensaje; };

// ---------------------------------------------------------------------------
//  Sistema y pantalla
// ---------------------------------------------------------------------------
inline void preparar_consola() { // activa colores ANSI en Windows
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (GetConsoleMode(h, &modo)) SetConsoleMode(h, modo | 0x0004);
#endif
}

inline void esperar(int ms) { // pausa en milisegundos
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

inline void limpiar() { cout << "\033[2J\033[H" << flush; } // borra la pantalla

inline void escribir(const string& texto, int ms = 15) { // efecto maquina de escribir
    for (char c : texto) { cout << c << flush; esperar(ms); }
}

inline void cargando(const string& mensaje) { // barra de carga animada
    cout << mensaje << endl << "[";
    for (int i = 0; i < 20; i++) { cout << "#" << flush; esperar(35); }
    cout << "] 100%" << endl;
    esperar(200);
}

inline void puntos(const string& mensaje) { // mensaje con tres puntos animados
    cout << mensaje;
    for (int i = 0; i < 3; i++) { cout << "." << flush; esperar(200); }
    cout << endl;
}

// ---------------------------------------------------------------------------
//  Lectura de datos por consola
// ---------------------------------------------------------------------------
inline string leer_texto(const string& mensaje) {
    cout << mensaje;
    string s;
    if (!getline(cin, s)) throw SalirPrograma{"\nEntrada cerrada. Saliendo del programa."}; // fin de entrada (Ctrl+D)
    return recortar(s);
}

inline string leer_texto_no_vacio(const string& mensaje) {
    while (true) {
        string s = leer_texto(mensaje);
        if (!s.empty()) return s;
        cout << Color::ROJO << "Este campo no puede estar vacio." << Color::RESET << endl;
    }
}

inline double leer_numero(const string& mensaje) {
    while (true) {
        string s = leer_texto(mensaje);
        try {
            size_t pos = 0;
            double v = stod(s, &pos);
            if (pos == s.size() && isfinite(v)) return v; // acepta solo si todo el texto es numero
        } catch (...) {}
        cout << Color::ROJO << "Numero invalido, intenta de nuevo." << Color::RESET << endl;
    }
}

inline double leer_numero_rango(const string& mensaje, double minimo, double maximo) {
    while (true) {
        double v = leer_numero(mensaje);
        if (v >= minimo && v <= maximo) return v;
        cout << Color::ROJO << "Ingresa un valor entre " << minimo << " y " << maximo << "." << Color::RESET << endl;
    }
}

inline double leer_numero_positivo(const string& mensaje) {
    while (true) {
        double v = leer_numero(mensaje);
        if (v > 0 && v <= 10000000) return v; // tope para evitar valores absurdos
        cout << Color::ROJO << "El valor debe ser mayor que cero." << Color::RESET << endl;
    }
}

inline int leer_entero_rango(const string& mensaje, int minimo, int maximo) {
    while (true) {
        double v = leer_numero(mensaje);
        if (v == floor(v) && v >= minimo && v <= maximo) return (int)v; // solo enteros
        cout << Color::ROJO << "Ingresa un entero entre " << minimo << " y " << maximo << "." << Color::RESET << endl;
    }
}

inline int leer_entero_positivo(const string& mensaje) { return leer_entero_rango(mensaje, 1, 100000); }

inline bool confirmar(const string& mensaje) {
    string r = a_minuscula(leer_texto(mensaje + " (s/n): "));
    return r == "s" || r == "si";
}

inline void pausa() { leer_texto("\nPresiona Enter para continuar..."); }

inline string leer_documento(const string& tipo) { // pide un DNI o RUC hasta que sea valido
    while (true) {
        string d = leer_texto("Numero de " + tipo + ": ");
        if (tipo == "RUC" ? ruc_valido(d) : dni_valido(d)) return d;
        if (tipo == "RUC") cout << Color::ROJO << "El RUC debe tener 11 digitos y empezar con 10 o 20." << Color::RESET << endl;
        else cout << Color::ROJO << "El DNI debe tener 8 digitos." << Color::RESET << endl;
    }
}

inline Fecha leer_fecha(const string& mensaje) {
    while (true) {
        string s = leer_texto(mensaje);
        if (s.empty()) return Fecha::hoy(); // Enter vacio = hoy
        Fecha f;
        if (Fecha::desde_texto(s, f)) return f;
        cout << Color::ROJO << "Fecha invalida. Usa el formato dd/mm/aaaa (ej. 05/03/2026)." << Color::RESET << endl;
    }
}