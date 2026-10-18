//  AGENDA UNIVERSITARIA 
//
//  Conceptos de POO aplicados:
//   - Encapsulamiento : atributos privados/protegidos + getters
//   - Herencia        : Clase, Examen y Tarea heredan de Evento
//   - Abstraccion     : Evento es una clase abstracta (metodos virtuales puros)
//   - Polimorfismo    : Agenda maneja punteros a Evento sin saber su tipo real
//   - Composicion     : Evento contiene Fecha y Hora; Agenda contiene Eventos
//                       Libreta contiene Contactos

#include <algorithm>  // para ordenar y buscar
#include <cctype>  // para revisar letras y numeros
#include <fstream>  // para leer y guardar archivos
#include <iomanip>  // para dar formato al texto
#include <iostream>  // para mostrar y leer datos en pantalla
#include <memory>  // para manejar objetos creados con new
#include <sstream>  // para armar textos
#include <stdexcept>  // para manejar errores
#include <string>  // para usar textos
#include <utility>  // para mover datos
#include <vector>  // para usar listas

using namespace std; 

// Crea un objeto y lo guarda en un puntero
template <typename T, typename... Args>  // T es el tipo de objeto, Args son sus datos
unique_ptr<T> crear(Args&&... args) {  // recibe los datos del objeto
    return unique_ptr<T>(new T(forward<Args>(args)...));  // crea el objeto y lo devuelve
}

// Funciones de ayuda
namespace util {

vector<string> dividir(const string& texto, char sep) {  // corta un texto en partes usando un separador
    vector<string> partes;
    string actual;  // parte que se va armando
    for (size_t i = 0; i < texto.size(); ++i) {
        if (texto[i] == sep) {
            partes.push_back(actual);
            actual.clear();
        } else {
            actual += texto[i];
        }
    }
    partes.push_back(actual);  // guarda la ultima parte
    return partes;
}

string limpiar(string s) {  // cambia el simbolo | por / dentro de un texto
    replace(s.begin(), s.end(), '|', '/');
    return s;
}

string aMinusculas(string s) {  // pasa un texto a minusculas
    for (size_t i = 0; i < s.size(); ++i)
        s[i] = static_cast<char>(tolower(static_cast<unsigned char>(s[i])));
    return s;
}

string recortar(const string& s) {  // quita los espacios del inicio y del final
    size_t a = s.find_first_not_of(" \t\r\n");  // busca la primera letra que no es espacio
    if (a == string::npos) return "";  // si solo hay espacios, devuelve vacio
    size_t b = s.find_last_not_of(" \t\r\n");  // busca la ultima letra que no es espacio
    return s.substr(a, b - a + 1);  // devuelve el texto sin los espacios de los lados
}

string leerLinea(const string& mensaje) {  // muestra un mensaje y lee lo que escribe el usuario
    cout << mensaje;
    string linea;
    getline(cin, linea);
    return recortar(linea);
}

int leerEntero(const string& mensaje, int min, int max) {  // pide un numero entre min y max
    while (true) {  // repite hasta que el numero sea correcto
        try {
            int n = stoi(leerLinea(mensaje));  // lee el texto y lo convierte en numero
            if (n >= min && n <= max) return n;  // si esta dentro del rango, lo devuelve
        } catch (...) {}  // si no era un numero, no hace nada
        cout << "  -> Ingrese un numero entre " << min << " y " << max << ".\n";
    }
}

}  // fin de util

// Fecha
class Fecha {  // guarda una fecha
private:
    int dia_, mes_, anio_;

    static bool esBisiesto(int a) { return (a % 4 == 0 && a % 100 != 0) || a % 400 == 0; }  // dice si el anio es bisiesto
    static int diasEnMes(int m, int a) {  // dice cuantos dias tiene un mes
        static const int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  // dias de cada mes
        return (m == 2 && esBisiesto(a)) ? 29 : d[m - 1];  // febrero bisiesto tiene 29, los demas usan la lista
    }

public:
    Fecha(int dia = 1, int mes = 1, int anio = 2025) : dia_(dia), mes_(mes), anio_(anio) {
        if (mes < 1 || mes > 12 || anio < 1900 || anio > 2100 || dia < 1 ||  // revisa que mes, anio y dia sean correctos
            dia > diasEnMes(mes, anio))  // revisa que el dia no pase los dias del mes
            throw invalid_argument("Fecha invalida");  // si algo esta mal, da un error
    }

    static Fecha desdeTexto(const string& s) {  // convierte un texto como 20/10/2026 en una fecha
        auto p = util::dividir(s, '/');  // separa el texto por las barras
        if (p.size() != 3) throw invalid_argument("Formato esperado: dd/mm/aaaa");  // debe tener 3 partes
        try {
            return Fecha(stoi(p[0]), stoi(p[1]), stoi(p[2]));  // crea la fecha con dia, mes y anio
        } catch (const invalid_argument&) {  // si las partes no eran numeros
            throw invalid_argument("Fecha invalida (formato dd/mm/aaaa)");  // da un error con un mensaje claro
        }
    }

    string aTexto() const {  // devuelve la fecha escrita como texto
        ostringstream os;
        os << setfill('0') << setw(2) << dia_ << '/' << setw(2) << mes_ << '/' << anio_;  // escribe dia/mes/anio con dos cifras
        return os.str();
    }

    bool operator==(const Fecha& o) const {  // dice si dos fechas son iguales
        return dia_ == o.dia_ && mes_ == o.mes_ && anio_ == o.anio_;  // iguales si dia, mes y anio coinciden
    }
    bool operator<(const Fecha& o) const {  // dice si esta fecha es anterior a otra
        if (anio_ != o.anio_) return anio_ < o.anio_;  // si los anios son distintos, decide por el anio
        if (mes_ != o.mes_) return mes_ < o.mes_;  // si los meses son distintos, decide por el mes
        return dia_ < o.dia_;  // si no, decide por el dia
    }
};

// Hora
class Hora {  // guarda una hora
private:
    int hh_, mm_;

public:
    Hora(int hh = 0, int mm = 0) : hh_(hh), mm_(mm) {
        if (hh < 0 || hh > 23 || mm < 0 || mm > 59) throw invalid_argument("Hora invalida");  // revisa que la hora y los minutos sean correctos
    }

    static Hora desdeTexto(const string& s) {  // convierte un texto como 10:30 en una hora
        auto p = util::dividir(s, ':');  // separa el texto por los dos puntos
        if (p.size() != 2) throw invalid_argument("Formato esperado: hh:mm");  // debe tener 2 partes
        try {
            return Hora(stoi(p[0]), stoi(p[1]));  // crea la hora con horas y minutos
        } catch (const invalid_argument&) {  // si las partes no eran numeros
            throw invalid_argument("Hora invalida (formato hh:mm)");  // da un error con un mensaje claro
        }
    }

    string aTexto() const {  // devuelve la hora escrita como texto
        ostringstream os;
        os << setfill('0') << setw(2) << hh_ << ':' << setw(2) << mm_;  // escribe hora:minutos con dos cifras
        return os.str();
    }

    bool operator<(const Hora& o) const {  // dice si esta hora es anterior a otra
        return hh_ != o.hh_ ? hh_ < o.hh_ : mm_ < o.mm_;  // compara las horas, y si son iguales compara los minutos
    }
};

// Evento: es la base de Clase, Examen y Tarea
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

    virtual ~Evento() = default;  // permite borrar bien los eventos de cada tipo

    virtual string tipo() const = 0;  // cada tipo de evento dice cual es su nombre
    virtual string detalles() const = 0;  // cada tipo de evento dice sus datos propios
    virtual string extrasSerializados() const = 0;  // cada tipo de evento da sus datos propios para guardarlos

    int id() const { return id_; }
    const string& titulo() const { return titulo_; }
    const Fecha& fecha() const { return fecha_; }
    const Hora& hora() const { return hora_; }

    void mostrar() const {  // muestra el evento en pantalla
        cout << "[" << setw(2) << id_ << "] " << left << setw(8) << tipo() << right << " | "  // muestra el id y el tipo
             << fecha_.aTexto() << " " << hora_.aTexto() << " | " << titulo_ << "\n";
        string det = detalles();
        if (!det.empty()) cout << "       " << det << "\n";
        if (!descripcion_.empty()) cout << "       Nota: " << descripcion_ << "\n";
    }

    string serializar() const {  // arma una linea de texto para guardar el evento en el archivo
        return tipo() + "|" + to_string(id_) + "|" + titulo_ + "|" + fecha_.aTexto() + "|" +  // une tipo, id, titulo y fecha
               hora_.aTexto() + "|" + descripcion_ + "|" + extrasSerializados();
    }

    bool operator<(const Evento& o) const {  // dice si este evento va antes que otro
        if (fecha_ == o.fecha_) return hora_ < o.hora_;  // si la fecha es igual, compara la hora
        return fecha_ < o.fecha_;  // si no, compara la fecha
    }
};

// Clase: una clase de la universidad
class Clase : public Evento {  // una Clase es un tipo de Evento
private:
    string curso_, aula_, docente_;

public:
    Clase(int id, string titulo, Fecha f, Hora h, string desc, string curso, string aula,
          string docente)
        : Evento(id, move(titulo), f, h, move(desc)), curso_(util::limpiar(move(curso))),
          aula_(util::limpiar(move(aula))), docente_(util::limpiar(move(docente))) {}

    string tipo() const override { return "CLASE"; }
    string detalles() const override {  // devuelve los datos propios de la clase
        return "Curso: " + curso_ + " | Aula: " + aula_ + " | Docente: " + docente_;
    }
    string extrasSerializados() const override { return curso_ + "|" + aula_ + "|" + docente_; }  // datos propios para guardar en el archivo
};

// Examen
class Examen : public Evento {  // un Examen es un tipo de Evento
private:
    string curso_, temas_;
    int peso_;  // porcentaje que vale en la nota final

public:
    Examen(int id, string titulo, Fecha f, Hora h, string desc, string curso, string temas,
           int peso)
        : Evento(id, move(titulo), f, h, move(desc)), curso_(util::limpiar(move(curso))),
          temas_(util::limpiar(move(temas))), peso_(peso) {}

    string tipo() const override { return "EXAMEN"; }
    string detalles() const override {  // devuelve los datos propios del examen
        return "Curso: " + curso_ + " | Temas: " + temas_ + " | Peso: " + to_string(peso_) + "%";
    }
    string extrasSerializados() const override {  // datos propios para guardar en el archivo
        return curso_ + "|" + temas_ + "|" + to_string(peso_);
    }
};

// Tarea
class Tarea : public Evento {  // una Tarea es un tipo de Evento
private:
    string curso_;
    int prioridad_;  // 1 = alta, 2 = media, 3 = baja
    bool completada_;  // si la tarea ya se termino

    string textoPrioridad() const {  // devuelve la prioridad en palabras
        return prioridad_ == 1 ? "Alta" : (prioridad_ == 2 ? "Media" : "Baja");  // 1 es Alta, 2 es Media, otro es Baja
    }

public:
    Tarea(int id, string titulo, Fecha f, Hora h, string desc, string curso, int prioridad,
          bool completada = false)  // por defecto la tarea esta pendiente
        : Evento(id, move(titulo), f, h, move(desc)), curso_(util::limpiar(move(curso))),
          prioridad_(prioridad), completada_(completada) {}

    string tipo() const override { return "TAREA"; }
    string detalles() const override {  // devuelve los datos propios de la tarea
        return "Curso: " + curso_ + " | Prioridad: " + textoPrioridad() +
               " | Estado: " + (completada_ ? "COMPLETADA" : "Pendiente");
    }
    string extrasSerializados() const override {  // datos propios para guardar en el archivo
        return curso_ + "|" + to_string(prioridad_) + "|" + (completada_ ? "1" : "0");  // une curso, prioridad y estado (1 = completada)
    }

    void completar() { completada_ = true; }
    bool estaCompletada() const { return completada_; }
};

// Agenda: guarda y maneja todos los eventos
class Agenda {
private:
    vector<unique_ptr<Evento>> eventos_;
    int proximoId_ = 1;  // el siguiente id que se va a usar

    void ordenar() {  // ordena los eventos por fecha y hora
        sort(eventos_.begin(), eventos_.end(),
             [](const unique_ptr<Evento>& a, const unique_ptr<Evento>& b) { return *a < *b; });  // compara dos eventos para saber cual va primero
    }

public:
    int generarId() { return proximoId_++; }  // da un id nuevo y suma uno para el siguiente

    void agregar(unique_ptr<Evento> e) {  // agrega un evento a la lista
        eventos_.push_back(move(e));
        ordenar();
    }

    bool eliminar(int id) {  // borra el evento con ese id
        auto it = find_if(eventos_.begin(), eventos_.end(),
                          [id](const unique_ptr<Evento>& e) { return e->id() == id; });
        if (it == eventos_.end()) return false;
        eventos_.erase(it);
        return true;
    }

    bool completarTarea(int id) {  // marca como completada la tarea con ese id
        for (auto& e : eventos_) {
            if (e->id() == id) {
                if (auto* t = dynamic_cast<Tarea*>(e.get())) {  // revisa si el evento es una tarea
                    t->completar();
                    return true;
                }
                return false;  // el evento existe pero no es una tarea
            }
        }
        return false;
    }

    void listarTodos() const {  // muestra todos los eventos
        if (eventos_.empty()) {
            cout << "\n  (La agenda esta vacia)\n";
            return;
        }
        cout << "\n=== TODOS LOS EVENTOS ===\n";
        for (const auto& e : eventos_) e->mostrar();
    }

    void listarPorFecha(const Fecha& f) const {  // muestra los eventos de una fecha
        bool hay = false;  // guarda si se encontro algun evento
        cout << "\n=== EVENTOS DEL " << f.aTexto() << " ===\n";
        for (const auto& e : eventos_)
            if (e->fecha() == f) {
                e->mostrar();
                hay = true;
            }
        if (!hay) cout << "  (Sin eventos en esa fecha)\n";
    }

    void listarPorTipo(const string& tipo) const {  // muestra los eventos de un tipo
        bool hay = false;  // guarda si se encontro algun evento
        cout << "\n=== " << tipo << " ===\n";
        for (const auto& e : eventos_)
            if (e->tipo() == tipo) {
                e->mostrar();
                hay = true;
            }
        if (!hay) cout << "  (Ninguno registrado)\n";
    }

    bool guardar(const string& ruta) const {  // guarda los eventos en un archivo
        ofstream out(ruta);  // abre el archivo para escribir
        if (!out) return false;
        for (const auto& e : eventos_) out << e->serializar() << "\n";
        return true;
    }

    int cargar(const string& ruta) {  // lee los eventos de un archivo y devuelve cuantos leyo
        ifstream in(ruta);  // abre el archivo para leer
        if (!in) return 0;  // si no existe, no lee nada
        string linea;
        int cargados = 0;
        while (getline(in, linea)) {  // lee el archivo linea por linea
            auto p = util::dividir(linea, '|');  // separa la linea en partes
            if (p.size() < 9) continue;  // si faltan datos, salta esta linea
            try {
                int id = stoi(p[1]);
                Fecha f = Fecha::desdeTexto(p[3]);
                Hora h = Hora::desdeTexto(p[4]);
                unique_ptr<Evento> ev;  // aqui se guarda el evento que se va a crear
                if (p[0] == "CLASE")
                    ev = crear<Clase>(id, p[2], f, h, p[5], p[6], p[7], p[8]);
                else if (p[0] == "EXAMEN")
                    ev = crear<Examen>(id, p[2], f, h, p[5], p[6], p[7], stoi(p[8]));
                else if (p[0] == "TAREA" && p.size() >= 9)
                    ev = crear<Tarea>(id, p[2], f, h, p[5], p[6], stoi(p[7]),
                                            p.size() > 8 && p[8] == "1");  // el ultimo dato dice si esta completada
                if (ev) {
                    proximoId_ = max(proximoId_, id + 1);  // evita repetir ids
                    eventos_.push_back(move(ev));
                    ++cargados;
                }
            } catch (...) { /* si la linea esta mal, se salta */ }  // si hubo un error, ignora la linea
        }
        ordenar();
        return cargados;
    }
};

// Contacto: una persona de contacto
class Contacto {
private:
    int id_;
    string nombre_, rol_, telefono_, correo_, notas_;

public:
    Contacto(int id, string nombre, string rol, string telefono, string correo, string notas)
        : id_(id), nombre_(util::limpiar(move(nombre))), rol_(util::limpiar(move(rol))),
          telefono_(util::limpiar(move(telefono))), correo_(util::limpiar(move(correo))),
          notas_(util::limpiar(move(notas))) {}

    static bool telefonoValido(const string& t) {  // dice si un telefono esta bien escrito
        if (t.empty()) return true;  // vacio esta permitido
        size_t digitos = 0;
        for (size_t i = 0; i < t.size(); ++i) {
            char c = t[i];
            if (isdigit(static_cast<unsigned char>(c))) ++digitos;
            else if (c != '+' && c != ' ' && c != '-') return false;  // si no es numero, + , espacio ni guion, esta mal
        }
        return digitos >= 6 && digitos <= 15;  // debe tener entre 6 y 15 numeros
    }

    static bool correoValido(const string& c) {  // dice si un correo esta bien escrito
        if (c.empty()) return true;  // vacio esta permitido
        if (c.find(' ') != string::npos) return false;  // no puede tener espacios
        size_t arroba = c.find('@');
        if (arroba == string::npos || arroba == 0) return false;  // debe tener arroba y no estar al inicio
        size_t punto = c.find('.', arroba);
        return punto != string::npos && punto > arroba + 1 && punto < c.size() - 1;  // el punto no puede estar pegado a la arroba ni al final
    }

    int id() const { return id_; }
    const string& nombre() const { return nombre_; }
    const string& rol() const { return rol_; }
    const string& telefono() const { return telefono_; }
    const string& correo() const { return correo_; }
    const string& notas() const { return notas_; }

    void setNombre(const string& v) { nombre_ = util::limpiar(v); }
    void setRol(const string& v) { rol_ = util::limpiar(v); }
    void setTelefono(const string& v) { telefono_ = util::limpiar(v); }
    void setCorreo(const string& v) { correo_ = util::limpiar(v); }
    void setNotas(const string& v) { notas_ = util::limpiar(v); }

    void mostrar() const {  // muestra el contacto en pantalla
        cout << "[" << setw(2) << id_ << "] " << nombre_ << " (" << rol_ << ")\n";
        if (!telefono_.empty()) cout << "       Tel:    " << telefono_ << "\n";
        if (!correo_.empty()) cout << "       Correo: " << correo_ << "\n";
        if (!notas_.empty()) cout << "       Notas:  " << notas_ << "\n";
    }

    bool coincide(const string& texto) const {  // dice si el contacto tiene el texto buscado
        string t = util::aMinusculas(texto);
        return util::aMinusculas(nombre_).find(t) != string::npos ||
               util::aMinusculas(rol_).find(t) != string::npos ||
               util::aMinusculas(correo_).find(t) != string::npos ||
               telefono_.find(texto) != string::npos;
    }

    string serializar() const {  // arma una linea de texto para guardar el contacto en el archivo
        return to_string(id_) + "|" + nombre_ + "|" + rol_ + "|" + telefono_ + "|" + correo_ +  // une id, nombre, rol, telefono y correo
               "|" + notas_;
    }
};

// Libreta: guarda y maneja todos los contactos
class Libreta {
private:
    vector<Contacto> contactos_;
    int proximoId_ = 1;  // el siguiente id que se va a usar

public:
    int generarId() { return proximoId_++; }  // da un id nuevo y suma uno para el siguiente

    void reordenar() {  // ordena los contactos por nombre
        sort(contactos_.begin(), contactos_.end(), [](const Contacto& a, const Contacto& b) {
            return util::aMinusculas(a.nombre()) < util::aMinusculas(b.nombre());  // compara los nombres sin importar mayusculas
        });
    }

    bool existeNombre(const string& nombre, int ignorarId = -1) const {  // dice si ya existe un contacto con ese nombre
        string n = util::aMinusculas(nombre);
        for (size_t i = 0; i < contactos_.size(); ++i)
            if (contactos_[i].id() != ignorarId && util::aMinusculas(contactos_[i].nombre()) == n)  // compara nombres (sin contar el id que se ignora)
                return true;
        return false;
    }

    bool agregar(const Contacto& c) {  // agrega un contacto a la lista
        if (existeNombre(c.nombre())) return false;  // si el nombre ya existe, no lo agrega
        contactos_.push_back(c);
        reordenar();
        return true;
    }

    bool eliminar(int id) {  // borra el contacto con ese id
        for (size_t i = 0; i < contactos_.size(); ++i)
            if (contactos_[i].id() == id) {
                contactos_.erase(contactos_.begin() + i);
                return true;
            }
        return false;
    }

    Contacto* buscarPorId(int id) {  // busca un contacto por id y devuelve donde esta
        for (size_t i = 0; i < contactos_.size(); ++i)
            if (contactos_[i].id() == id) return &contactos_[i];
        return nullptr;
    }

    size_t cantidad() const { return contactos_.size(); }

    void listarTodos() const {  // muestra todos los contactos
        if (contactos_.empty()) {
            cout << "\n  (No hay contactos registrados)\n";
            return;
        }
        cout << "\n=== CONTACTOS (" << contactos_.size() << ") ===\n";
        for (size_t i = 0; i < contactos_.size(); ++i) contactos_[i].mostrar();
    }

    void listarPorRol(const string& rol) const {  // muestra los contactos de un rol
        bool hay = false;  // guarda si se encontro algun contacto
        cout << "\n=== CONTACTOS: " << rol << " ===\n";
        for (size_t i = 0; i < contactos_.size(); ++i)
            if (contactos_[i].rol() == rol) {
                contactos_[i].mostrar();
                hay = true;
            }
        if (!hay) cout << "  (Ninguno registrado)\n";
    }

    void buscar(const string& texto) const {  // muestra los contactos que tengan el texto buscado
        bool hay = false;  // guarda si se encontro algun contacto
        cout << "\n=== RESULTADOS PARA \"" << texto << "\" ===\n";
        for (size_t i = 0; i < contactos_.size(); ++i)
            if (contactos_[i].coincide(texto)) {
                contactos_[i].mostrar();
                hay = true;
            }
        if (!hay) cout << "  (Sin coincidencias)\n";
    }

    bool guardar(const string& ruta) const {  // guarda los contactos en un archivo
        ofstream out(ruta);  // abre el archivo para escribir
        if (!out) return false;
        for (size_t i = 0; i < contactos_.size(); ++i) out << contactos_[i].serializar() << "\n";
        return true;
    }

    int cargar(const string& ruta) {  // lee los contactos de un archivo y devuelve cuantos leyo
        ifstream in(ruta);  // abre el archivo para leer
        if (!in) return 0;  // si no existe, no lee nada
        string linea;
        int cargados = 0;
        while (getline(in, linea)) {  // lee el archivo linea por linea
            vector<string> p = util::dividir(linea, '|');  // separa la linea en partes
            if (p.size() < 6) continue;  // si faltan datos, salta esta linea
            try {
                int id = stoi(p[0]);
                contactos_.push_back(Contacto(id, p[1], p[2], p[3], p[4], p[5]));
                proximoId_ = max(proximoId_, id + 1);  // evita repetir ids
                ++cargados;
            } catch (...) { /* si la linea esta mal, se salta */ }  // si hubo un error, ignora la linea
        }
        reordenar();
        return cargados;
    }
};

// Pantalla de eventos
Fecha pedirFecha() {  // pide una fecha hasta que este bien escrita
    while (true) {  // repite hasta que la fecha sea correcta
        try {
            return Fecha::desdeTexto(util::leerLinea("  Fecha (dd/mm/aaaa): "));
        } catch (const exception& e) {  // si la fecha estaba mal
            cout << "  -> " << e.what() << "\n";
        }
    }
}

Hora pedirHora() {  // pide una hora hasta que este bien escrita
    while (true) {  // repite hasta que la hora sea correcta
        try {
            return Hora::desdeTexto(util::leerLinea("  Hora (hh:mm): "));
        } catch (const exception& e) {  // si la hora estaba mal
            cout << "  -> " << e.what() << "\n";
        }
    }
}

void agregarClase(Agenda& ag) {  // pide los datos de una clase y la agrega a la agenda
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

void agregarExamen(Agenda& ag) {  // pide los datos de un examen y lo agrega a la agenda
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

void agregarTarea(Agenda& ag) {  // pide los datos de una tarea y la agrega a la agenda
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

// Pantalla de contactos
string pedirRol() {  // pide elegir un rol y devuelve su nombre
    cout << "  Rol: 1=Docente  2=Companero  3=Administrativo  4=Otro\n";
    switch (util::leerEntero("  Opcion: ", 1, 4)) {
        case 1: return "Docente";
        case 2: return "Companero";
        case 3: return "Administrativo";
        default: return "Otro";
    }
}

string pedirTelefono(const string& mensaje) {  // pide un telefono hasta que este bien escrito
    while (true) {  // repite hasta que el telefono sea correcto
        string t = util::leerLinea(mensaje);
        if (t == "-" || Contacto::telefonoValido(t)) return t;  // si es "-" o esta bien, lo devuelve
        cout << "  -> Telefono invalido (use de 6 a 15 digitos; se permiten + - y espacios).\n";
    }
}

string pedirCorreo(const string& mensaje) {  // pide un correo hasta que este bien escrito
    while (true) {  // repite hasta que el correo sea correcto
        string c = util::leerLinea(mensaje);
        if (c == "-" || Contacto::correoValido(c)) return c;  // si es "-" o esta bien, lo devuelve
        cout << "  -> Correo invalido (ejemplo: nombre@universidad.edu).\n";
    }
}

void agregarContacto(Libreta& lib) {  // pide los datos de un contacto y lo agrega
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
    lib.agregar(Contacto(lib.generarId(), nombre, rol, tel == "-" ? "" : tel,  // crea el contacto (si el telefono es "-", queda vacio)
                         correo == "-" ? "" : correo, notas));  // lo mismo con el correo, y agrega las notas
    cout << "  Contacto registrado.\n";
}

void buscarContacto(const Libreta& lib) {  // pide un texto y busca contactos
    string texto = util::leerLinea("  Buscar (nombre, rol, telefono o correo): ");
    if (texto.empty()) {
        cout << "  Escriba algo para buscar.\n";
        return;
    }
    lib.buscar(texto);
}

void verContactosPorRol(const Libreta& lib) {  // muestra los contactos de un rol
    lib.listarPorRol(pedirRol());
}

void editarContacto(Libreta& lib) {  // cambia los datos de un contacto
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
        if (lib.existeNombre(nombre, id)) cout << "  -> Ese nombre ya existe, se mantiene el actual.\n";  // si el nombre ya lo tiene otro contacto, no lo cambia
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

    lib.reordenar();  // ordena otra vez por si cambio el nombre
    cout << "  Contacto actualizado.\n";
}

void eliminarContacto(Libreta& lib) {  // borra un contacto despues de preguntar
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

// Menus
void menuEventos(Agenda& agenda) {  // menu de eventos
    int opcion;
    do {  // repite el menu hasta elegir 0
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

void menuContactos(Libreta& libreta) {  // menu de contactos
    int opcion;
    do {  // repite el menu hasta elegir 0
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

bool guardarTodo(const Agenda& agenda, const Libreta& libreta, const string& archEventos,  // guarda eventos y contactos en sus archivos
                 const string& archContactos) {
    bool ok1 = agenda.guardar(archEventos);
    bool ok2 = libreta.guardar(archContactos);
    cout << (ok1 && ok2 ? "  Datos guardados en '" + archEventos + "' y '" + archContactos + "'.\n"
                        : "  Error al guardar los archivos.\n");
    return ok1 && ok2;  // devuelve verdadero solo si se guardo todo
}

// Programa principal
int main() {  // aqui empieza el programa
    const string ARCH_EVENTOS = "agenda.txt";
    const string ARCH_CONTACTOS = "contactos.txt";
    Agenda agenda;
    Libreta libreta;

    int ne = agenda.cargar(ARCH_EVENTOS);  // lee los eventos guardados
    int nc = libreta.cargar(ARCH_CONTACTOS);  // lee los contactos guardados
    if (ne > 0 || nc > 0)
        cout << "Se cargaron " << ne << " eventos y " << nc << " contactos.\n";

    int opcion;
    do {  // repite el menu hasta elegir 0
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
            case 3:  // la opcion 3 hace lo mismo que la 0
            case 0: guardarTodo(agenda, libreta, ARCH_EVENTOS, ARCH_CONTACTOS); break;
            default: break;
        }
    } while (opcion != 0);

    cout << "Hasta pronto!\n";
    return 0;
}