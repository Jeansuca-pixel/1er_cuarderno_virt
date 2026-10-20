#pragma once
#include <string>

namespace techstore {
class Fecha {
private:
    int dia, mes, anio;
public:
    Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}
    Fecha();

    static Fecha hoy();
    static bool es_bisiesto(int a);
    static int dias_del_mes(int m, int a);
    static bool es_valida(int d, int m, int a);
    static bool desde_texto(const std::string& s, Fecha& resultado);

    int clave() const { return anio * 10000 + mes * 100 + dia; }
    bool operator<(const Fecha& o) const { return clave() < o.clave(); }
    bool operator<=(const Fecha& o) const { return clave() <= o.clave(); }
    bool operator>=(const Fecha& o) const { return clave() >= o.clave(); }
    bool operator==(const Fecha& o) const { return clave() == o.clave(); }
    std::string texto() const;
};
}