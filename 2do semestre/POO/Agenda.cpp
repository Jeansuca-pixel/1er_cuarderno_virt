//  AGENDA UNIVERSITARIA 
//
//  Conceptos de POO aplicados:
//   - Encapsulamiento : atributos privados/protegidos + getters
//   - Herencia        : Clase, Examen y Tarea heredan de Evento
//   - Abstraccion     : Evento es una clase abstracta (metodos virtuales puros)
//   - Polimorfismo    : Agenda maneja punteros a Evento sin saber su tipo real
//   - Composicion     : Evento contiene Fecha y Hora; Agenda contiene Eventos
//                       Libreta contiene Contactos

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Equivalente a std::make_unique, para compilar tambien con C++11
template <typename T, typename... Args>
unique_ptr<T> crear(Args&&... args) {
    return unique_ptr<T>(new T(forward<Args>(args)...));
}

//  Utilidades
namespace util {

vector<string> dividir(const string& texto, char sep) {
    vector<string> partes;
    string actual;
    for (size_t i = 0; i < texto.size(); ++i) {
        if (texto[i] == sep) {
            partes.push_back(actual);
            actual.clear();
        } else {
            actual += texto[i];
        }
    }
    partes.push_back(actual);  // conserva campos vacios al final
    return partes;
}

string limpiar(string s) {  // el '|' se usa como separador al guardar
    replace(s.begin(), s.end(), '|', '/');
    return s;
}

string aMinusculas(string s) {
    for (size_t i = 0; i < s.size(); ++i)
        s[i] = static_cast<char>(tolower(static_cast<unsigned char>(s[i])));
    return s;
}

string recortar(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

string leerLinea(const string& mensaje) {
    cout << mensaje;
    string linea;
    getline(cin, linea);
    return recortar(linea);
}

int leerEntero(const string& mensaje, int min, int max) {
    while (true) {
        try {
            int n = stoi(leerLinea(mensaje));
            if (n >= min && n <= max) return n;
        } catch (...) {}
        cout << "  -> Ingrese un numero entre " << min << " y " << max << ".\n";
    }
}

}  // namespace util

//  Fecha
class Fecha {
private:
    int dia_, mes_, anio_;

    static bool esBisiesto(int a) { return (a % 4 == 0 && a % 100 != 0) || a % 400 == 0; }
    static int diasEnMes(int m, int a) {
        static const int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        return (m == 2 && esBisiesto(a)) ? 29 : d[m - 1];
    }

public:
    Fecha(int dia = 1, int mes = 1, int anio = 2025) : dia_(dia), mes_(mes), anio_(anio) {
        if (mes < 1 || mes > 12 || anio < 1900 || anio > 2100 || dia < 1 ||
            dia > diasEnMes(mes, anio))
            throw invalid_argument("Fecha invalida");
    }

    static Fecha desdeTexto(const string& s) {  // formato dd/mm/aaaa
        auto p = util::dividir(s, '/');
        if (p.size() != 3) throw invalid_argument("Formato esperado: dd/mm/aaaa");
        try {
            return Fecha(stoi(p[0]), stoi(p[1]), stoi(p[2]));
        } catch (const invalid_argument&) {
            throw invalid_argument("Fecha invalida (formato dd/mm/aaaa)");
        }
    }

    string aTexto() const {
        ostringstream os;
        os << setfill('0') << setw(2) << dia_ << '/' << setw(2) << mes_ << '/' << anio_;
        return os.str();
    }

    bool operator==(const Fecha& o) const {
        return dia_ == o.dia_ && mes_ == o.mes_ && anio_ == o.anio_;
    }
    bool operator<(const Fecha& o) const {
        if (anio_ != o.anio_) return anio_ < o.anio_;
        if (mes_ != o.mes_) return mes_ < o.mes_;
        return dia_ < o.dia_;
    }
};

//  Hora
class Hora {
private:
    int hh_, mm_;

public:
    Hora(int hh = 0, int mm = 0) : hh_(hh), mm_(mm) {
        if (hh < 0 || hh > 23 || mm < 0 || mm > 59) throw invalid_argument("Hora invalida");
    }

    static Hora desdeTexto(const string& s) {  // formato hh:mm
        auto p = util::dividir(s, ':');
        if (p.size() != 2) throw invalid_argument("Formato esperado: hh:mm");
        try {
            return Hora(stoi(p[0]), stoi(p[1]));
        } catch (const invalid_argument&) {
            throw invalid_argument("Hora invalida (formato hh:mm)");
        }
    }

    string aTexto() const {
        ostringstream os;
        os << setfill('0') << setw(2) << hh_ << ':' << setw(2) << mm_;
        return os.str();
    }

    bool operator<(const Hora& o) const {
        return hh_ != o.hh_ ? hh_ < o.hh_ : mm_ < o.mm_;
    }
};

//  Evento (clase abstracta base)
class Evento {
protected:
    int id_;
    string titulo_;
    Fecha fecha_;
    Hora hora_;
    string descripcion_;

public:
    Evento(int id, string titulo, Fecha fecha, Hora hora, string descripcion)
        : id_(id), titulo_(util::limpiar(move(titulo))), fecha_(fecha), hora_(hora),
          descripcion_(util::limpiar(move(descripcion))) {}

    virtual ~Evento() = default;

    // Metodos virtuales puros: cada subclase define su comportamiento
    virtual string tipo() const = 0;
    virtual string detalles() const = 0;
    virtual string extrasSerializados() const = 0;

    // Getters
    int id() const { return id_; }
    const string& titulo() const { return titulo_; }
    const Fecha& fecha() const { return fecha_; }
    const Hora& hora() const { return hora_; }

    // Metodo plantilla: estructura comun + detalles propios de cada tipo
    void mostrar() const {
        cout << "[" << setw(2) << id_ << "] " << left << setw(8) << tipo() << right << " | "
             << fecha_.aTexto() << " " << hora_.aTexto() << " | " << titulo_ << "\n";
        string det = detalles();
        if (!det.empty()) cout << "       " << det << "\n";
        if (!descripcion_.empty()) cout << "       Nota: " << descripcion_ << "\n";
    }

    string serializar() const {
        return tipo() + "|" + to_string(id_) + "|" + titulo_ + "|" + fecha_.aTexto() + "|" +
               hora_.aTexto() + "|" + descripcion_ + "|" + extrasSerializados();
    }

    bool operator<(const Evento& o) const {
        if (fecha_ == o.fecha_) return hora_ < o.hora_;
        return fecha_ < o.fecha_;
    }
};

//  Clase (sesion de clases)
class Clase : public Evento {
private:
    string curso_, aula_, docente_;

public:
    Clase(int id, string titulo, Fecha f, Hora h, string desc, string curso, string aula,
          string docente)
        : Evento(id, move(titulo), f, h, move(desc)), curso_(util::limpiar(move(curso))),
          aula_(util::limpiar(move(aula))), docente_(util::limpiar(move(docente))) {}

    string tipo() const override { return "CLASE"; }
    string detalles() const override {
        return "Curso: " + curso_ + " | Aula: " + aula_ + " | Docente: " + docente_;
    }
    string extrasSerializados() const override { return curso_ + "|" + aula_ + "|" + docente_; }
};

//  Examen
class Examen : public Evento {
private:
    string curso_, temas_;
    int peso_;  // porcentaje de la nota final

public:
    Examen(int id, string titulo, Fecha f, Hora h, string desc, string curso, string temas,
           int peso)
        : Evento(id, move(titulo), f, h, move(desc)), curso_(util::limpiar(move(curso))),
          temas_(util::limpiar(move(temas))), peso_(peso) {}

    string tipo() const override { return "EXAMEN"; }
    string detalles() const override {
        return "Curso: " + curso_ + " | Temas: " + temas_ + " | Peso: " + to_string(peso_) + "%";
    }
    string extrasSerializados() const override {
        return curso_ + "|" + temas_ + "|" + to_string(peso_);
    }
};

//  Tarea
class Tarea : public Evento {
private:
    string curso_;
    int prioridad_;  // 1 = alta, 2 = media, 3 = baja
    bool completada_;

    string textoPrioridad() const {
        return prioridad_ == 1 ? "Alta" : (prioridad_ == 2 ? "Media" : "Baja");
    }

public:
    Tarea(int id, string titulo, Fecha f, Hora h, string desc, string curso, int prioridad,
          bool completada = false)
        : Evento(id, move(titulo), f, h, move(desc)), curso_(util::limpiar(move(curso))),
          prioridad_(prioridad), completada_(completada) {}

    string tipo() const override { return "TAREA"; }
    string detalles() const override {
        return "Curso: " + curso_ + " | Prioridad: " + textoPrioridad() +
               " | Estado: " + (completada_ ? "COMPLETADA" : "Pendiente");
    }
    string extrasSerializados() const override {
        return curso_ + "|" + to_string(prioridad_) + "|" + (completada_ ? "1" : "0");
    }

    void completar() { completada_ = true; }
    bool estaCompletada() const { return completada_; }
};

//  Agenda (gestiona la coleccion de eventos de forma polimorfica)
class Agenda {
private:
    vector<unique_ptr<Evento>> eventos_;
    int proximoId_ = 1;

    void ordenar() {
        sort(eventos_.begin(), eventos_.end(),
             [](const unique_ptr<Evento>& a, const unique_ptr<Evento>& b) { return *a < *b; });
    }

public:
    int generarId() { return proximoId_++; }

    void agregar(unique_ptr<Evento> e) {
        eventos_.push_back(move(e));
        ordenar();
    }

    bool eliminar(int id) {
        auto it = find_if(eventos_.begin(), eventos_.end(),
                          [id](const unique_ptr<Evento>& e) { return e->id() == id; });
        if (it == eventos_.end()) return false;
        eventos_.erase(it);
        return true;
    }

    bool completarTarea(int id) {
        for (auto& e : eventos_) {
            if (e->id() == id) {
                if (auto* t = dynamic_cast<Tarea*>(e.get())) {
                    t->completar();
                    return true;
                }
                return false;  // existe, pero no es una tarea
            }
        }
        return false;
    }

    void listarTodos() const {
        if (eventos_.empty()) {
            cout << "\n  (La agenda esta vacia)\n";
            return;
        }
        cout << "\n=== TODOS LOS EVENTOS ===\n";
        for (const auto& e : eventos_) e->mostrar();
    }

    void listarPorFecha(const Fecha& f) const {
        bool hay = false;
        cout << "\n=== EVENTOS DEL " << f.aTexto() << " ===\n";
        for (const auto& e : eventos_)
            if (e->fecha() == f) {
                e->mostrar();
                hay = true;
            }
        if (!hay) cout << "  (Sin eventos en esa fecha)\n";
    }

    void listarPorTipo(const string& tipo) const {
        bool hay = false;
        cout << "\n=== " << tipo << " ===\n";
        for (const auto& e : eventos_)
            if (e->tipo() == tipo) {
                e->mostrar();
                hay = true;
            }
        if (!hay) cout << "  (Ninguno registrado)\n";
    }

    // ---- Persistencia en archivo de texto ----
    bool guardar(const string& ruta) const {
        ofstream out(ruta);
        if (!out) return false;
        for (const auto& e : eventos_) out << e->serializar() << "\n";
        return true;
    }

    int cargar(const string& ruta) {
        ifstream in(ruta);
        if (!in) return 0;
        string linea;
        int cargados = 0;
        while (getline(in, linea)) {
            auto p = util::dividir(linea, '|');
            if (p.size() < 9) continue;
            try {
                int id = stoi(p[1]);
                Fecha f = Fecha::desdeTexto(p[3]);
                Hora h = Hora::desdeTexto(p[4]);
                unique_ptr<Evento> ev;
                if (p[0] == "CLASE")
                    ev = crear<Clase>(id, p[2], f, h, p[5], p[6], p[7], p[8]);
                else if (p[0] == "EXAMEN")
                    ev = crear<Examen>(id, p[2], f, h, p[5], p[6], p[7], stoi(p[8]));
                else if (p[0] == "TAREA" && p.size() >= 9)
                    ev = crear<Tarea>(id, p[2], f, h, p[5], p[6], stoi(p[7]),
                                            p.size() > 8 && p[8] == "1");
                if (ev) {
                    proximoId_ = max(proximoId_, id + 1);
                    eventos_.push_back(move(ev));
                    ++cargados;
                }
            } catch (...) { /* linea corrupta: se ignora */ }
        }
        ordenar();
        return cargados;
    }
};

//  Contacto (docentes, companeros, personal administrativo, etc.)
class Contacto {
private:
    int id_;
    string nombre_, rol_, telefono_, correo_, notas_;

public:
    Contacto(int id, string nombre, string rol, string telefono, string correo, string notas)
        : id_(id), nombre_(util::limpiar(move(nombre))), rol_(util::limpiar(move(rol))),
          telefono_(util::limpiar(move(telefono))), correo_(util::limpiar(move(correo))),
          notas_(util::limpiar(move(notas))) {}

    // Validaciones (cadena vacia = dato no registrado)
    static bool telefonoValido(const string& t) {
        if (t.empty()) return true;
        size_t digitos = 0;
        for (size_t i = 0; i < t.size(); ++i) {
            char c = t[i];
            if (isdigit(static_cast<unsigned char>(c))) ++digitos;
            else if (c != '+' && c != ' ' && c != '-') return false;
        }
        return digitos >= 6 && digitos <= 15;
    }

    static bool correoValido(const string& c) {
        if (c.empty()) return true;
        if (c.find(' ') != string::npos) return false;
        size_t arroba = c.find('@');
        if (arroba == string::npos || arroba == 0) return false;
        size_t punto = c.find('.', arroba);
        return punto != string::npos && punto > arroba + 1 && punto < c.size() - 1;
    }

    // Getters
    int id() const { return id_; }
    const string& nombre() const { return nombre_; }
    const string& rol() const { return rol_; }
    const string& telefono() const { return telefono_; }
    const string& correo() const { return correo_; }
    const string& notas() const { return notas_; }

    // Setters (para editar)
    void setNombre(const string& v) { nombre_ = util::limpiar(v); }
    void setRol(const string& v) { rol_ = util::limpiar(v); }
    void setTelefono(const string& v) { telefono_ = util::limpiar(v); }
    void setCorreo(const string& v) { correo_ = util::limpiar(v); }
    void setNotas(const string& v) { notas_ = util::limpiar(v); }

    void mostrar() const {
        cout << "[" << setw(2) << id_ << "] " << nombre_ << " (" << rol_ << ")\n";
        if (!telefono_.empty()) cout << "       Tel:    " << telefono_ << "\n";
        if (!correo_.empty()) cout << "       Correo: " << correo_ << "\n";
        if (!notas_.empty()) cout << "       Notas:  " << notas_ << "\n";
    }

    // Busca el texto (sin distinguir mayusculas) en nombre, rol, telefono y correo
    bool coincide(const string& texto) const {
        string t = util::aMinusculas(texto);
        return util::aMinusculas(nombre_).find(t) != string::npos ||
               util::aMinusculas(rol_).find(t) != string::npos ||
               util::aMinusculas(correo_).find(t) != string::npos ||
               telefono_.find(texto) != string::npos;
    }

    string serializar() const {
        return to_string(id_) + "|" + nombre_ + "|" + rol_ + "|" + telefono_ + "|" + correo_ +
               "|" + notas_;
    }
};

//  Libreta (gestiona la coleccion de contactos)
class Libreta {
private:
    vector<Contacto> contactos_;
    int proximoId_ = 1;

public:
    int generarId() { return proximoId_++; }

    void reordenar() {  // alfabetico por nombre
        sort(contactos_.begin(), contactos_.end(), [](const Contacto& a, const Contacto& b) {
            return util::aMinusculas(a.nombre()) < util::aMinusculas(b.nombre());
        });
    }

    bool existeNombre(const string& nombre, int ignorarId = -1) const {
        string n = util::aMinusculas(nombre);
        for (size_t i = 0; i < contactos_.size(); ++i)
            if (contactos_[i].id() != ignorarId && util::aMinusculas(contactos_[i].nombre()) == n)
                return true;
        return false;
    }

    bool agregar(const Contacto& c) {
        if (existeNombre(c.nombre())) return false;  // evita duplicados
        contactos_.push_back(c);
        reordenar();
        return true;
    }

    bool eliminar(int id) {
        for (size_t i = 0; i < contactos_.size(); ++i)
            if (contactos_[i].id() == id) {
                contactos_.erase(contactos_.begin() + i);
                return true;
            }
        return false;
    }

    Contacto* buscarPorId(int id) {
        for (size_t i = 0; i < contactos_.size(); ++i)
            if (contactos_[i].id() == id) return &contactos_[i];
        return nullptr;
    }

    size_t cantidad() const { return contactos_.size(); }

    void listarTodos() const {
        if (contactos_.empty()) {
            cout << "\n  (No hay contactos registrados)\n";
            return;
        }
        cout << "\n=== CONTACTOS (" << contactos_.size() << ") ===\n";
        for (size_t i = 0; i < contactos_.size(); ++i) contactos_[i].mostrar();
    }

    void listarPorRol(const string& rol) const {
        bool hay = false;
        cout << "\n=== CONTACTOS: " << rol << " ===\n";
        for (size_t i = 0; i < contactos_.size(); ++i)
            if (contactos_[i].rol() == rol) {
                contactos_[i].mostrar();
                hay = true;
            }
        if (!hay) cout << "  (Ninguno registrado)\n";
    }

    void buscar(const string& texto) const {
        bool hay = false;
        cout << "\n=== RESULTADOS PARA \"" << texto << "\" ===\n";
        for (size_t i = 0; i < contactos_.size(); ++i)
            if (contactos_[i].coincide(texto)) {
                contactos_[i].mostrar();
                hay = true;
            }
        if (!hay) cout << "  (Sin coincidencias)\n";
    }

    // ---- Persistencia ----
    bool guardar(const string& ruta) const {
        ofstream out(ruta);
        if (!out) return false;
        for (size_t i = 0; i < contactos_.size(); ++i) out << contactos_[i].serializar() << "\n";
        return true;
    }

    int cargar(const string& ruta) {
        ifstream in(ruta);
        if (!in) return 0;
        string linea;
        int cargados = 0;
        while (getline(in, linea)) {
            vector<string> p = util::dividir(linea, '|');
            if (p.size() < 6) continue;
            try {
                int id = stoi(p[0]);
                contactos_.push_back(Contacto(id, p[1], p[2], p[3], p[4], p[5]));
                proximoId_ = max(proximoId_, id + 1);
                ++cargados;
            } catch (...) { /* linea corrupta: se ignora */ }
        }
        reordenar();
        return cargados;
    }
};

//  Interfaz de consola
Fecha pedirFecha() {
    while (true) {
        try {
            return Fecha::desdeTexto(util::leerLinea("  Fecha (dd/mm/aaaa): "));
        } catch (const exception& e) {
            cout << "  -> " << e.what() << "\n";
        }
    }
}

Hora pedirHora() {
    while (true) {
        try {
            return Hora::desdeTexto(util::leerLinea("  Hora (hh:mm): "));
        } catch (const exception& e) {
            cout << "  -> " << e.what() << "\n";
        }
    }
}

void agregarClase(Agenda& ag) {
    cout << "\n--- Nueva clase ---\n";
    string titulo = util::leerLinea("  Titulo: ");
    Fecha f = pedirFecha();
    Hora h = pedirHora();
    string curso = util::leerLinea("  Curso: ");
    string aula = util::leerLinea("  Aula: ");
    string docente = util::leerLinea("  Docente: ");
    string desc = util::leerLinea("  Nota (opcional): ");
    ag.agregar(crear<Clase>(ag.generarId(), titulo, f, h, desc, curso, aula, docente));
    cout << "  Clase registrada.\n";
}

void agregarExamen(Agenda& ag) {
    cout << "\n--- Nuevo examen ---\n";
    string titulo = util::leerLinea("  Titulo: ");
    Fecha f = pedirFecha();
    Hora h = pedirHora();
    string curso = util::leerLinea("  Curso: ");
    string temas = util::leerLinea("  Temas: ");
    int peso = util::leerEntero("  Peso en la nota final (0-100%): ", 0, 100);
    string desc = util::leerLinea("  Nota (opcional): ");
    ag.agregar(crear<Examen>(ag.generarId(), titulo, f, h, desc, curso, temas, peso));
    cout << "  Examen registrado.\n";
}

void agregarTarea(Agenda& ag) {
    cout << "\n--- Nueva tarea ---\n";
    string titulo = util::leerLinea("  Titulo: ");
    Fecha f = pedirFecha();
    Hora h = pedirHora();
    string curso = util::leerLinea("  Curso: ");
    int prio = util::leerEntero("  Prioridad (1=Alta, 2=Media, 3=Baja): ", 1, 3);
    string desc = util::leerLinea("  Nota (opcional): ");
    ag.agregar(crear<Tarea>(ag.generarId(), titulo, f, h, desc, curso, prio));
    cout << "  Tarea registrada.\n";
}

// ---------------------------- Interfaz de contactos --------------------------
string pedirRol() {
    cout << "  Rol: 1=Docente  2=Companero  3=Administrativo  4=Otro\n";
    switch (util::leerEntero("  Opcion: ", 1, 4)) {
        case 1: return "Docente";
        case 2: return "Companero";
        case 3: return "Administrativo";
        default: return "Otro";
    }
}

string pedirTelefono(const string& mensaje) {
    while (true) {
        string t = util::leerLinea(mensaje);
        if (t == "-" || Contacto::telefonoValido(t)) return t;
        cout << "  -> Telefono invalido (use de 6 a 15 digitos; se permiten + - y espacios).\n";
    }
}

string pedirCorreo(const string& mensaje) {
    while (true) {
        string c = util::leerLinea(mensaje);
        if (c == "-" || Contacto::correoValido(c)) return c;
        cout << "  -> Correo invalido (ejemplo: nombre@universidad.edu).\n";
    }
}

void agregarContacto(Libreta& lib) {
    cout << "\n--- Nuevo contacto ---\n";
    string nombre = util::leerLinea("  Nombre completo: ");
    if (nombre.empty()) {
        cout << "  El nombre no puede estar vacio.\n";
        return;
    }
    if (lib.existeNombre(nombre)) {
        cout << "  Ya existe un contacto con ese nombre.\n";
        return;
    }
    string rol = pedirRol();
    string tel = pedirTelefono("  Telefono (opcional): ");
    string correo = pedirCorreo("  Correo (opcional): ");
    string notas = util::leerLinea("  Notas (opcional): ");
    lib.agregar(Contacto(lib.generarId(), nombre, rol, tel == "-" ? "" : tel,
                         correo == "-" ? "" : correo, notas));
    cout << "  Contacto registrado.\n";
}

void buscarContacto(const Libreta& lib) {
    string texto = util::leerLinea("  Buscar (nombre, rol, telefono o correo): ");
    if (texto.empty()) {
        cout << "  Escriba algo para buscar.\n";
        return;
    }
    lib.buscar(texto);
}

void verContactosPorRol(const Libreta& lib) {
    lib.listarPorRol(pedirRol());
}

void editarContacto(Libreta& lib) {
    int id = util::leerEntero("  ID del contacto a editar: ", 1, 100000);
    Contacto* c = lib.buscarPorId(id);
    if (!c) {
        cout << "  No existe un contacto con ese ID.\n";
        return;
    }
    cout << "\n  Datos actuales:\n";
    c->mostrar();
    cout << "\n  (Enter = dejar igual, '-' = borrar el dato)\n";

    string nombre = util::leerLinea("  Nuevo nombre: ");
    if (!nombre.empty()) {
        if (lib.existeNombre(nombre, id)) cout << "  -> Ese nombre ya existe, se mantiene el actual.\n";
        else c->setNombre(nombre);
    }
    string tel = pedirTelefono("  Nuevo telefono: ");
    if (!tel.empty()) c->setTelefono(tel == "-" ? "" : tel);
    string correo = pedirCorreo("  Nuevo correo: ");
    if (!correo.empty()) c->setCorreo(correo == "-" ? "" : correo);

    cout << "  Nuevo rol: 0=sin cambio  1=Docente  2=Companero  3=Administrativo  4=Otro\n";
    switch (util::leerEntero("  Opcion: ", 0, 4)) {
        case 1: c->setRol("Docente"); break;
        case 2: c->setRol("Companero"); break;
        case 3: c->setRol("Administrativo"); break;
        case 4: c->setRol("Otro"); break;
        default: break;
    }
    string notas = util::leerLinea("  Nuevas notas: ");
    if (!notas.empty()) c->setNotas(notas == "-" ? "" : notas);

    lib.reordenar();
    cout << "  Contacto actualizado.\n";
}

void eliminarContacto(Libreta& lib) {
    int id = util::leerEntero("  ID del contacto a eliminar: ", 1, 100000);
    Contacto* c = lib.buscarPorId(id);
    if (!c) {
        cout << "  No existe un contacto con ese ID.\n";
        return;
    }
    c->mostrar();
    string r = util::leerLinea("  Seguro que desea eliminarlo? (s/n): ");
    if (r == "s" || r == "S") {
        lib.eliminar(id);
        cout << "  Contacto eliminado.\n";
    } else {
        cout << "  Operacion cancelada.\n";
    }
}

// ------------------------------- Menus ---------------------------------------
void menuEventos(Agenda& agenda) {
    int opcion;
    do {
        cout << "\n------------------------- EVENTOS -------------------------\n"
             << " 1. Agregar clase          5. Ver eventos de una fecha\n"
             << " 2. Agregar examen         6. Ver solo las tareas\n"
             << " 3. Agregar tarea          7. Marcar tarea como completada\n"
             << " 4. Ver todos los eventos  8. Eliminar evento\n"
             << " 0. Volver\n";
        opcion = util::leerEntero("Opcion: ", 0, 8);
        switch (opcion) {
            case 1: agregarClase(agenda); break;
            case 2: agregarExamen(agenda); break;
            case 3: agregarTarea(agenda); break;
            case 4: agenda.listarTodos(); break;
            case 5: agenda.listarPorFecha(pedirFecha()); break;
            case 6: agenda.listarPorTipo("TAREA"); break;
            case 7: {
                int id = util::leerEntero("  ID de la tarea: ", 1, 100000);
                cout << (agenda.completarTarea(id) ? "  Tarea completada.\n"
                                                   : "  No existe una tarea con ese ID.\n");
                break;
            }
            case 8: {
                int id = util::leerEntero("  ID del evento a eliminar: ", 1, 100000);
                cout << (agenda.eliminar(id) ? "  Evento eliminado.\n"
                                             : "  No existe un evento con ese ID.\n");
                break;
            }
            default: break;
        }
    } while (opcion != 0);
}

void menuContactos(Libreta& libreta) {
    int opcion;
    do {
        cout << "\n------------------------ CONTACTOS ------------------------\n"
             << " 1. Agregar contacto       4. Ver contactos por rol\n"
             << " 2. Ver todos los contactos 5. Editar contacto\n"
             << " 3. Buscar contacto        6. Eliminar contacto\n"
             << " 0. Volver\n";
        opcion = util::leerEntero("Opcion: ", 0, 6);
        switch (opcion) {
            case 1: agregarContacto(libreta); break;
            case 2: libreta.listarTodos(); break;
            case 3: buscarContacto(libreta); break;
            case 4: verContactosPorRol(libreta); break;
            case 5: editarContacto(libreta); break;
            case 6: eliminarContacto(libreta); break;
            default: break;
        }
    } while (opcion != 0);
}

bool guardarTodo(const Agenda& agenda, const Libreta& libreta, const string& archEventos,
                 const string& archContactos) {
    bool ok1 = agenda.guardar(archEventos);
    bool ok2 = libreta.guardar(archContactos);
    cout << (ok1 && ok2 ? "  Datos guardados en '" + archEventos + "' y '" + archContactos + "'.\n"
                        : "  Error al guardar los archivos.\n");
    return ok1 && ok2;
}

int main() {
    const string ARCH_EVENTOS = "agenda.txt";
    const string ARCH_CONTACTOS = "contactos.txt";
    Agenda agenda;
    Libreta libreta;

    int ne = agenda.cargar(ARCH_EVENTOS);
    int nc = libreta.cargar(ARCH_CONTACTOS);
    if (ne > 0 || nc > 0)
        cout << "Se cargaron " << ne << " eventos y " << nc << " contactos.\n";

    int opcion;
    do {
        cout << "\n=============== AGENDA UNIVERSITARIA ===============\n"
             << " 1. Eventos (clases, examenes, tareas)\n"
             << " 2. Contactos\n"
             << " 3. Guardar\n"
             << " 0. Guardar y salir\n"
             << "====================================================\n";
        opcion = util::leerEntero("Opcion: ", 0, 3);
        switch (opcion) {
            case 1: menuEventos(agenda); break;
            case 2: menuContactos(libreta); break;
            case 3:
            case 0: guardarTodo(agenda, libreta, ARCH_EVENTOS, ARCH_CONTACTOS); break;
            default: break;
        }
    } while (opcion != 0);

    cout << "Hasta pronto!\n";
    return 0;
}