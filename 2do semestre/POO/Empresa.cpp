#include <iostream>
#include <string>
using namespace std;

// Clase auxiliar Fecha
class Fecha {
private:
    int dia;
    int mes;
    int anio;

public:
    Fecha() {
        dia = 1;
        mes = 1;
        anio = 2026;
    }

    Fecha(int d, int m, int a) {
        dia = d;
        mes = m;
        anio = a;
    }

    void imprimir() {
        cout << dia << "/" << mes << "/" << anio;
    }
};

// Clase Base: Servicio
class Servicio {
private:
    string Id_servicio;
    string Nombre;
    string Descripcion;
    float Costo_base;
    Fecha Fecha_solicitud;
    string Estado; // ej. Pendiente, En Proceso, Completado, Cancelado

public:
    Servicio(string id, string nom, string desc, float costo, Fecha fecha, string est) {
        Id_servicio = id;
        Nombre = nom;
        Descripcion = desc;
        Costo_base = costo;
        Fecha_solicitud = fecha;
        Estado = est;
    }

    // Getters
    string get_id_servicio() {
        return Id_servicio;
    }

    string get_nombre() {
        return Nombre;
    }

    string get_descripcion() {
        return Descripcion;
    }

    float get_costo_base() {
        return Costo_base;
    }

    Fecha get_fecha_solicitud() {
        return Fecha_solicitud;
    }

    string get_estado() {
        return Estado;
    }

    // Setters
    void set_nombre(string nom) {
        Nombre = nom;
    }

    void set_descripcion(string desc) {
        Descripcion = desc;
    }

    void set_costo_base(float costo) {
        Costo_base = costo;
    }

    void set_estado(string est) {
        Estado = est;
    }
};

// Clase Derivada: Mantenimiento
class Mantenimiento : public Servicio {
private:
    string Tipo_mantenimiento; // ej. Preventivo, Correctivo
    string Equipo_afectado;    // o Id_producto
    string Diagnostico;
    string Repuestos_utilizados[20]; // Arreglo para la lista de repuestos
    int Total_repuestos;
    float Horas_trabajadas;

public:
    Mantenimiento(string id, string nom, string desc, float costo, Fecha fecha, string est,
                  string tipo_manten, string equipo, string diag, float horas)
        : Servicio(id, nom, desc, costo, fecha, est) {
        Tipo_mantenimiento = tipo_manten;
        Equipo_afectado = equipo;
        Diagnostico = diag;
        Horas_trabajadas = horas;
        Total_repuestos = 0;
    }

    // Getters
    string get_tipo_mantenimiento() {
        return Tipo_mantenimiento;
    }

    string get_equipo_afectado() {
        return Equipo_afectado;
    }

    string get_diagnostico() {
        return Diagnostico;
    }

    float get_horas_trabajadas() {
        return Horas_trabajadas;
    }

    void get_repuestos_utilizados() {
        for (int i = 0; i < Total_repuestos; i++) {
            cout << "- " << Repuestos_utilizados[i] << endl;
        }
    }

    // Setters
    void set_tipo_mantenimiento(string tipo) {
        Tipo_mantenimiento = tipo;
    }

    void set_equipo_afectado(string equipo) {
        Equipo_afectado = equipo;
    }

    void set_diagnostico(string diag) {
        Diagnostico = diag;
    }

    void set_repuestos_utilizados(string repuesto) {
        if (Total_repuestos < 20) {
            Repuestos_utilizados[Total_repuestos] = repuesto;
            Total_repuestos++;
        }
    }

    void set_horas_trabajadas(float horas) {
        Horas_trabajadas = horas;
    }
};

// Clase Derivada: Reclamo
class Reclamo : public Servicio {
private:
    string Numero_reclamo;
    string Motivo;
    string Prioridad; // ej. Alta, Media, Baja
    string Solucion_propuesta;
    Fecha Fecha_resolucion;

public:
    Reclamo(string id, string nom, string desc, float costo, Fecha fecha, string est,
            string num_reclamo, string mot, string prior, string sol_propuesta, Fecha fecha_res)
        : Servicio(id, nom, desc, costo, fecha, est) {
        Numero_reclamo = num_reclamo;
        Motivo = mot;
        Prioridad = prior;
        Solucion_propuesta = sol_propuesta;
        Fecha_resolucion = fecha_res;
    }

    // Getters
    string get_numero_reclamo() {
        return Numero_reclamo;
    }

    string get_motivo() {
        return Motivo;
    }

    string get_prioridad() {
        return Prioridad;
    }

    string get_solucion_propuesta() {
        return Solucion_propuesta;
    }

    Fecha get_fecha_resolucion() {
        return Fecha_resolucion;
    }

    // Setters
    void set_motivo(string mot) {
        Motivo = mot;
    }

    void set_prioridad(string prior) {
        Prioridad = prior;
    }

    void set_solucion_propuesta(string sol) {
        Solucion_propuesta = sol;
    }

    void set_fecha_resolucion(Fecha fecha_res) {
        Fecha_resolucion = fecha_res;
    }
};

// Clase Derivada: Garantia
class Garantia : public Servicio {
private:
    string Codigo_garantia;
    string Numero_comprobante_origen; // ej. Factura o Boleta de compra
    Fecha Fecha_vencimiento_garantia;
    string Motivo_falla;
    bool Cobertura_aprobada;          // Aplica o no aplica la garantia

public:
    Garantia(string id, string nom, string desc, float costo, Fecha fecha, string est,
             string cod_garantia, string num_comp, Fecha fecha_venc, string mot_falla, bool cobertura)
        : Servicio(id, nom, desc, costo, fecha, est) {
        Codigo_garantia = cod_garantia;
        Numero_comprobante_origen = num_comp;
        Fecha_vencimiento_garantia = fecha_venc;
        Motivo_falla = mot_falla;
        Cobertura_aprobada = cobertura;
    }

    // Getters
    string get_codigo_garantia() {
        return Codigo_garantia;
    }

    string get_numero_comprobante_origen() {
        return Numero_comprobante_origen;
    }

    Fecha get_fecha_vencimiento_garantia() {
        return Fecha_vencimiento_garantia;
    }

    string get_motivo_falla() {
        return Motivo_falla;
    }

    bool get_cobertura_aprobada() {
        return Cobertura_aprobada;
    }

    // Setters
    void set_fecha_vencimiento_garantia(Fecha fecha_venc) {
        Fecha_vencimiento_garantia = fecha_venc;
    }

    void set_motivo_falla(string mot) {
        Motivo_falla = mot;
    }

    void set_cobertura_aprobada(bool cob) {
        Cobertura_aprobada = cob;
    }
};

int main() {
    Fecha fechaSol(1, 10, 2026);
    Fecha fechaResol(5, 10, 2026);

    Reclamo r1("SRV-001", "Atencion al cliente", "Problema con el producto", 0.0,
               fechaSol, "En Proceso", "REC-998", "Pantalla rota", "Alta", 
               "Cambio de panel", fechaResol);

    cout << "ID Servicio: " << r1.get_id_servicio() << endl;
    cout << "Numero de reclamo: " << r1.get_numero_reclamo() << endl;
    cout << "Motivo: " << r1.get_motivo() << endl;
    cout << "Fecha de resolucion: ";
    r1.get_fecha_resolucion().imprimir();
    cout << endl;
    return 0;
}