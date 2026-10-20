#include <techstore/fecha.h>
#include <techstore/utilidades.h>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace techstore {

Fecha::Fecha() : dia(1), mes(1), anio(2000) { *this = hoy(); }

Fecha Fecha::hoy() {
    std::time_t t = std::time(nullptr);
    std::tm info{};
#ifdef _WIN32
    std::tm* local = std::localtime(&t);
    if (local != nullptr) info = *local;
#else
    localtime_r(&t, &info);
#endif
    return Fecha(info.tm_mday, info.tm_mon + 1, info.tm_year + 1900);
}

bool Fecha::es_bisiesto(int a) { return (a % 4 == 0 && a % 100 != 0) || a % 400 == 0; }

int Fecha::dias_del_mes(int m, int a) {
    static const int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && es_bisiesto(a)) return 29;
    return dias[m - 1];
}

bool Fecha::es_valida(int d, int m, int a) {
    if (a < 2000 || a > 2100) return false;
    if (m < 1 || m > 12) return false;
    return d >= 1 && d <= dias_del_mes(m, a);
}

bool Fecha::desde_texto(const std::string& s, Fecha& resultado) {
    std::vector<std::string> p = dividir(s, '/');
    if (p.size() != 3) return false;
    if (!solo_digitos(p[0]) || !solo_digitos(p[1]) || !solo_digitos(p[2])) return false;
    if (p[0].size() > 2 || p[1].size() > 2 || p[2].size() != 4) return false;
    int d = std::stoi(p[0]), m = std::stoi(p[1]), a = std::stoi(p[2]);
    if (!es_valida(d, m, a)) return false;
    resultado = Fecha(d, m, a);
    return true;
}

std::string Fecha::texto() const {
    std::ostringstream o;
    o << std::setfill('0') << std::setw(2) << dia << "/" << std::setw(2) << mes << "/" << std::setw(4) << anio;
    return o.str();
}

}