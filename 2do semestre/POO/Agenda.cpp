//  AGENDA UNIVERSITARIA 

#include <iostream>   // escribir en pantalla y leer del teclado
#include <fstream>    // guardar y leer archivos de texto
#include <sstream>    // armar textos pegando pedacitos
#include <string>     // trabajar con textos
#include <vector>     // listas que crecen
#include <cstdlib>    // atoi (texto a número) y exit (cerrar el programa)
#include <cstdio>     // sscanf (sacar números de un texto)
#include <cctype>     // tolower (a minúscula) e isdigit (¿es un dígito?)
#include <ctime>      // para obtener la fecha de hoy
using namespace std;  // nos ahorra escribir "std::" delante de todo

// ---------- Leer datos del teclado ----------

string leerTexto(string mensaje) {
    string texto;
    cout << mensaje;
    if (!getline(cin, texto)) {                         // Si ya no se puede leer nada (se cerró la entrada)
        cout << endl << "Entrada cerrada. Saliendo." << endl;
        exit(0);                                        // cierra el programa para no quedar dando vueltas sin fin
    }
    return texto;
}

int leerNumero(string mensaje) {
    string texto = leerTexto(mensaje);
    if (texto.empty()) return -1;
    for (int i = 0; i < (int)texto.size(); i++) {
        if (!isdigit((unsigned char)texto[i])) return -1;   // apareció algo que no es dígito
    }
    return atoi(texto.c_str());
}

//  repite la pregunta hasta que el número esté dentro del rango permitido.
int leerNumeroEntre(string mensaje, int minimo, int maximo) {
    int n = leerNumero(mensaje);
    while (n < minimo || n > maximo) {
        cout << "Numero no valido. Debe estar entre " << minimo << " y " << maximo << "." << endl;
        n = leerNumero(mensaje);
    }
    return n;
}

string aMinusculas(string texto) {
    for (int i = 0; i < (int)texto.size(); i++) texto[i] = tolower((unsigned char)texto[i]);
    return texto;
}

// ---------- Clase Fecha (del diagrama) ----------

class Fecha {
private:
    int dia, mes, anio;

public:
    Fecha() { dia = 0; mes = 0; anio = 0; }
    Fecha(int d, int m, int a) { dia = d; mes = m; anio = a; }

    int getDia() { return dia; }
    int getMes() { return mes; }
    int getAnio() { return anio; }

    // Revisa que la fecha tenga sentido
    // revisa los días reales de cada mes y los años bisiestos.
    bool esValida() {
        if (anio < 1 || mes < 1 || mes > 12 || dia < 1) return false;
        int diasDelMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
        int maximo = diasDelMes[mes - 1];
        if (mes == 2 && bisiesto) maximo = 29;
        return dia <= maximo;
    }

    // Convierte a texto dd/mm/aaaa
    string aTexto() {
        ostringstream s;
        if (dia < 10) s << "0";                         // pone un 0 delante si es de una cifra
        s << dia << "/";
        if (mes < 10) s << "0";
        s << mes << "/" << anio;
        return s.str();
    }

    void imprimir() { cout << aTexto(); }

    // Crea una fecha desde un texto "dd/mm/aaaa" (si falla, queda en 0/0/0)
    static Fecha desdeTexto(string texto) {
        int d = 0, m = 0, a = 0;
        sscanf(texto.c_str(), "%d/%d/%d", &d, &m, &a);  // saca los tres números separados por barras
        return Fecha(d, m, a);
    }

    // Fecha de hoy
    static Fecha hoy() {
        time_t t = time(NULL);                          // pregunta al reloj de la computadora
        tm* ahora = localtime(&t);
        return Fecha(ahora->tm_mday, ahora->tm_mon + 1, ahora->tm_year + 1900);  // el mes cuenta desde 0 y el año desde 1900, por eso se suma
    }
};

// ---------- Clase Evento (clase padre) ----------

class Evento {
protected:
    string titulo, curso, fecha, hora;

public:
    Evento(string t, string c, string f, string h) {
        titulo = t;
        curso = c;
        fecha = f;
        hora = h;
    }

    virtual ~Evento() {}                                // permite que, al borrar un evento, se borre completo con lo propio de cada tipo

    string getFecha() { return fecha; }

    virtual bool completar() { return false; }          // solo las tareas se pueden completar

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
        Evento::mostrar();                              // primero los datos comunes
        cout << "         Aula: " << aula << " | Docente: " << docente << endl;
    }

    void guardar(ofstream& archivo) {
        archivo << "CLASE" << endl;                     // el tipo va primero, para saber qué leer al cargar
        Evento::guardar(archivo);
        archivo << aula << endl << docente << endl;
    }
};

// ---------- Clase Examen (hija de Evento) ----------

class Examen : public Evento {
private:
    string temas;
    int peso;

public:
    Examen(string t, string c, string f, string h, string tem, int p) : Evento(t, c, f, h) {
        temas = tem;
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
        return true;                                    // avisa que sí se pudo
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
        archivo << prioridad << endl << (completada ? "1" : "0") << endl;   // 1 = hecha, 0 = pendiente
    }
};

// ---------- Clase Agenda (guarda todos los eventos) ----------

class Agenda {
private:
    vector<Evento*> eventos;                            // clases, exámenes y tareas mezclados en una sola lista

public:
    ~Agenda() {                                         // se ejecuta solo cuando el programa termina
        for (int i = 0; i < cantidad(); i++) delete eventos[i];   // libera la memoria de cada evento
    }

    int cantidad() { return eventos.size(); }

    void agregar(Evento* e) { eventos.push_back(e); }

    void mostrarTodos() {
        if (cantidad() == 0) {
            cout << "La agenda esta vacia." << endl;
            return;
        }
        for (int i = 0; i < cantidad(); i++) {
            cout << (i + 1) << ". ";                    // el número de orden empieza en 1
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
        return eventos[numero - 1]->completar();        // solo funciona si ese evento es una tarea
    }

    bool eliminar(int numero) {
        if (numero < 1 || numero > cantidad()) return false;
        delete eventos[numero - 1];                     // libera la memoria
        eventos.erase(eventos.begin() + numero - 1);    // lo quita de la lista
        return true;
    }

    void guardar(string nombreArchivo) {
        ofstream archivo(nombreArchivo.c_str());
        for (int i = 0; i < cantidad(); i++) eventos[i]->guardar(archivo);   // cada evento se escribe a sí mismo
    }

    void cargar(string nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str());        // si el archivo no existe, simplemente no lee nada
        string tipo;
        while (getline(archivo, tipo)) {
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

// ---------- Clase Contacto (segun el diagrama) ----------

class Contacto {
private:
    vector<string> numeros;  // Numeros -> string[]
    string nombre;
    string correo;
    string direccion;
    bool bloqueado;
    bool eliminado;
    Fecha nacimiento;        // Fecha de nacimiento -> Fecha

public:
    Contacto(string n, vector<string> nums, string c, string d, Fecha f) {
        nombre = n;
        numeros = nums;
        correo = c;
        direccion = d;
        nacimiento = f;
        bloqueado = false;
        eliminado = false;
    }

    // Getters
    string getNombre() { return nombre; }
    bool estaBloqueado() { return bloqueado; }
    bool estaEliminado() { return eliminado; }

    // Setters (para editar)
    void setNombre(string n) { nombre = n; }
    void setNumeros(vector<string> n) { numeros = n; }
    void setCorreo(string c) { correo = c; }
    void setDireccion(string d) { direccion = d; }
    void setNacimiento(Fecha f) { nacimiento = f; }
    void setBloqueado(bool b) { bloqueado = b; }
    void setEliminado(bool e) { eliminado = e; }

    void eliminar() { eliminado = true; }   // borrado logico: no se destruye, solo se esconde
    void bloquear() { bloqueado = true; }
    void desbloquear() { bloqueado = false; }

    int calcularEdad() {
        Fecha hoy = Fecha::hoy();
        int edad = hoy.getAnio() - nacimiento.getAnio();
        if (hoy.getMes() < nacimiento.getMes() ||
            (hoy.getMes() == nacimiento.getMes() && hoy.getDia() < nacimiento.getDia())) {
            edad--;                                     // todavía no cumplió años este año
        }
        return edad;
    }

    bool cumpleHoy() {
        Fecha hoy = Fecha::hoy();
        return nacimiento.getDia() == hoy.getDia() && nacimiento.getMes() == hoy.getMes();
    }

    void mostrar() {
        cout << nombre;
        if (bloqueado) cout << "  [BLOQUEADO]";
        cout << endl;

        cout << "      Numeros: ";
        for (int i = 0; i < (int)numeros.size(); i++) {
            if (i > 0) cout << ", ";
            cout << numeros[i];
        }
        cout << endl;
        cout << "      Correo: " << correo << " | Direccion: " << direccion << endl;
        cout << "      Nacimiento: ";
        nacimiento.imprimir();
        if (nacimiento.esValida()) cout << " (" << calcularEdad() << " anios)";
        cout << endl;
    }

    // Buscar: el texto aparece en nombre, correo, direccion o algun numero
    bool coincide(string texto) {
        texto = aMinusculas(texto);                     // así no importa si escribe mayúsculas o minúsculas
        if (aMinusculas(nombre).find(texto) != string::npos) return true;
        if (aMinusculas(correo).find(texto) != string::npos) return true;
        if (aMinusculas(direccion).find(texto) != string::npos) return true;
        for (int i = 0; i < (int)numeros.size(); i++) {
            if (numeros[i].find(texto) != string::npos) return true;
        }
        return false;
    }

    // Formato del archivo: 7 lineas por contacto (los numeros van separados por ;)
    void guardar(ofstream& archivo) {
        string linea = "";
        for (int i = 0; i < (int)numeros.size(); i++) {
            if (i > 0) linea += ";";
            linea += numeros[i];
        }
        archivo << nombre << endl << linea << endl << correo << endl << direccion << endl
                << nacimiento.aTexto() << endl << (bloqueado ? "1" : "0") << endl
                << (eliminado ? "1" : "0") << endl;
    }
};

// ---------- Clase Libreta (guarda todos los contactos) ----------

class Libreta {
private:
    vector<Contacto> contactos;

public:
    int cantidad() { return contactos.size(); }

    void registrar(Contacto c) { contactos.push_back(c); }

    // Los numeros no cambian aunque haya contactos eliminados
    void listar() {
        bool hay = false;
        for (int i = 0; i < cantidad(); i++) {
            if (contactos[i].estaEliminado()) continue; // los borrados se saltan
            cout << (i + 1) << ". ";
            contactos[i].mostrar();
            hay = true;
        }
        if (!hay) cout << "No hay contactos." << endl;
    }

    void buscar(string texto) {
        bool encontrado = false;
        for (int i = 0; i < cantidad(); i++) {
            if (contactos[i].estaEliminado()) continue;
            if (contactos[i].coincide(texto)) {
                cout << (i + 1) << ". ";
                contactos[i].mostrar();
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No se encontro ningun contacto." << endl;
    }

    void listarCumpleaniosHoy() {
        bool hay = false;
        for (int i = 0; i < cantidad(); i++) {
            if (contactos[i].estaEliminado()) continue;
            if (contactos[i].cumpleHoy()) {
                cout << (i + 1) << ". ";
                contactos[i].mostrar();
                hay = true;
            }
        }
        if (!hay) cout << "Nadie cumple anios hoy." << endl;
    }

    // Devuelve el contacto para editarlo (o NULL si no existe o fue eliminado)
    Contacto* obtener(int numero) {
        if (numero < 1 || numero > cantidad()) return NULL;
        if (contactos[numero - 1].estaEliminado()) return NULL;
        return &contactos[numero - 1];                  // devuelve "la dirección" del contacto, no una copia, para poder modificarlo
    }

    void guardar(string nombreArchivo) {
        ofstream archivo(nombreArchivo.c_str());
        for (int i = 0; i < cantidad(); i++) {
            if (!contactos[i].estaEliminado()) contactos[i].guardar(archivo);   // los borrados no se guardan
        }
    }

    void cargar(string nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str());
        string nombre, numerosTexto, correo, direccion, fecha, bloq, elim;
        while (getline(archivo, nombre)) {
            getline(archivo, numerosTexto);
            getline(archivo, correo);
            getline(archivo, direccion);
            getline(archivo, fecha);
            getline(archivo, bloq);
            getline(archivo, elim);

            // separar los numeros por ;
            vector<string> numeros;
            string actual = "";                         // el teléfono que se está armando
            for (int i = 0; i < (int)numerosTexto.size(); i++) {
                if (numerosTexto[i] == ';') {
                    numeros.push_back(actual);
                    actual = "";
                } else {
                    actual += numerosTexto[i];
                }
            }
            if (actual != "") numeros.push_back(actual);   // guarda el último teléfono

            Contacto c(nombre, numeros, correo, direccion, Fecha::desdeTexto(fecha));
            c.setBloqueado(bloq == "1");
            c.setEliminado(elim == "1");
            registrar(c);
        }
    }
};

// ---------- Funciones auxiliares para pedir datos de contacto ----------

vector<string> leerNumeros() {
    vector<string> numeros;
    while (true) {
        string n = leerTexto("Numero de telefono (vacio para terminar): ");
        if (n == "") break;
        numeros.push_back(n);
    }
    return numeros;
}

Fecha leerFecha(string mensaje) {
    Fecha f = Fecha::desdeTexto(leerTexto(mensaje));
    while (!f.esValida()) {
        cout << "Fecha no valida. Use el formato dd/mm/aaaa." << endl;
        f = Fecha::desdeTexto(leerTexto(mensaje));
    }
    return f;
}

// ---------- Menu de eventos ----------

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
            string fecha = leerFecha("Fecha (dd/mm/aaaa): ").aTexto();   // la fecha se revisa y se guarda siempre con el mismo formato
            string hora = leerTexto("Hora (hh:mm): ");
            string aula = leerTexto("Aula: ");
            string docente = leerTexto("Docente: ");
            agenda.agregar(new Clase(titulo, curso, fecha, hora, aula, docente));
            cout << "Clase agregada." << endl;
        } else if (opcion == 2) {
            string titulo = leerTexto("Titulo: ");
            string curso = leerTexto("Curso: ");
            string fecha = leerFecha("Fecha (dd/mm/aaaa): ").aTexto();   
            string hora = leerTexto("Hora (hh:mm): ");
            string temas = leerTexto("Temas: ");
            int peso = leerNumeroEntre("Peso en la nota final (%): ", 0, 100);   //  solo acepta de 0 a 100
            agenda.agregar(new Examen(titulo, curso, fecha, hora, temas, peso));
            cout << "Examen agregado." << endl;
        } else if (opcion == 3) {
            string titulo = leerTexto("Titulo: ");
            string curso = leerTexto("Curso: ");
            string fecha = leerFecha("Fecha (dd/mm/aaaa): ").aTexto();   
            string hora = leerTexto("Hora (hh:mm): ");
            int prioridad = leerNumeroEntre("Prioridad (1=Alta, 2=Media, 3=Baja): ", 1, 3);   //  solo acepta 1, 2 o 3
            agenda.agregar(new Tarea(titulo, curso, fecha, hora, prioridad, false));   // false = empieza pendiente
            cout << "Tarea agregada." << endl;
        } else if (opcion == 4) {
            agenda.mostrarTodos();
        } else if (opcion == 5) {
            agenda.mostrarPorFecha(leerFecha("Fecha (dd/mm/aaaa): ").aTexto());   //  así "5/3/2026" encuentra "05/03/2026"
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

// ---------- Menu de contactos (el mismo orden del diagrama) ----------

void menuContactos(Libreta& libreta) {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n--- CONTACTOS ---" << endl;
        cout << "1) Registrar nuevo contacto" << endl;
        cout << "2) Editar contacto" << endl;
        cout << "3) Listar contactos" << endl;
        cout << "4) Eliminar contacto" << endl;
        cout << "5) Bloquear / desbloquear contacto" << endl;
        cout << "6) Buscar contacto" << endl;
        cout << "7) Cumpleanios de hoy" << endl;
        cout << "0) Volver" << endl;
        opcion = leerNumero("Opcion: ");

        if (opcion == 1) {
            string nombre = leerTexto("Nombre: ");
            vector<string> numeros = leerNumeros();
            string correo = leerTexto("Correo electronico: ");
            string direccion = leerTexto("Direccion: ");
            Fecha nac = leerFecha("Fecha de nacimiento (dd/mm/aaaa): ");
            libreta.registrar(Contacto(nombre, numeros, correo, direccion, nac));
            cout << "Contacto registrado." << endl;
        } else if (opcion == 2) {
            int numero = leerNumero("Numero del contacto a editar: ");
            Contacto* c = libreta.obtener(numero);
            if (c == NULL) {
                cout << "No existe ese contacto." << endl;
            } else {
                cout << "(Si deja un dato vacio, no se cambia)" << endl;
                string nombre = leerTexto("Nuevo nombre: ");
                string correo = leerTexto("Nuevo correo: ");
                string direccion = leerTexto("Nueva direccion: ");
                string fecha = leerTexto("Nueva fecha de nacimiento (dd/mm/aaaa): ");
                string cambiar = leerTexto("Cambiar numeros? (s/n): ");

                if (nombre != "") c->setNombre(nombre);
                if (correo != "") c->setCorreo(correo);
                if (direccion != "") c->setDireccion(direccion);
                if (fecha != "") {
                    Fecha f = Fecha::desdeTexto(fecha);
                    if (f.esValida()) c->setNacimiento(f);
                    else cout << "Fecha no valida, no se cambio." << endl;
                }
                if (aMinusculas(cambiar) == "s") c->setNumeros(leerNumeros());
                cout << "Contacto actualizado." << endl;
            }
        } else if (opcion == 3) {
            libreta.listar();
        } else if (opcion == 4) {
            int numero = leerNumero("Numero del contacto a eliminar: ");
            Contacto* c = libreta.obtener(numero);
            if (c == NULL) {
                cout << "No existe ese contacto." << endl;
            } else {
                c->eliminar();
                cout << "Contacto eliminado." << endl;
            }
        } else if (opcion == 5) {
            int numero = leerNumero("Numero del contacto: ");
            Contacto* c = libreta.obtener(numero);
            if (c == NULL) {
                cout << "No existe ese contacto." << endl;
            } else if (c->estaBloqueado()) {            // si ya estaba bloqueado, lo desbloquea; si no, lo bloquea
                c->desbloquear();
                cout << "Contacto desbloqueado." << endl;
            } else {
                c->bloquear();
                cout << "Contacto bloqueado." << endl;
            }
        } else if (opcion == 6) {
            libreta.buscar(leerTexto("Buscar: "));
        } else if (opcion == 7) {
            libreta.listarCumpleaniosHoy();
        } else if (opcion != 0) {
            cout << "Opcion no valida." << endl;
        }
    }
}

// ---------- Programa principal ----------

int main() {
    Agenda agenda;
    Libreta libreta;

    agenda.cargar("datos_eventos.txt");                 // trae lo guardado la vez anterior
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
        } else if (opcion == 3 || opcion == 0) {        // la opción 0 también guarda antes de salir
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