#pragma once
#include <string>

namespace techstore {
namespace Config {
    inline const double IGV = 0.18;
    inline const int STOCK_MINIMO = 3;
    inline const double DESCUENTO_FRECUENTE = 5.0;
    inline const double DESCUENTO_MAXIMO = 30.0;
    inline const int COMPRAS_PARA_FRECUENTE = 3;
    inline const std::string RUC_EMPRESA = "20601234567";
    inline const std::string NOMBRE_EMPRESA = "TECH STORE S.A.C.";
    inline const std::string DIRECCION_EMPRESA = "Av. Floral 450 - Puno / Cel: 900-691-089x";
    inline const std::string ROL_ADMIN = "ADMIN";
    inline const std::string ROL_CAJERO = "CAJERO";
    inline const std::string ESTADO_COMPLETADA = "Completada";
    inline const std::string ESTADO_ANULADA = "Anulada";
    inline const std::string CAT_GENERAL = "General";
    inline const std::string CAT_FRECUENTE = "Frecuente";
}
}