#include <techstore/utilidades.h>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace techstore {

std::string recortar(const std::string& s) {
    size_t i = s.find_first_not_of(" \t\r\n");
    if (i == std::string::npos) return "";
    size_t j = s.find_last_not_of(" \t\r\n");
    return s.substr(i, j - i + 1);
}

std::string a_minuscula(std::string s) {
    for (char& c : s) c = (char)std::tolower((unsigned char)c);
    return s;
}

std::string a_mayuscula(std::string s) {
    for (char& c : s) c = (char)std::toupper((unsigned char)c);
    return s;
}

double redondear2(double v) { return std::round(v * 100.0) / 100.0; }

std::string dinero(double v) {
    std::ostringstream o;
    o << "S/ " << std::fixed << std::setprecision(2) << v;
    return o.str();
}

std::vector<std::string> dividir(const std::string& texto, char sep) {
    std::vector<std::string> partes;
    std::string actual;
    for (char c : texto) {
        if (c == sep) { partes.push_back(actual); actual.clear(); }
        else actual += c;
    }
    partes.push_back(actual);
    return partes;
}

std::string centrar(const std::string& t, std::size_t ancho) {
    if (t.size() >= ancho) return t;
    return std::string((ancho - t.size()) / 2, ' ') + t;
}

bool solo_digitos(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(),
        [](unsigned char c) { return std::isdigit(c) != 0; });
}

bool dni_valido(const std::string& s) { return s.size() == 8 && solo_digitos(s); }

bool ruc_valido(const std::string& s) {
    return s.size() == 11 && solo_digitos(s) && (s.substr(0, 2) == "10" || s.substr(0, 2) == "20");
}

}