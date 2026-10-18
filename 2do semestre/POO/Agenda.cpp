//  AGENDA UNIVERSITARIA 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>  // para atoi (convierte texto a numero)
#include <cctype>  // para tolower (pasa una letra a minuscula)
using namespace std;

// ---------- Leer datos del teclado ----------

string leerTexto(string mensaje) {
    string texto;
    cout << mensaje;
    getline(cin, texto);
    return texto;
}

int leerNumero(string mensaje) {
    return atoi(leerTexto(mensaje).c_str());  // si no escriben un numero, da 0
}

// Pasa un texto a minusculas (para buscar sin importar mayusculas)
string aMinusculas(string texto) {
    for (int i = 0; i < (int)texto.size(); i++) texto[i] = tolower(texto[i]);
    return texto;
}

// ---------- Clase Evento (clase padre) ----------
// Guarda lo que tienen en comun las clases, los examenes y las tareas

class Evento {
protected:  // las clases hijas pueden usar estos datos
    string titulo, curso, fecha, hora;

public:
    Evento(string t, string c, string f, string h) {
        titulo = t;
        curso = c;
        fecha = f;
        hora = h;
    }

    virtual ~Evento() {}

    string getFecha() { return fecha; }

    // Cada hijo cambia estas funciones a su manera
    virtual bool completar() { return false; }  // solo las tareas se pueden completar

    virtual void mostrar() {
        cout << titulo << " | " << fecha << " " << hora << " | Curso: " << curso << endl;
    }

    virtual void guardar(ofstream& archivo) {
        archivo << titulo << endl << curso << endl << fecha << endl << hora << endl;
    }
};

// ---------- Clase Clase (hija de Evento) ----------

class Clase : public Evento {
private:
    string aula, docente;

public:
    Clase(string t, string c, string f, string h, string a, string d) : Evento(t, c, f, h) {
        aula = a;
        docente = d;
    }

    void mostrar() {
        cout << "[CLASE]  ";
        Evento::mostrar();  // usa lo que ya hace el padre
        cout << "         Aula: " << aula << " | Docente: " << docente << endl;
    }

    void guardar(ofstream& archivo) {
        archivo << "CLASE" << endl;
        Evento::guardar(archivo);
        archivo << aula << endl << docente << endl;
    }
};

// ---------- Clase Examen (hija de Evento) ----------

class Examen : public Evento {
private:
    string temas;
    int peso;  // porcentaje de la nota final

public:
    Examen(string t, string c, string f, string h, string tm, int p) : Evento(t, c, f, h) {
        temas = tm;
        peso = p;
    }

    void mostrar() {
        cout << "[EXAMEN] ";
        Evento::mostrar();
        cout << "         Temas: " << temas << " | Peso: " << peso << "%" << endl;
    }

    void guardar(ofstream& archivo) {
        archivo << "EXAMEN" << endl;
        Evento::guardar(archivo);
        archivo << temas << endl << peso << endl;
    }
};

// ---------- Clase Tarea (hija de Evento) ----------

class Tarea : public Evento {
private:
    int prioridad;  // 1 = alta, 2 = media, 3 = baja
    bool completada;

public:
    Tarea(string t, string c, string f, string h, int p, bool comp) : Evento(t, c, f, h) {
        prioridad = p;
        completada = comp;
    }

    bool completar() {
        completada = true;
        return true;
    }

    void mostrar() {
        string textoPrioridad = "Baja";
        if (prioridad == 1) textoPrioridad = "Alta";
        else if (prioridad == 2) textoPrioridad = "Media";

        string estado = "Pendiente";
        if (completada) estado = "Completada";

        cout << "[TAREA]  ";
        Evento::mostrar();
        cout << "         Prioridad: " << textoPrioridad << " | Estado: " << estado << endl;
    }

    void guardar(ofstream& archivo) {
        archivo << "TAREA" << endl;
        Evento::guardar(archivo);
        archivo << prioridad << endl << (completada ? "1" : "0") << endl;
    }
};

// ---------- Clase Agenda (guarda todos los eventos) ----------

class Agenda {
private:
    vector<Evento*> eventos;  // lista de eventos (clases, examenes y tareas)

public:
    ~Agenda() {
        for (int i = 0; i < cantidad(); i++) delete eventos[i];  // libera la memoria
    }

    int cantidad() { return eventos.size(); }

    void agregar(Evento* e) { eventos.push_back(e); }

    void mostrarTodos() {
        if (cantidad() == 0) {
            cout << "La agenda esta vacia." << endl;
            return;
        }
        for (int i = 0; i < cantidad(); i++) {
            cout << (i + 1) << ". ";
            eventos[i]->mostrar();
        }
    }

    void mostrarPorFecha(string fecha) {
        bool hayEventos = false;
        for (int i = 0; i < cantidad(); i++) {
            if (eventos[i]->getFecha() == fecha) {
                cout << (i + 1) << ". ";
                eventos[i]->mostrar();
                hayEventos = true;
            }
        }
        if (!hayEventos) cout << "No hay eventos en esa fecha." << endl;
    }

    bool completarTarea(int numero) {
        if (numero < 1 || numero > cantidad()) return false;
        return eventos[numero - 1]->completar();  // da false si no es una tarea
    }

    bool eliminar(int numero) {
        if (numero < 1 || numero > cantidad()) return false;
        delete eventos[numero - 1];
        eventos.erase(eventos.begin() + numero - 1);
        return true;
    }

    void guardar(string nombreArchivo) {
        ofstream archivo(nombreArchivo.c_str());
        for (int i = 0; i < cantidad(); i++) eventos[i]->guardar(archivo);
    }

    void cargar(string nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str());
        string tipo;
        while (getline(archivo, tipo)) {  // lee el tipo de cada evento guardado
            string titulo, curso, fecha, hora;
            getline(archivo, titulo);
            getline(archivo, curso);
            getline(archivo, fecha);
            getline(archivo, hora);

            if (tipo == "CLASE") {
                string aula, docente;
                getline(archivo, aula);
                getline(archivo, docente);
                agregar(new Clase(titulo, curso, fecha, hora, aula, docente));
            } else if (tipo == "EXAMEN") {
                string temas, peso;
                getline(archivo, temas);
                getline(archivo, peso);
                agregar(new Examen(titulo, curso, fecha, hora, temas, atoi(peso.c_str())));
            } else if (tipo == "TAREA") {
                string prioridad, completada;
                getline(archivo, prioridad);
                getline(archivo, completada);
                agregar(new Tarea(titulo, curso, fecha, hora, atoi(prioridad.c_str()), completada == "1"));
            }
        }
    }
};

// ---------- Clase Contacto ----------

class Contacto {
private:  // los datos estan protegidos: solo se cambian con las funciones de abajo
    string nombre, rol, telefono, correo;

public:
    Contacto(string n, string r, string t, string c) {
        nombre = n;
        rol = r;
        telefono = t;
        correo = c;
    }

    string getNombre() { return nombre; }
    string getRol() { return rol; }
    string getTelefono() { return telefono; }
    string getCorreo() { return correo; }

    void setNombre(string n) { nombre = n; }
    void setRol(string r) { rol = r; }
    void setTelefono(string t) { telefono = t; }
    void setCorreo(string c) { correo = c; }

    void mostrar() {
        cout << nombre << " (" << rol << ")" << endl;
        cout << "      Tel: " << telefono << " | Correo: " << correo << endl;
    }

    // Dice si el texto aparece en el nombre, el rol o el correo
    bool coincide(string texto) {
        texto = aMinusculas(texto);
        return aMinusculas(nombre).find(texto) != string::npos ||
               aMinusculas(rol).find(texto) != string::npos ||
               aMinusculas(correo).find(texto) != string::npos;
    }

    void guardar(ofstream& archivo) {
        archivo << nombre << endl << rol << endl << telefono << endl << correo << endl;
    }
};

// ---------- Clase Libreta (guarda todos los contactos) ----------

class Libreta {
private:
    vector<Contacto> contactos;

public:
    int cantidad() { return contactos.size(); }

    void agregar(Contacto c) { contactos.push_back(c); }

    void mostrarTodos() {
        if (cantidad() == 0) {
            cout << "No hay contactos." << endl;
            return;
        }
        for (int i = 0; i < cantidad(); i++) {
            cout << (i + 1) << ". ";
            contactos[i].mostrar();
        }
    }

    void buscar(string texto) {
        bool encontrado = false;
        for (int i = 0; i < cantidad(); i++) {
            if (contactos[i].coincide(texto)) {
                cout << (i + 1) << ". ";
                contactos[i].mostrar();
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No se encontro ningun contacto." << endl;
    }

    // Devuelve el contacto para poder editarlo (o NULL si no existe)
    Contacto* obtener(int numero) {
        if (numero < 1 || numero > cantidad()) return NULL;
        return &contactos[numero - 1];
    }

    bool eliminar(int numero) {
        if (numero < 1 || numero > cantidad()) return false;
        contactos.erase(contactos.begin() + numero - 1);
        return true;
    }

    void guardar(string nombreArchivo) {
        ofstream archivo(nombreArchivo.c_str());
        for (int i = 0; i < cantidad(); i++) contactos[i].guardar(archivo);
    }

    void cargar(string nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str());
        string nombre, rol, telefono, correo;
        while (getline(archivo, nombre)) {
            getline(archivo, rol);
            getline(archivo, telefono);
            getline(archivo, correo);
            agregar(Contacto(nombre, rol, telefono, correo));
        }
    }
};

// ---------- Menu de eventos ----------
// Se recibe la agenda con & para trabajar con la agenda original

void menuEventos(Agenda& agenda) {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n--- EVENTOS ---" << endl;
        cout << "1. Agregar clase" << endl;
        cout << "2. Agregar examen" << endl;
        cout << "3. Agregar tarea" << endl;
        cout << "4. Ver todos los eventos" << endl;
        cout << "5. Ver eventos de una fecha" << endl;
        cout << "6. Marcar tarea como completada" << endl;
        cout << "7. Eliminar evento" << endl;
        cout << "0. Volver" << endl;
        opcion = leerNumero("Opcion: ");

        if (opcion == 1) {
            string titulo = leerTexto("Titulo: ");
            string curso = leerTexto("Curso: ");
            string fecha = leerTexto("Fecha (dd/mm/aaaa): ");
            string hora = leerTexto("Hora (hh:mm): ");
            string aula = leerTexto("Aula: ");
            string docente = leerTexto("Docente: ");
            agenda.agregar(new Clase(titulo, curso, fecha, hora, aula, docente));
            cout << "Clase agregada." << endl;
        } else if (opcion == 2) {
            string titulo = leerTexto("Titulo: ");
            string curso = leerTexto("Curso: ");
            string fecha = leerTexto("Fecha (dd/mm/aaaa): ");
            string hora = leerTexto("Hora (hh:mm): ");
            string temas = leerTexto("Temas: ");
            int peso = leerNumero("Peso en la nota final (%): ");
            agenda.agregar(new Examen(titulo, curso, fecha, hora, temas, peso));
            cout << "Examen agregado." << endl;
        } else if (opcion == 3) {
            string titulo = leerTexto("Titulo: ");
            string curso = leerTexto("Curso: ");
            string fecha = leerTexto("Fecha (dd/mm/aaaa): ");
            string hora = leerTexto("Hora (hh:mm): ");
            int prioridad = leerNumero("Prioridad (1=Alta, 2=Media, 3=Baja): ");
            agenda.agregar(new Tarea(titulo, curso, fecha, hora, prioridad, false));
            cout << "Tarea agregada." << endl;
        } else if (opcion == 4) {
            agenda.mostrarTodos();
        } else if (opcion == 5) {
            agenda.mostrarPorFecha(leerTexto("Fecha (dd/mm/aaaa): "));
        } else if (opcion == 6) {
            int numero = leerNumero("Numero de la tarea: ");
            if (agenda.completarTarea(numero)) cout << "Tarea completada." << endl;
            else cout << "Ese numero no es una tarea." << endl;
        } else if (opcion == 7) {
            int numero = leerNumero("Numero del evento a eliminar: ");
            if (agenda.eliminar(numero)) cout << "Evento eliminado." << endl;
            else cout << "No existe ese numero." << endl;
        } else if (opcion != 0) {
            cout << "Opcion no valida." << endl;
        }
    }
}

// ---------- Menu de contactos ----------

void menuContactos(Libreta& libreta) {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n--- CONTACTOS ---" << endl;
        cout << "1. Agregar contacto" << endl;
        cout << "2. Ver todos los contactos" << endl;
        cout << "3. Buscar contacto" << endl;
        cout << "4. Editar contacto" << endl;
        cout << "5. Eliminar contacto" << endl;
        cout << "0. Volver" << endl;
        opcion = leerNumero("Opcion: ");

        if (opcion == 1) {
            string nombre = leerTexto("Nombre: ");
            string rol = leerTexto("Rol (docente, companero, etc.): ");
            string telefono = leerTexto("Telefono: ");
            string correo = leerTexto("Correo: ");
            libreta.agregar(Contacto(nombre, rol, telefono, correo));
            cout << "Contacto agregado." << endl;
        } else if (opcion == 2) {
            libreta.mostrarTodos();
        } else if (opcion == 3) {
            libreta.buscar(leerTexto("Buscar: "));
        } else if (opcion == 4) {
            int numero = leerNumero("Numero del contacto a editar: ");
            Contacto* c = libreta.obtener(numero);
            if (c == NULL) {
                cout << "No existe ese contacto." << endl;
            } else {
                cout << "(Si deja un dato vacio, no se cambia)" << endl;
                string nombre = leerTexto("Nuevo nombre: ");
                string rol = leerTexto("Nuevo rol: ");
                string telefono = leerTexto("Nuevo telefono: ");
                string correo = leerTexto("Nuevo correo: ");
                if (nombre != "") c->setNombre(nombre);
                if (rol != "") c->setRol(rol);
                if (telefono != "") c->setTelefono(telefono);
                if (correo != "") c->setCorreo(correo);
                cout << "Contacto actualizado." << endl;
            }
        } else if (opcion == 5) {
            int numero = leerNumero("Numero del contacto a eliminar: ");
            if (libreta.eliminar(numero)) cout << "Contacto eliminado." << endl;
            else cout << "No existe ese contacto." << endl;
        } else if (opcion != 0) {
            cout << "Opcion no valida." << endl;
        }
    }
}

// ---------- Programa principal ----------

int main() {
    Agenda agenda;
    Libreta libreta;

    // Al empezar, lee lo que se guardo la ultima vez
    agenda.cargar("datos_eventos.txt");
    libreta.cargar("datos_contactos.txt");

    int opcion = -1;
    while (opcion != 0) {
        cout << "\n=== AGENDA UNIVERSITARIA ===" << endl;
        cout << "1. Eventos" << endl;
        cout << "2. Contactos" << endl;
        cout << "3. Guardar" << endl;
        cout << "0. Guardar y salir" << endl;
        opcion = leerNumero("Opcion: ");

        if (opcion == 1) {
            menuEventos(agenda);
        } else if (opcion == 2) {
            menuContactos(libreta);
        } else if (opcion == 3 || opcion == 0) {
            agenda.guardar("datos_eventos.txt");
            libreta.guardar("datos_contactos.txt");
            cout << "Datos guardados." << endl;
        } else {
            cout << "Opcion no valida." << endl;
        }
    }

    cout << "Hasta pronto!" << endl;
    return 0;
}