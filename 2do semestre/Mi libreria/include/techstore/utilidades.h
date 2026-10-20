#pragma once
#include <string>
#include <vector>

namespace techstore {
std::string recortar(const std::string& s);
std::string a_minuscula(std::string s);
std::string a_mayuscula(std::string s);
double redondear2(double v);
std::string dinero(double v);
std::vector<std::string> dividir(const std::string& texto, char sep);
std::string centrar(const std::string& t, std::size_t ancho);
bool solo_digitos(const std::string& s);
bool dni_valido(const std::string& s);
bool ruc_valido(const std::string& s);
}

