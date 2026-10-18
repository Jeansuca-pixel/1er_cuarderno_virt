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

using namespace std;  // para no escribir std:: a cada rato

// Crea un objeto y lo guarda en un puntero
template <typename T, typename... Args>  // T es el tipo de objeto, Args son sus datos
unique_ptr<T> crear(Args&&... args) {  // recibe los datos del objeto
    return unique_ptr<T>(new T(forward<Args>(args)...));  // crea el objeto y lo devuelve
}

// Funciones de ayuda
namespace util {

vector<string> dividir(const string& texto, char sep) {  // corta un texto en partes usando un separador
    vector<string> partes;  // lista donde se guardan las partes
    string actual;  // parte que se va armando
    for (size_t i = 0; i < texto.size(); ++i) {  // recorre cada letra del texto
        if (texto[i] == sep) {  // si la letra es el separador
            partes.push_back(actual);  // guarda la parte armada
            actual.clear();  // empieza una parte nueva
        } else {  // si no es el separador
            actual += texto[i];  // agrega la letra a la parte actual
        }
    }
    partes.push_back(actual);  // guarda la ultima parte
    return partes;  // devuelve todas las partes
}

string limpiar(string s) {  // cambia el simbolo | por / dentro de un texto
    replace(s.begin(), s.end(), '|', '/');  // hace el cambio
    return s;  // devuelve el texto cambiado
}

string aMinusculas(string s) {  // pasa un texto a minusculas
    for (size_t i = 0; i < s.size(); ++i)  // recorre cada letra
        s[i] = static_cast<char>(tolower(static_cast<unsigned char>(s[i])));  // la pasa a minuscula
    return s;  // devuelve el texto en minusculas
}

string recortar(const string& s) {  // quita los espacios del inicio y del final
    size_t a = s.find_first_not_of(" \t\r\n");  // busca la primera letra que no es espacio
    if (a == string::npos) return "";  // si solo hay espacios, devuelve vacio
    size_t b = s.find_last_not_of(" \t\r\n");  // busca la ultima letra que no es espacio
    return s.substr(a, b - a + 1);  // devuelve el texto sin los espacios de los lados
}

string leerLinea(const string& mensaje) {  // muestra un mensaje y lee lo que escribe el usuario
    cout << mensaje;  // muestra el mensaje
    string linea;  // aqui se guarda lo que escribe el usuario
    getline(cin, linea);  // lee la linea completa
    return recortar(linea);  // la devuelve sin espacios de los lados
}

int leerEntero(const string& mensaje, int min, int max) {  // pide un numero entre min y max
    while (true) {  // repite hasta que el numero sea correcto
        try {  // intenta convertir el texto en numero
            int n = stoi(leerLinea(mensaje));  // lee el texto y lo convierte en numero
            if (n >= min && n <= max) return n;  // si esta dentro del rango, lo devuelve
        } catch (...) {}  // si no era un numero, no hace nada
        cout << "  -> Ingrese un numero entre " << min << " y " << max << ".\n";  // avisa que el dato esta mal
    }
}

}  // fin de util

// Fecha
class Fecha {  // guarda una fecha
private:  // datos que solo usa la clase
    int dia_, mes_, anio_;  // dia, mes y anio

    static bool esBisiesto(int a) { return (a % 4 == 0 && a % 100 != 0) || a % 400 == 0; }  // dice si el anio es bisiesto
    static int diasEnMes(int m, int a) {  // dice cuantos dias tiene un mes
        static const int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  // dias de cada mes
        return (m == 2 && esBisiesto(a)) ? 29 : d[m - 1];  // febrero bisiesto tiene 29, los demas usan la lista
    }

public:  // datos y funciones que se pueden usar desde afuera
    Fecha(int dia = 1, int mes = 1, int anio = 2025) : dia_(dia), mes_(mes), anio_(anio) {  // crea la fecha con los datos recibidos
        if (mes < 1 || mes > 12 || anio < 1900 || anio > 2100 || dia < 1 ||  // revisa que mes, anio y dia sean correctos
            dia > diasEnMes(mes, anio))  // revisa que el dia no pase los dias del mes
            throw invalid_argument("Fecha invalida");  // si algo esta mal, da un error
    }

    static Fecha desdeTexto(const string& s) {  // convierte un texto como 20/10/2026 en una fecha
        auto p = util::dividir(s, '/');  // separa el texto por las barras
        if (p.size() != 3) throw invalid_argument("Formato esperado: dd/mm/aaaa");  // debe tener 3 partes
        try {  // intenta convertir las partes en numeros
            return Fecha(stoi(p[0]), stoi(p[1]), stoi(p[2]));  // crea la fecha con dia, mes y anio
        } catch (const invalid_argument&) {  // si las partes no eran numeros
            throw invalid_argument("Fecha invalida (formato dd/mm/aaaa)");  // da un error con un mensaje claro
        }
    }

    string aTexto() const {  // devuelve la fecha escrita como texto
        ostringstream os;  // para armar el texto
        os << setfill('0') << setw(2) << dia_ << '/' << setw(2) << mes_ << '/' << anio_;  // escribe dia/mes/anio con dos cifras
        return os.str();  // devuelve el texto
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
private:  // datos que solo usa la clase
    int hh_, mm_;  // horas y minutos

public:  // datos y funciones que se pueden usar desde afuera
    Hora(int hh = 0, int mm = 0) : hh_(hh), mm_(mm) {  // crea la hora con los datos recibidos
        if (hh < 0 || hh > 23 || mm < 0 || mm > 59) throw invalid_argument("Hora invalida");  // revisa que la hora y los minutos sean correctos
    }

    static Hora desdeTexto(const string& s) {  // convierte un texto como 10:30 en una hora
        auto p = util::dividir(s, ':');  // separa el texto por los dos puntos
        if (p.size() != 2) throw invalid_argument("Formato esperado: hh:mm");  // debe tener 2 partes
        try {  // intenta convertir las partes en numeros
            return Hora(stoi(p[0]), stoi(p[1]));  // crea la hora con horas y minutos
        } catch (const invalid_argument&) {  // si las partes no eran numeros
            throw invalid_argument("Hora invalida (formato hh:mm)");  // da un error con un mensaje claro
        }
    }

    string aTexto() const {  // devuelve la hora escrita como texto
        ostringstream os;  // para armar el texto
        os << setfill('0') << setw(2) << hh_ << ':' << setw(2) << mm_;  // escribe hora:minutos con dos cifras
        return os.str();  // devuelve el texto
    }

    bool operator<(const Hora& o) const {  // dice si esta hora es anterior a otra
        return hh_ != o.hh_ ? hh_ < o.hh_ : mm_ < o.mm_;  // compara las horas, y si son iguales compara los minutos
    }
};

// Evento: es la base de Clase, Examen y Tarea
class Evento {
protected:  // datos que pueden usar las clases que heredan de esta
    int id_;  // numero que identifica al evento
    string titulo_;  // titulo del evento
    Fecha fecha_;  // fecha del evento
    Hora hora_;  // hora del evento
    string descripcion_;  // nota del evento

public:  // datos y funciones que se pueden usar desde afuera
    Evento(int id, string titulo, Fecha fecha, Hora hora, string descripcion)  // crea un evento con estos datos
        : id_(id), titulo_(util::limpiar(move(titulo))), fecha_(fecha), hora_(hora),  // guarda id, titulo, fecha y hora
          descripcion_(util::limpiar(move(descripcion))) {}  // guarda la nota

    virtual ~Evento() = default;  // permite borrar bien los eventos de cada tipo

    virtual string tipo() const = 0;  // cada tipo de evento dice cual es su nombre
    virtual string detalles() const = 0;  // cada tipo de evento dice sus datos propios
    virtual string extrasSerializados() const = 0;  // cada tipo de evento da sus datos propios para guardarlos

    int id() const { return id_; }  // devuelve el id
    const string& titulo() const { return titulo_; }  // devuelve el titulo
    const Fecha& fecha() const { return fecha_; }  // devuelve la fecha
    const Hora& hora() const { return hora_; }  // devuelve la hora

    void mostrar() const {  // muestra el evento en pantalla
        cout << "[" << setw(2) << id_ << "] " << left << setw(8) << tipo() << right << " | "  // muestra el id y el tipo
             << fecha_.aTexto() << " " << hora_.aTexto() << " | " << titulo_ << "\n";  // muestra la fecha, la hora y el titulo
        string det = detalles();  // pide los datos propios del tipo de evento
        if (!det.empty()) cout << "       " << det << "\n";  // si hay datos, los muestra
        if (!descripcion_.empty()) cout << "       Nota: " << descripcion_ << "\n";  // si hay nota, la muestra
    }

    string serializar() const {  // arma una linea de texto para guardar el evento en el archivo
        return tipo() + "|" + to_string(id_) + "|" + titulo_ + "|" + fecha_.aTexto() + "|" +  // une tipo, id, titulo y fecha
               hora_.aTexto() + "|" + descripcion_ + "|" + extrasSerializados();  // une hora, nota y datos propios
    }

    bool operator<(const Evento& o) const {  // dice si este evento va antes que otro
        if (fecha_ == o.fecha_) return hora_ < o.hora_;  // si la fecha es igual, compara la hora
        return fecha_ < o.fecha_;  // si no, compara la fecha
    }
};

// Clase: una clase de la universidad
class Clase : public Evento {  // una Clase es un tipo de Evento
private:  // datos propios de la clase
    string curso_, aula_, docente_;  // curso, aula y docente

public:  // datos y funciones que se pueden usar desde afuera
    Clase(int id, string titulo, Fecha f, Hora h, string desc, string curso, string aula,  // crea una clase con estos datos
          string docente)
        : Evento(id, move(titulo), f, h, move(desc)), curso_(util::limpiar(move(curso))),  // guarda los datos generales y el curso
          aula_(util::limpiar(move(aula))), docente_(util::limpiar(move(docente))) {}  // guarda el aula y el docente

    string tipo() const override { return "CLASE"; }  // el tipo de evento es CLASE
    string detalles() const override {  // devuelve los datos propios de la clase
        return "Curso: " + curso_ + " | Aula: " + aula_ + " | Docente: " + docente_;  // une curso, aula y docente
    }
    string extrasSerializados() const override { return curso_ + "|" + aula_ + "|" + docente_; }  // datos propios para guardar en el archivo
};

// Examen
class Examen : public Evento {  // un Examen es un tipo de Evento
private:  // datos propios del examen
    string curso_, temas_;  // curso y temas
    int peso_;  // porcentaje que vale en la nota final

public:  // datos y funciones que se pueden usar desde afuera
    Examen(int id, string titulo, Fecha f, Hora h, string desc, string curso, string temas,  // crea un examen con estos datos
           int peso)
        : Evento(id, move(titulo), f, h, move(desc)), curso_(util::limpiar(move(curso))),  // guarda los datos generales y el curso
          temas_(util::limpiar(move(temas))), peso_(peso) {}  // guarda los temas y el peso

    string tipo() const override { return "EXAMEN"; }  // el tipo de evento es EXAMEN
    string detalles() const override {  // devuelve los datos propios del examen
        return "Curso: " + curso_ + " | Temas: " + temas_ + " | Peso: " + to_string(peso_) + "%";  // une curso, temas y peso
    }
    string extrasSerializados() const override {  // datos propios para guardar en el archivo
        return curso_ + "|" + temas_ + "|" + to_string(peso_);  // une curso, temas y peso
    }
};

// Tarea
class Tarea : public Evento {  // una Tarea es un tipo de Evento
private:  // datos propios de la tarea
    string curso_;  // curso de la tarea
    int prioridad_;  // 1 = alta, 2 = media, 3 = baja
    bool completada_;  // si la tarea ya se termino

    string textoPrioridad() const {  // devuelve la prioridad en palabras
        return prioridad_ == 1 ? "Alta" : (prioridad_ == 2 ? "Media" : "Baja");  // 1 es Alta, 2 es Media, otro es Baja
    }

public:  // datos y funciones que se pueden usar desde afuera
    Tarea(int id, string titulo, Fecha f, Hora h, string desc, string curso, int prioridad,  // crea una tarea con estos datos
          bool completada = false)  // por defecto la tarea esta pendiente
        : Evento(id, move(titulo), f, h, move(desc)), curso_(util::limpiar(move(curso))),  // guarda los datos generales y el curso
          prioridad_(prioridad), completada_(completada) {}  // guarda la prioridad y el estado

    string tipo() const override { return "TAREA"; }  // el tipo de evento es TAREA
    string detalles() const override {  // devuelve los datos propios de la tarea
        return "Curso: " + curso_ + " | Prioridad: " + textoPrioridad() +  // une el curso y la prioridad
               " | Estado: " + (completada_ ? "COMPLETADA" : "Pendiente");  // agrega si esta completada o pendiente
    }
    string extrasSerializados() const override {  // datos propios para guardar en el archivo
        return curso_ + "|" + to_string(prioridad_) + "|" + (completada_ ? "1" : "0");  // une curso, prioridad y estado (1 = completada)
    }

    void completar() { completada_ = true; }  // marca la tarea como completada
    bool estaCompletada() const { return completada_; }  // dice si la tarea esta completada
};

// Agenda: guarda y maneja todos los eventos
class Agenda {
private:  // datos propios de la agenda
    vector<unique_ptr<Evento>> eventos_;  // lista de todos los eventos
    int proximoId_ = 1;  // el siguiente id que se va a usar

    void ordenar() {  // ordena los eventos por fecha y hora
        sort(eventos_.begin(), eventos_.end(),  // ordena la lista de eventos
             [](const unique_ptr<Evento>& a, const unique_ptr<Evento>& b) { return *a < *b; });  // compara dos eventos para saber cual va primero
    }

public:  // datos y funciones que se pueden usar desde afuera
    int generarId() { return proximoId_++; }  // da un id nuevo y suma uno para el siguiente

    void agregar(unique_ptr<Evento> e) {  // agrega un evento a la lista
        eventos_.push_back(move(e));  // lo pone en la lista
        ordenar();  // vuelve a ordenar la lista
    }

    bool eliminar(int id) {  // borra el evento con ese id
        auto it = find_if(eventos_.begin(), eventos_.end(),  // busca el evento en la lista
                          [id](const unique_ptr<Evento>& e) { return e->id() == id; });  // el que tenga el mismo id
        if (it == eventos_.end()) return false;  // si no lo encuentra, devuelve falso
        eventos_.erase(it);  // lo borra de la lista
        return true;  // avisa que se borro
    }

    bool completarTarea(int id) {  // marca como completada la tarea con ese id
        for (auto& e : eventos_) {  // recorre todos los eventos
            if (e->id() == id) {  // si encuentra el id
                if (auto* t = dynamic_cast<Tarea*>(e.get())) {  // revisa si el evento es una tarea
                    t->completar();  // marca la tarea como completada
                    return true;  // avisa que se pudo
                }
                return false;  // el evento existe pero no es una tarea
            }
        }
        return false;  // no existe un evento con ese id
    }

    void listarTodos() const {  // muestra todos los eventos
        if (eventos_.empty()) {  // si no hay eventos
            cout << "\n  (La agenda esta vacia)\n";  // avisa que esta vacia
            return;  // termina
        }
        cout << "\n=== TODOS LOS EVENTOS ===\n";  // titulo de la lista
        for (const auto& e : eventos_) e->mostrar();  // muestra cada evento
    }

    void listarPorFecha(const Fecha& f) const {  // muestra los eventos de una fecha
        bool hay = false;  // guarda si se encontro algun evento
        cout << "\n=== EVENTOS DEL " << f.aTexto() << " ===\n";  // titulo con la fecha
        for (const auto& e : eventos_)  // recorre los eventos
            if (e->fecha() == f) {  // si el evento es de esa fecha
                e->mostrar();  // lo muestra
                hay = true;  // anota que se encontro uno
            }
        if (!hay) cout << "  (Sin eventos en esa fecha)\n";  // si no hubo ninguno, avisa
    }

    void listarPorTipo(const string& tipo) const {  // muestra los eventos de un tipo
        bool hay = false;  // guarda si se encontro algun evento
        cout << "\n=== " << tipo << " ===\n";  // titulo con el tipo
        for (const auto& e : eventos_)  // recorre los eventos
            if (e->tipo() == tipo) {  // si el evento es de ese tipo
                e->mostrar();  // lo muestra
                hay = true;  // anota que se encontro uno
            }
        if (!hay) cout << "  (Ninguno registrado)\n";  // si no hubo ninguno, avisa
    }

    bool guardar(const string& ruta) const {  // guarda los eventos en un archivo
        ofstream out(ruta);  // abre el archivo para escribir
        if (!out) return false;  // si no se pudo abrir, devuelve falso
        for (const auto& e : eventos_) out << e->serializar() << "\n";  // escribe una linea por cada evento
        return true;  // avisa que se guardo bien
    }

    int cargar(const string& ruta) {  // lee los eventos de un archivo y devuelve cuantos leyo
        ifstream in(ruta);  // abre el archivo para leer
        if (!in) return 0;  // si no existe, no lee nada
        string linea;  // aqui se guarda cada linea del archivo
        int cargados = 0;  // cuenta los eventos leidos
        while (getline(in, linea)) {  // lee el archivo linea por linea
            auto p = util::dividir(linea, '|');  // separa la linea en partes
            if (p.size() < 9) continue;  // si faltan datos, salta esta linea
            try {  // intenta leer la linea
                int id = stoi(p[1]);  // el id
                Fecha f = Fecha::desdeTexto(p[3]);  // la fecha
                Hora h = Hora::desdeTexto(p[4]);  // la hora
                unique_ptr<Evento> ev;  // aqui se guarda el evento que se va a crear
                if (p[0] == "CLASE")  // si el tipo es CLASE
                    ev = crear<Clase>(id, p[2], f, h, p[5], p[6], p[7], p[8]);  // crea una clase con los datos leidos
                else if (p[0] == "EXAMEN")  // si el tipo es EXAMEN
                    ev = crear<Examen>(id, p[2], f, h, p[5], p[6], p[7], stoi(p[8]));  // crea un examen con los datos leidos
                else if (p[0] == "TAREA" && p.size() >= 9)  // si el tipo es TAREA
                    ev = crear<Tarea>(id, p[2], f, h, p[5], p[6], stoi(p[7]),  // crea una tarea con los datos leidos
                                            p.size() > 8 && p[8] == "1");  // el ultimo dato dice si esta completada
                if (ev) {  // si se creo un evento
                    proximoId_ = max(proximoId_, id + 1);  // evita repetir ids
                    eventos_.push_back(move(ev));  // lo agrega a la lista
                    ++cargados;  // suma uno al contador
                }
            } catch (...) { /* si la linea esta mal, se salta */ }  // si hubo un error, ignora la linea
        }
        ordenar();  // ordena los eventos
        return cargados;  // devuelve cuantos eventos leyo
    }
};

// Contacto: una persona de contacto
class Contacto {
private:  // datos propios del contacto
    int id_;  // numero que identifica al contacto
    string nombre_, rol_, telefono_, correo_, notas_;  // nombre, rol, telefono, correo y notas

public:  // datos y funciones que se pueden usar desde afuera
    Contacto(int id, string nombre, string rol, string telefono, string correo, string notas)  // crea un contacto con estos datos
        : id_(id), nombre_(util::limpiar(move(nombre))), rol_(util::limpiar(move(rol))),  // guarda id, nombre y rol
          telefono_(util::limpiar(move(telefono))), correo_(util::limpiar(move(correo))),  // guarda telefono y correo
          notas_(util::limpiar(move(notas))) {}  // guarda las notas

    static bool telefonoValido(const string& t) {  // dice si un telefono esta bien escrito
        if (t.empty()) return true;  // vacio esta permitido
        size_t digitos = 0;  // cuenta los numeros del telefono
        for (size_t i = 0; i < t.size(); ++i) {  // recorre cada letra
            char c = t[i];  // letra actual
            if (isdigit(static_cast<unsigned char>(c))) ++digitos;  // si es un numero, lo cuenta
            else if (c != '+' && c != ' ' && c != '-') return false;  // si no es numero, + , espacio ni guion, esta mal
        }
        return digitos >= 6 && digitos <= 15;  // debe tener entre 6 y 15 numeros
    }

    static bool correoValido(const string& c) {  // dice si un correo esta bien escrito
        if (c.empty()) return true;  // vacio esta permitido
        if (c.find(' ') != string::npos) return false;  // no puede tener espacios
        size_t arroba = c.find('@');  // busca la arroba
        if (arroba == string::npos || arroba == 0) return false;  // debe tener arroba y no estar al inicio
        size_t punto = c.find('.', arroba);  // busca un punto despues de la arroba
        return punto != string::npos && punto > arroba + 1 && punto < c.size() - 1;  // el punto no puede estar pegado a la arroba ni al final
    }

    int id() const { return id_; }  // devuelve el id
    const string& nombre() const { return nombre_; }  // devuelve el nombre
    const string& rol() const { return rol_; }  // devuelve el rol
    const string& telefono() const { return telefono_; }  // devuelve el telefono
    const string& correo() const { return correo_; }  // devuelve el correo
    const string& notas() const { return notas_; }  // devuelve las notas

    void setNombre(const string& v) { nombre_ = util::limpiar(v); }  // cambia el nombre
    void setRol(const string& v) { rol_ = util::limpiar(v); }  // cambia el rol
    void setTelefono(const string& v) { telefono_ = util::limpiar(v); }  // cambia el telefono
    void setCorreo(const string& v) { correo_ = util::limpiar(v); }  // cambia el correo
    void setNotas(const string& v) { notas_ = util::limpiar(v); }  // cambia las notas

    void mostrar() const {  // muestra el contacto en pantalla
        cout << "[" << setw(2) << id_ << "] " << nombre_ << " (" << rol_ << ")\n";  // muestra id, nombre y rol
        if (!telefono_.empty()) cout << "       Tel:    " << telefono_ << "\n";  // muestra el telefono si hay
        if (!correo_.empty()) cout << "       Correo: " << correo_ << "\n";  // muestra el correo si hay
        if (!notas_.empty()) cout << "       Notas:  " << notas_ << "\n";  // muestra las notas si hay
    }

    bool coincide(const string& texto) const {  // dice si el contacto tiene el texto buscado
        string t = util::aMinusculas(texto);  // pasa lo buscado a minusculas
        return util::aMinusculas(nombre_).find(t) != string::npos ||  // lo busca en el nombre
               util::aMinusculas(rol_).find(t) != string::npos ||  // o en el rol
               util::aMinusculas(correo_).find(t) != string::npos ||  // o en el correo
               telefono_.find(texto) != string::npos;  // o en el telefono
    }

    string serializar() const {  // arma una linea de texto para guardar el contacto en el archivo
        return to_string(id_) + "|" + nombre_ + "|" + rol_ + "|" + telefono_ + "|" + correo_ +  // une id, nombre, rol, telefono y correo
               "|" + notas_;  // agrega las notas
    }
};

// Libreta: guarda y maneja todos los contactos
class Libreta {
private:  // datos propios de la libreta
    vector<Contacto> contactos_;  // lista de contactos
    int proximoId_ = 1;  // el siguiente id que se va a usar

public:  // datos y funciones que se pueden usar desde afuera
    int generarId() { return proximoId_++; }  // da un id nuevo y suma uno para el siguiente

    void reordenar() {  // ordena los contactos por nombre
        sort(contactos_.begin(), contactos_.end(), [](const Contacto& a, const Contacto& b) {  // ordena la lista de contactos
            return util::aMinusculas(a.nombre()) < util::aMinusculas(b.nombre());  // compara los nombres sin importar mayusculas
        });
    }

    bool existeNombre(const string& nombre, int ignorarId = -1) const {  // dice si ya existe un contacto con ese nombre
        string n = util::aMinusculas(nombre);  // pasa el nombre a minusculas
        for (size_t i = 0; i < contactos_.size(); ++i)  // recorre los contactos
            if (contactos_[i].id() != ignorarId && util::aMinusculas(contactos_[i].nombre()) == n)  // compara nombres (sin contar el id que se ignora)
                return true;  // encontro uno igual
        return false;  // no hay ninguno igual
    }

    bool agregar(const Contacto& c) {  // agrega un contacto a la lista
        if (existeNombre(c.nombre())) return false;  // si el nombre ya existe, no lo agrega
        contactos_.push_back(c);  // lo pone en la lista
        reordenar();  // vuelve a ordenar la lista
        return true;  // avisa que se agrego
    }

    bool eliminar(int id) {  // borra el contacto con ese id
        for (size_t i = 0; i < contactos_.size(); ++i)  // recorre los contactos
            if (contactos_[i].id() == id) {  // si encuentra el id
                contactos_.erase(contactos_.begin() + i);  // lo borra de la lista
                return true;  // avisa que se borro
            }
        return false;  // no existe un contacto con ese id
    }

    Contacto* buscarPorId(int id) {  // busca un contacto por id y devuelve donde esta
        for (size_t i = 0; i < contactos_.size(); ++i)  // recorre los contactos
            if (contactos_[i].id() == id) return &contactos_[i];  // si el id coincide, lo devuelve
        return nullptr;  // si no lo encuentra, devuelve nada
    }

    size_t cantidad() const { return contactos_.size(); }  // devuelve cuantos contactos hay

    void listarTodos() const {  // muestra todos los contactos
        if (contactos_.empty()) {  // si no hay contactos
            cout << "\n  (No hay contactos registrados)\n";  // avisa que no hay
            return;  // termina
        }
        cout << "\n=== CONTACTOS (" << contactos_.size() << ") ===\n";  // titulo con la cantidad
        for (size_t i = 0; i < contactos_.size(); ++i) contactos_[i].mostrar();  // muestra cada contacto
    }

    void listarPorRol(const string& rol) const {  // muestra los contactos de un rol
        bool hay = false;  // guarda si se encontro algun contacto
        cout << "\n=== CONTACTOS: " << rol << " ===\n";  // titulo con el rol
        for (size_t i = 0; i < contactos_.size(); ++i)  // recorre los contactos
            if (contactos_[i].rol() == rol) {  // si el contacto tiene ese rol
                contactos_[i].mostrar();  // lo muestra
                hay = true;  // anota que se encontro uno
            }
        if (!hay) cout << "  (Ninguno registrado)\n";  // si no hubo ninguno, avisa
    }

    void buscar(const string& texto) const {  // muestra los contactos que tengan el texto buscado
        bool hay = false;  // guarda si se encontro algun contacto
        cout << "\n=== RESULTADOS PARA \"" << texto << "\" ===\n";  // titulo con lo buscado
        for (size_t i = 0; i < contactos_.size(); ++i)  // recorre los contactos
            if (contactos_[i].coincide(texto)) {  // si el contacto tiene el texto
                contactos_[i].mostrar();  // lo muestra
                hay = true;  // anota que se encontro uno
            }
        if (!hay) cout << "  (Sin coincidencias)\n";  // si no hubo ninguno, avisa
    }

    bool guardar(const string& ruta) const {  // guarda los contactos en un archivo
        ofstream out(ruta);  // abre el archivo para escribir
        if (!out) return false;  // si no se pudo abrir, devuelve falso
        for (size_t i = 0; i < contactos_.size(); ++i) out << contactos_[i].serializar() << "\n";  // escribe una linea por cada contacto
        return true;  // avisa que se guardo bien
    }

    int cargar(const string& ruta) {  // lee los contactos de un archivo y devuelve cuantos leyo
        ifstream in(ruta);  // abre el archivo para leer
        if (!in) return 0;  // si no existe, no lee nada
        string linea;  // aqui se guarda cada linea del archivo
        int cargados = 0;  // cuenta los contactos leidos
        while (getline(in, linea)) {  // lee el archivo linea por linea
            vector<string> p = util::dividir(linea, '|');  // separa la linea en partes
            if (p.size() < 6) continue;  // si faltan datos, salta esta linea
            try {  // intenta leer la linea
                int id = stoi(p[0]);  // el id
                contactos_.push_back(Contacto(id, p[1], p[2], p[3], p[4], p[5]));  // crea el contacto y lo agrega a la lista
                proximoId_ = max(proximoId_, id + 1);  // evita repetir ids
                ++cargados;  // suma uno al contador
            } catch (...) { /* si la linea esta mal, se salta */ }  // si hubo un error, ignora la linea
        }
        reordenar();  // ordena los contactos
        return cargados;  // devuelve cuantos contactos leyo
    }
};

// Pantalla de eventos
Fecha pedirFecha() {  // pide una fecha hasta que este bien escrita
    while (true) {  // repite hasta que la fecha sea correcta
        try {  // intenta crear la fecha
            return Fecha::desdeTexto(util::leerLinea("  Fecha (dd/mm/aaaa): "));  // pide el texto y lo convierte en fecha
        } catch (const exception& e) {  // si la fecha estaba mal
            cout << "  -> " << e.what() << "\n";  // muestra el error y vuelve a pedir
        }
    }
}

Hora pedirHora() {  // pide una hora hasta que este bien escrita
    while (true) {  // repite hasta que la hora sea correcta
        try {  // intenta crear la hora
            return Hora::desdeTexto(util::leerLinea("  Hora (hh:mm): "));  // pide el texto y lo convierte en hora
        } catch (const exception& e) {  // si la hora estaba mal
            cout << "  -> " << e.what() << "\n";  // muestra el error y vuelve a pedir
        }
    }
}

void agregarClase(Agenda& ag) {  // pide los datos de una clase y la agrega a la agenda
    cout << "\n--- Nueva clase ---\n";  // titulo
    string titulo = util::leerLinea("  Titulo: ");  // pide el titulo
    Fecha f = pedirFecha();  // pide la fecha
    Hora h = pedirHora();  // pide la hora
    string curso = util::leerLinea("  Curso: ");  // pide el curso
    string aula = util::leerLinea("  Aula: ");  // pide el aula
    string docente = util::leerLinea("  Docente: ");  // pide el docente
    string desc = util::leerLinea("  Nota (opcional): ");  // pide una nota
    ag.agregar(crear<Clase>(ag.generarId(), titulo, f, h, desc, curso, aula, docente));  // crea la clase y la agrega
    cout << "  Clase registrada.\n";  // avisa que se guardo
}

void agregarExamen(Agenda& ag) {  // pide los datos de un examen y lo agrega a la agenda
    cout << "\n--- Nuevo examen ---\n";  // titulo
    string titulo = util::leerLinea("  Titulo: ");  // pide el titulo
    Fecha f = pedirFecha();  // pide la fecha
    Hora h = pedirHora();  // pide la hora
    string curso = util::leerLinea("  Curso: ");  // pide el curso
    string temas = util::leerLinea("  Temas: ");  // pide los temas
    int peso = util::leerEntero("  Peso en la nota final (0-100%): ", 0, 100);  // pide el peso del examen
    string desc = util::leerLinea("  Nota (opcional): ");  // pide una nota
    ag.agregar(crear<Examen>(ag.generarId(), titulo, f, h, desc, curso, temas, peso));  // crea el examen y lo agrega
    cout << "  Examen registrado.\n";  // avisa que se guardo
}

void agregarTarea(Agenda& ag) {  // pide los datos de una tarea y la agrega a la agenda
    cout << "\n--- Nueva tarea ---\n";  // titulo
    string titulo = util::leerLinea("  Titulo: ");  // pide el titulo
    Fecha f = pedirFecha();  // pide la fecha
    Hora h = pedirHora();  // pide la hora
    string curso = util::leerLinea("  Curso: ");  // pide el curso
    int prio = util::leerEntero("  Prioridad (1=Alta, 2=Media, 3=Baja): ", 1, 3);  // pide la prioridad
    string desc = util::leerLinea("  Nota (opcional): ");  // pide una nota
    ag.agregar(crear<Tarea>(ag.generarId(), titulo, f, h, desc, curso, prio));  // crea la tarea y la agrega
    cout << "  Tarea registrada.\n";  // avisa que se guardo
}

// Pantalla de contactos
string pedirRol() {  // pide elegir un rol y devuelve su nombre
    cout << "  Rol: 1=Docente  2=Companero  3=Administrativo  4=Otro\n";  // muestra las opciones
    switch (util::leerEntero("  Opcion: ", 1, 4)) {  // pide un numero del 1 al 4
        case 1: return "Docente";  // opcion 1
        case 2: return "Companero";  // opcion 2
        case 3: return "Administrativo";  // opcion 3
        default: return "Otro";  // opcion 4
    }
}

string pedirTelefono(const string& mensaje) {  // pide un telefono hasta que este bien escrito
    while (true) {  // repite hasta que el telefono sea correcto
        string t = util::leerLinea(mensaje);  // pide el telefono
        if (t == "-" || Contacto::telefonoValido(t)) return t;  // si es "-" o esta bien, lo devuelve
        cout << "  -> Telefono invalido (use de 6 a 15 digitos; se permiten + - y espacios).\n";  // avisa que esta mal
    }
}

string pedirCorreo(const string& mensaje) {  // pide un correo hasta que este bien escrito
    while (true) {  // repite hasta que el correo sea correcto
        string c = util::leerLinea(mensaje);  // pide el correo
        if (c == "-" || Contacto::correoValido(c)) return c;  // si es "-" o esta bien, lo devuelve
        cout << "  -> Correo invalido (ejemplo: nombre@universidad.edu).\n";  // avisa que esta mal
    }
}

void agregarContacto(Libreta& lib) {  // pide los datos de un contacto y lo agrega
    cout << "\n--- Nuevo contacto ---\n";  // titulo
    string nombre = util::leerLinea("  Nombre completo: ");  // pide el nombre
    if (nombre.empty()) {  // si no escribio nada
        cout << "  El nombre no puede estar vacio.\n";  // avisa
        return;  // cancela
    }
    if (lib.existeNombre(nombre)) {  // si el nombre ya existe
        cout << "  Ya existe un contacto con ese nombre.\n";  // avisa
        return;  // cancela
    }
    string rol = pedirRol();  // pide el rol
    string tel = pedirTelefono("  Telefono (opcional): ");  // pide el telefono
    string correo = pedirCorreo("  Correo (opcional): ");  // pide el correo
    string notas = util::leerLinea("  Notas (opcional): ");  // pide las notas
    lib.agregar(Contacto(lib.generarId(), nombre, rol, tel == "-" ? "" : tel,  // crea el contacto (si el telefono es "-", queda vacio)
                         correo == "-" ? "" : correo, notas));  // lo mismo con el correo, y agrega las notas
    cout << "  Contacto registrado.\n";  // avisa que se guardo
}

void buscarContacto(const Libreta& lib) {  // pide un texto y busca contactos
    string texto = util::leerLinea("  Buscar (nombre, rol, telefono o correo): ");  // pide lo que se quiere buscar
    if (texto.empty()) {  // si no escribio nada
        cout << "  Escriba algo para buscar.\n";  // avisa
        return;  // cancela
    }
    lib.buscar(texto);  // busca y muestra los resultados
}

void verContactosPorRol(const Libreta& lib) {  // muestra los contactos de un rol
    lib.listarPorRol(pedirRol());  // pide el rol y muestra los contactos
}

void editarContacto(Libreta& lib) {  // cambia los datos de un contacto
    int id = util::leerEntero("  ID del contacto a editar: ", 1, 100000);  // pide el id del contacto
    Contacto* c = lib.buscarPorId(id);  // busca el contacto
    if (!c) {  // si no existe
        cout << "  No existe un contacto con ese ID.\n";  // avisa
        return;  // termina
    }
    cout << "\n  Datos actuales:\n";  // titulo
    c->mostrar();  // muestra los datos actuales
    cout << "\n  (Enter = dejar igual, '-' = borrar el dato)\n";  // explica como usarlo

    string nombre = util::leerLinea("  Nuevo nombre: ");  // pide el nuevo nombre
    if (!nombre.empty()) {  // si escribio algo
        if (lib.existeNombre(nombre, id)) cout << "  -> Ese nombre ya existe, se mantiene el actual.\n";  // si el nombre ya lo tiene otro contacto, no lo cambia
        else c->setNombre(nombre);  // si no, cambia el nombre
    }
    string tel = pedirTelefono("  Nuevo telefono: ");  // pide el nuevo telefono
    if (!tel.empty()) c->setTelefono(tel == "-" ? "" : tel);  // si escribio algo, lo cambia ("-" lo borra)
    string correo = pedirCorreo("  Nuevo correo: ");  // pide el nuevo correo
    if (!correo.empty()) c->setCorreo(correo == "-" ? "" : correo);  // si escribio algo, lo cambia ("-" lo borra)

    cout << "  Nuevo rol: 0=sin cambio  1=Docente  2=Companero  3=Administrativo  4=Otro\n";  // muestra las opciones de rol
    switch (util::leerEntero("  Opcion: ", 0, 4)) {  // pide un numero del 0 al 4
        case 1: c->setRol("Docente"); break;  // cambia el rol a Docente
        case 2: c->setRol("Companero"); break;  // cambia el rol a Companero
        case 3: c->setRol("Administrativo"); break;  // cambia el rol a Administrativo
        case 4: c->setRol("Otro"); break;  // cambia el rol a Otro
        default: break;  // 0 no cambia nada
    }
    string notas = util::leerLinea("  Nuevas notas: ");  // pide las nuevas notas
    if (!notas.empty()) c->setNotas(notas == "-" ? "" : notas);  // si escribio algo, lo cambia ("-" lo borra)

    lib.reordenar();  // ordena otra vez por si cambio el nombre
    cout << "  Contacto actualizado.\n";  // avisa que se guardo el cambio
}

void eliminarContacto(Libreta& lib) {  // borra un contacto despues de preguntar
    int id = util::leerEntero("  ID del contacto a eliminar: ", 1, 100000);  // pide el id del contacto
    Contacto* c = lib.buscarPorId(id);  // busca el contacto
    if (!c) {  // si no existe
        cout << "  No existe un contacto con ese ID.\n";  // avisa
        return;  // termina
    }
    c->mostrar();  // muestra el contacto que se va a borrar
    string r = util::leerLinea("  Seguro que desea eliminarlo? (s/n): ");  // pregunta si esta seguro
    if (r == "s" || r == "S") {  // si responde que si
        lib.eliminar(id);  // borra el contacto
        cout << "  Contacto eliminado.\n";  // avisa que se borro
    } else {  // si responde otra cosa
        cout << "  Operacion cancelada.\n";  // avisa que se cancelo
    }
}

// Menus
void menuEventos(Agenda& agenda) {  // menu de eventos
    int opcion;  // opcion elegida
    do {  // repite el menu hasta elegir 0
        cout << "\n------------------------- EVENTOS -------------------------\n"  // titulo del menu
             << " 1. Agregar clase          5. Ver eventos de una fecha\n"  // opciones 1 y 5
             << " 2. Agregar examen         6. Ver solo las tareas\n"  // opciones 2 y 6
             << " 3. Agregar tarea          7. Marcar tarea como completada\n"  // opciones 3 y 7
             << " 4. Ver todos los eventos  8. Eliminar evento\n"  // opciones 4 y 8
             << " 0. Volver\n";  // opcion 0
        opcion = util::leerEntero("Opcion: ", 0, 8);  // pide la opcion
        switch (opcion) {  // hace lo que corresponde a la opcion
            case 1: agregarClase(agenda); break;  // agrega una clase
            case 2: agregarExamen(agenda); break;  // agrega un examen
            case 3: agregarTarea(agenda); break;  // agrega una tarea
            case 4: agenda.listarTodos(); break;  // muestra todos los eventos
            case 5: agenda.listarPorFecha(pedirFecha()); break;  // pide una fecha y muestra sus eventos
            case 6: agenda.listarPorTipo("TAREA"); break;  // muestra solo las tareas
            case 7: {  // marcar tarea como completada
                int id = util::leerEntero("  ID de la tarea: ", 1, 100000);  // pide el id de la tarea
                cout << (agenda.completarTarea(id) ? "  Tarea completada.\n"  // si se pudo, lo avisa
                                                   : "  No existe una tarea con ese ID.\n");  // si no, avisa que no existe
                break;  // termina esta opcion
            }
            case 8: {  // eliminar evento
                int id = util::leerEntero("  ID del evento a eliminar: ", 1, 100000);  // pide el id del evento
                cout << (agenda.eliminar(id) ? "  Evento eliminado.\n"  // si se pudo, lo avisa
                                             : "  No existe un evento con ese ID.\n");  // si no, avisa que no existe
                break;  // termina esta opcion
            }
            default: break;  // 0 no hace nada y sale del menu
        }
    } while (opcion != 0);  // vuelve al menu mientras no sea 0
}

void menuContactos(Libreta& libreta) {  // menu de contactos
    int opcion;  // opcion elegida
    do {  // repite el menu hasta elegir 0
        cout << "\n------------------------ CONTACTOS ------------------------\n"  // titulo del menu
             << " 1. Agregar contacto       4. Ver contactos por rol\n"  // opciones 1 y 4
             << " 2. Ver todos los contactos 5. Editar contacto\n"  // opciones 2 y 5
             << " 3. Buscar contacto        6. Eliminar contacto\n"  // opciones 3 y 6
             << " 0. Volver\n";  // opcion 0
        opcion = util::leerEntero("Opcion: ", 0, 6);  // pide la opcion
        switch (opcion) {  // hace lo que corresponde a la opcion
            case 1: agregarContacto(libreta); break;  // agrega un contacto
            case 2: libreta.listarTodos(); break;  // muestra todos los contactos
            case 3: buscarContacto(libreta); break;  // busca contactos
            case 4: verContactosPorRol(libreta); break;  // muestra contactos de un rol
            case 5: editarContacto(libreta); break;  // edita un contacto
            case 6: eliminarContacto(libreta); break;  // elimina un contacto
            default: break;  // 0 no hace nada y sale del menu
        }
    } while (opcion != 0);  // vuelve al menu mientras no sea 0
}

bool guardarTodo(const Agenda& agenda, const Libreta& libreta, const string& archEventos,  // guarda eventos y contactos en sus archivos
                 const string& archContactos) {
    bool ok1 = agenda.guardar(archEventos);  // guarda los eventos
    bool ok2 = libreta.guardar(archContactos);  // guarda los contactos
    cout << (ok1 && ok2 ? "  Datos guardados en '" + archEventos + "' y '" + archContactos + "'.\n"  // si todo salio bien, lo avisa
                        : "  Error al guardar los archivos.\n");  // si no, avisa del error
    return ok1 && ok2;  // devuelve verdadero solo si se guardo todo
}

// Programa principal
int main() {  // aqui empieza el programa
    const string ARCH_EVENTOS = "agenda.txt";  // nombre del archivo de eventos
    const string ARCH_CONTACTOS = "contactos.txt";  // nombre del archivo de contactos
    Agenda agenda;  // la agenda con los eventos
    Libreta libreta;  // la libreta con los contactos

    int ne = agenda.cargar(ARCH_EVENTOS);  // lee los eventos guardados
    int nc = libreta.cargar(ARCH_CONTACTOS);  // lee los contactos guardados
    if (ne > 0 || nc > 0)  // si se leyo algo
        cout << "Se cargaron " << ne << " eventos y " << nc << " contactos.\n";  // avisa cuantos se leyeron

    int opcion;  // opcion elegida en el menu principal
    do {  // repite el menu hasta elegir 0
        cout << "\n=============== AGENDA UNIVERSITARIA ===============\n"  // titulo del menu
             << " 1. Eventos (clases, examenes, tareas)\n"  // opcion 1
             << " 2. Contactos\n"  // opcion 2
             << " 3. Guardar\n"  // opcion 3
             << " 0. Guardar y salir\n"  // opcion 0
             << "====================================================\n";  // linea final del menu
        opcion = util::leerEntero("Opcion: ", 0, 3);  // pide la opcion
        switch (opcion) {  // hace lo que corresponde a la opcion
            case 1: menuEventos(agenda); break;  // abre el menu de eventos
            case 2: menuContactos(libreta); break;  // abre el menu de contactos
            case 3:  // la opcion 3 hace lo mismo que la 0
            case 0: guardarTodo(agenda, libreta, ARCH_EVENTOS, ARCH_CONTACTOS); break;  // guarda todo
            default: break;  // no hace nada
        }
    } while (opcion != 0);  // sigue mientras no se elija salir

    cout << "Hasta pronto!\n";  // mensaje de despedida
    return 0;  // el programa termino bien
}