#include <array>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <locale>
#include <regex>
#include <windows.h>
#include <string>

using namespace std;

// Gestor autos
const size_t columnasRegistro{7};                                    // Evita hardcodear el tamaño del array
void eliminarAuto(const string &);                                   // Solicita codigo y llama a eliminar registro auto del inventario
bool eliminarRegistroAuto(const string &, const string &);           // Eliminar registro auto del inventario
array<string, columnasRegistro> getAuto(const string, const string); // Obtener registro auto del inventario
void imprimirDatosAuto(const array<string, columnasRegistro> &);
void modificarRegistroAuto(const string &, const string &);
void reporteAutosInventario(const string &);
bool setAuto(const array<string, columnasRegistro> &, const string); // Escribir registro auto en el inventario
string solicitarCodigoAuto();                                        // Solicita el código del auto al usuario y valida
// Gestor competencias
const size_t columnasRegistroCompetencia{7};                                // Evita hardcodear el tamaño del array
array<string, columnasRegistro> getCompetencia(const string, const string); // Obtener registro competencia del inventario
void imprimirDatosCompetencia(const array<string, columnasRegistro> &);     // Imprimie en consola los datos de la competencia
string menuGanadorCompetencia(const string, const string);                  // Este menu pregunta por el auto ganador
void reporteCompetencias(const string &);
bool setCompetencia(const array<string, columnasRegistroCompetencia> &, const string); // Escribe en archivo el registro de la competencia

// Validador entradas
bool validarCodigo(const string &, const char); // Valida el formato del código
bool validarFormatoFecha(const string &);
bool validarNombre(string);
bool validarNumeroRango(const string &, const int, const int); // Valida el rango de un número
bool validarSoloNumero(const string &);

// Gestor General
bool confirmarAccion(const string &); // Se usa para preguntas de sí o no, evita duplicar código
void consultarAutoInventario(const string);
void consultarCompetenciaInventario(const string);
string generarCodigoUnico(const string, const string);                 // Genera un código único para el auto o competencia
bool ingresarAutoAlInventario(const string);                           // Llama a la función que ingresa el auto al inventario
bool ingresarCompetenciaAlInventario(const string, const string);      // Llama a la función que ingresa la competencia al inventario
void menuInventarioAutos(const string);                                // Abre el menú para gestionar inventario de autos
void menuInventarioCompetencias(const string, const string);           // Abre el menú para gestionar inventario de competencias
bool solicitarNumero(const string &, const short, string &, int, int); // Solicita un número al usuario y valida
bool solicitarTexto(const string, string &, int);                      // Solicita un texto al usuario y valida
int mostrarMenu(const string, const int);                              // Muestra un menú y devuelve la opción seleccionada por el usuario

void formatearParaSalida(string &);                                        // Formatea texto según un estandar común para escribir en archivo
string convertirRegistroEnString(const array<string, columnasRegistro> &); // Convierte un arreglo a un sólo string

//* MAIN
int main()
{
    setlocale(LC_CTYPE, "Spanish");
    system("CLS");
    const string ArchivoAutos = "Autos.txt";
    const string ArchivoCompetencias = "Competencias.txt";

    string menu = "\n\n************************************\n"
                  "Sistema de gestión de autos.\n"
                  "1. Inventario de autos.\n"
                  "2. Inventario de competencias.\n"
                  "3. Reporte de autos.\n"
                  "4. Reporte de competencias.\n"
                  "5. Salir.\n"
                  "Seleccione una opcion: ";
    bool deseaSalir = false;
    do
    {
        int seleccionMenu = mostrarMenu(menu, 5);
        switch (seleccionMenu)
        {
        case 1:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            menuInventarioAutos(ArchivoAutos);
            cout << "Presione Enter para continuar...";
            cin.get();
            break;
        case 2:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            menuInventarioCompetencias(ArchivoCompetencias, ArchivoAutos);
            cout << "Presione Enter para continuar...";
            cin.get();
            break;
        case 3:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            reporteAutosInventario(ArchivoAutos);
            cout << "Presione Enter para continuar...";
            cin.get();
            break;
        case 4:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            reporteCompetencias(ArchivoCompetencias);
            cout << "Presione Enter para continuar...";
            cin.get();
            break;
        case 5:
            if (confirmarAccion("Desea salir del programa?"))
            {
                deseaSalir = true;
            }
            else
            {
                deseaSalir = false;
            }
            break;
        default:
            cout << "Opción no válida. Intente de nuevo." << endl;
            break;
        }
    } while (!deseaSalir);

    reporteAutosInventario("Autos.txt");

    return 0;
}

// Gestor autos

void eliminarAuto(const string &direccionArchivo)
{
    eliminarRegistroAuto(solicitarCodigoAuto(), direccionArchivo);
}

bool eliminarRegistroAuto(const string &codigo, const string &direccionArchivo)
{
    /*Esta función simula la eliminación de un registro de auto en el inventario,
    mediante la creación de un archivo temporal. Si el registro se encuentra,
    no se copia al archivo temporal. Luego, se elimina el archivo original y
    se renombra el archivo temporal al nombre del archivo original.
    */
    try
    {
        ifstream inArchivoAutos(direccionArchivo, ios::in);
        if (!inArchivoAutos)
        {
            cout << "Error: No se pudo abrir el archivo " << direccionArchivo << endl;
            throw runtime_error("Error al abrir el archivo: " + direccionArchivo);
        }

        cout << "El archivo se abrió correctamente" << endl;

        ofstream outArchivoTemp("temp.txt", ios::out);
        array<string, columnasRegistro> registroAuto;

        bool autoEncontrado = false;
        // Este while escribe en el archivo temporal todos los registros que no son el que se busca
        while (inArchivoAutos >> registroAuto[0] >> quoted(registroAuto[1]) >> registroAuto[2] >> registroAuto[3] >> quoted(registroAuto[4]) >> registroAuto[5] >> registroAuto[6])
        {
            if (registroAuto[0].empty() || !(registroAuto[0] == codigo)) // Si el registro no es el que se busca, lo escribe en el archivo temporal
            {

                outArchivoTemp << registroAuto[0] << " " << quoted(registroAuto[1]) << " "
                               << registroAuto[2] << " " << registroAuto[3] << " "
                               << quoted(registroAuto[4]) << " " << registroAuto[5] << " "
                               << registroAuto[6] << endl;
            }
            else
            {
                autoEncontrado = true;
                cout << "\n============================ \n"
                     << "Datos del auto a eliminar \n"
                     << "============================ \n";

                imprimirDatosAuto(registroAuto);
            }
        }

        outArchivoTemp.close();
        inArchivoAutos.close();

        string seleccionUsuario;

        if (autoEncontrado)
        {
            if (!confirmarAccion("¿Está seguro de que desea eliminar este auto?"))
            {
                cout << "Eliminación cancelada por el usuario" << endl;
                return false;
            }
            else
            {
            }
            if (remove(direccionArchivo.c_str()) != 0)
            {
                cerr << "Error al eliminar el archivo original: " << direccionArchivo << endl;
                remove("temp.txt");
                return false;
            }

            if (rename("temp.txt", direccionArchivo.c_str()) != 0)
            {
                cerr << "Error al renombrar el archivo temporal a: " << direccionArchivo << endl;
                return false;
            }
            cout << "\n============================ \n"
                 << "El auto se ha eliminado correctamente! \n"
                 << "============================ \n";
            return true;
        }
        else
        {
            cout << "No se encontró el registro del auto " << codigo << endl;
            remove("temp.txt");
            return false;
        }
    }
    catch (const exception &e)
    {
        cerr << "No se pudo eliminar el archivo: " << e.what() << endl;
        return false;
    }
}

array<string, columnasRegistro> getAuto(const string codigo, const string direccionArchivo)
{
    // Esta función busca un registro de auto en el inventario y lo devuelve como un array de strings.
    array<string, columnasRegistro> registroAuto;

    fstream inArchivoAutos(direccionArchivo, ios::in);
    if (!inArchivoAutos)
    {
        cerr << "No se pudo abrir el archivo" << endl;
    }
    else
    {
        cout << "El archivo se abrió correctamente" << endl;
    }

    while (inArchivoAutos >> registroAuto[0] >> quoted(registroAuto[1]) >> registroAuto[2] >> registroAuto[3] >> quoted(registroAuto[4]) >> registroAuto[5] >> registroAuto[6])
    {
        if (registroAuto[0] == codigo)
        {
            cout << "Auto encontrado" << endl;
            return registroAuto;
        }
    }
    inArchivoAutos.close();
    cout << "Auto no encontrado!" << endl;
    return array<string, columnasRegistro>{};
}

void imprimirDatosAuto(const array<string, columnasRegistro> &registroAuto)
{
    cout << "\n============================ \n";
    cout << "Datos del auto encontrado \n";
    cout << "============================ \n";
    cout << "Código Auto: " << registroAuto[0] << endl;
    cout << "Nombre del Auto: " << registroAuto[1] << endl;
    cout << "Velocidad Máxima: " << registroAuto[2] << " km/h" << endl;
    cout << "Caballos de fuerza: " << registroAuto[3] << " HP" << endl;
    cout << "Equipo/Propietario: " << registroAuto[4] << endl;
    cout << "Costo del auto: " << registroAuto[5] << " USD" << endl;
    cout << "Número de identificación del registrador: " << registroAuto[6] << endl;
}

void modificarAuto(const string &direccionArchivo)
{
    bool deseaSalir = false;
    do
    {
        modificarRegistroAuto(solicitarCodigoAuto(), direccionArchivo);
        confirmarAccion("¿Desea volver al menú principal?");
    } while (deseaSalir);
}

void modificarRegistroAuto(const string &codigo, const string &direccionArchivo)
{
    /* Al igual que la función de eliminar, esta función simula la modificación
    de un registro de auto en el inventario, mediante la creación de un archivo temporal.
    Si el registro se encuentra, se modifica y se copia al archivo temporal.
    */
    try
    {

        ifstream inArchivoAutos(direccionArchivo, ios::in);
        if (!inArchivoAutos)
        {
            cout << "Error: No se pudo abrir el archivo " << direccionArchivo << endl;
            throw runtime_error("Error al abrir el archivo: " + direccionArchivo);
        }

        cout << "El archivo se abrió correctamente" << endl;

        ofstream outArchivoTemp("temp.txt", ios::out);
        array<string, columnasRegistro> registroAuto;
        array<string, columnasRegistro> registroAutoModificado;

        bool autoEncontrado = false;

        while (inArchivoAutos >> registroAuto[0] >> quoted(registroAuto[1]) >> registroAuto[2] >> registroAuto[3] >> quoted(registroAuto[4]) >> registroAuto[5] >> registroAuto[6])
        {
            if (registroAuto[0].empty() || !(registroAuto[0] == codigo))
            {
                outArchivoTemp << registroAuto[0] << " " << quoted(registroAuto[1]) << " "
                               << registroAuto[2] << " " << registroAuto[3] << " "
                               << quoted(registroAuto[4]) << " " << registroAuto[5] << " "
                               << registroAuto[6] << endl;
            }
            else if (registroAuto[0] == codigo)
            {
                autoEncontrado = true;
                cout << "\n============================ \n"
                     << "Datos del auto a modificar \n"
                     << "============================ \n";

                imprimirDatosAuto(registroAuto);

                string mensaje = "Ingrese la velocidad máxima (km/h) del automóvil ";
                mensaje += "\n (igual a 0 & más de 3 cifras serán omitidas): ";
                solicitarNumero(mensaje, 4, registroAuto[2], 999, 1);

                cin.clear();

                mensaje = "Ingrese los caballos de fuerza(hp) del automóvil";
                mensaje += "\n (más de 3 cifras serán omitidas): ";
                solicitarNumero(mensaje, 4, registroAuto[3], 999, 1);

                cin.clear();
                registroAutoModificado = registroAuto;
                outArchivoTemp << registroAuto[0] << " " << quoted(registroAuto[1]) << " "
                               << registroAuto[2] << " " << registroAuto[3] << " "
                               << quoted(registroAuto[4]) << " " << registroAuto[5] << " "
                               << registroAuto[6] << endl;
            }
        }

        outArchivoTemp.close();
        inArchivoAutos.close();

        string seleccionUsuario;

        if (autoEncontrado)
        {
            if (!confirmarAccion("¿Está seguro de que deesea modificar la velocidad máxima y los caballos de fuerza?"))
            {
                cout << "Modificción cancelada por el usuario" << endl;
                return;
            }
            else if (remove(direccionArchivo.c_str()) != 0)
            {
                cerr << "Error al eliminar el archivo original: " << direccionArchivo << endl;
                remove("temp.txt");
                return;
            }

            if (rename("temp.txt", direccionArchivo.c_str()) != 0)
            {
                cerr << "Error al renombrar el archivo temporal a: " << direccionArchivo << endl;
                return;
            }
            cout << "\n============================ \n"
                 << "El auto se ha modificado correctamente en: " << direccionArchivo << " \n"
                 << "============================ \n";
            cout << convertirRegistroEnString(registroAutoModificado) << endl;
            return;
        }
        else
        {
            cout << "No se encontró el registro del auto " << codigo << endl;
            remove("temp.txt");
            return;
        }
    }
    catch (const exception &e)
    {
        cerr << "No se pudo eliminar el archivo: " << e.what() << endl;
        return;
    }
}

void reporteAutosInventario(const string &direccionArchivo)
{
    // Esta función recorre el archivo de autos e imprime cada registro en consola.
    ifstream inArchivosAutos(direccionArchivo, ios::in);
    array<string, columnasRegistro> registroAuto;
    if (!inArchivosAutos)
    {
        cout << "Error: No se pudo abrir el archivo " << direccionArchivo << endl;
    }
    else
    {
        cout << "El archivo se abrió correctamente" << endl;
        cout << endl;
        cout << "============================ \n"
             << "Reporte de autos en inventario \n"
             << "============================ \n";
        cout << endl;
        cout << left << setw(11) << "Codigo" << "|"
             << setw(27) << "Nombre" << "|"
             << setw(10) << "Velocidad" << "|"
             << setw(10) << "Caballos" << "|"
             << setw(9) << "Costo" << "|"
             << setw(15) << "ID Propietario" << "|" << endl;

        while (inArchivosAutos >> registroAuto[0] >> quoted(registroAuto[1]) >> registroAuto[2] >> registroAuto[3] >> quoted(registroAuto[4]) >> registroAuto[5] >> registroAuto[6])
        {
            cout << left << setw(11) << registroAuto[0] << "|"
                 << setw(27) << registroAuto[1] << "|"
                 << setw(10) << registroAuto[2] << "|"
                 << setw(10) << registroAuto[3] << "|"
                 << setw(9) << registroAuto[5] << "|"
                 << setw(15) << registroAuto[6] << "|" << endl;
        }
    }
}

bool setAuto(const array<string, columnasRegistro> &infoAuto, string direccionArchivo)
{
    // Esta función escribe directamente en el archivo de autos el registro del auto.
    ofstream appArchivoAutos(direccionArchivo, ios::app);
    if (!appArchivoAutos)
    {
        cout << "Hubo un error, no se pudo abrir el archivo!" << endl;
        return false;
    }
    else
    {
        cout << "El archivo se pudo abrir exitosamente!" << endl;
    }

    string registroAuto = convertirRegistroEnString(infoAuto);

    appArchivoAutos << registroAuto << endl;
    appArchivoAutos.close();

    return true;
}

string solicitarCodigoAuto()
{
    string codigoAuto;
    bool entradaCorrecta = false;
    do
    {
        cout << "Ingrese el código del automóvil (formato A00000000): ";
        cin.width(9);
        getline(cin, codigoAuto);

        cout << endl;
        if (codigoAuto.empty()) // Validar que el código no esté vacío
        {
            cerr << "Error: Código de auto no puede estar vacío." << endl;
            entradaCorrecta = false;
        }
        else if (!validarCodigo(codigoAuto, 'A')) // Validar el formato del código
        {
            cerr << "Código inválido, por favor siga el formato." << endl;
            entradaCorrecta = false;
        }
        else
        {
            entradaCorrecta = true;
        }
    } while (!entradaCorrecta);

    return codigoAuto;
}

// Gestor competencias

array<string, columnasRegistro> getCompetencia(const string codigo, const string direccionArchivo)
{
    // Esta función busca un registro de competencia en el inventario y lo devuelve como un array de strings.
    array<string, columnasRegistroCompetencia> registroCompetencia;

    fstream inArchivoAutos(direccionArchivo, ios::in);
    if (!inArchivoAutos)
    {
        cerr << "No se pudo abrir el archivo" << endl;
    }
    else
    {
        cout << "El archivo se abrió correctamente" << endl;
    }
    // Busca el registro de competencia en el archivo y lo devuelve como un array de strings.
    while (inArchivoAutos >> registroCompetencia[0] >> registroCompetencia[1] >> registroCompetencia[2] >> registroCompetencia[3] >> quoted(registroCompetencia[4]) >> quoted(registroCompetencia[5]) >> registroCompetencia[6])
    {
        if (registroCompetencia[0] == codigo)
        {
            cout << "Competencia encontrada!" << endl;
            return registroCompetencia;
        }
    }
    cout << "Competencia no encontrada!" << endl;
    inArchivoAutos.close();
    return {""};
}

void imprimirDatosCompetencia(const array<string, columnasRegistro> &registroCompetencia)
{
    cout << "\n============================ \n";
    cout << "Datos de la competencia encontrada \n";
    cout << "============================ \n";
    cout << "Código de competencia: " << registroCompetencia[0] << endl;
    cout << "Código de Auto 1: " << registroCompetencia[1] << endl;
    cout << "Código de Auto 2: " << registroCompetencia[2] << endl;
    cout << "Fecha: " << registroCompetencia[3] << " HP" << endl;
    cout << "Categoría: " << registroCompetencia[4] << endl;
    cout << "Estado: " << registroCompetencia[5] << endl;
    cout << "Ganador: " << registroCompetencia[6] << endl;
}

string menuCategoriaCompetencia()
{
    int seleccionMenu = 0;
    bool entradaCorrecta = false;
    string categoriaCompetencia = "";

    string menu = "\n\n************************************\n"
                  "Seleccione la categoría de la carrera.\n"
                  "1. Sprint.\n"
                  "2. Endurance.\n"
                  "3. Drift.\n"
                  "Seleccione una opcion: ";

    do
    {
        cout << menu;
        cin >> seleccionMenu;

        if (!cin.good())
        {
            cout << "Sólo se permite el ingreso de números" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Presione Enter para continuar...";
            cin.get();
        }
        else
        {
            switch (seleccionMenu)
            {

            case 1:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                categoriaCompetencia = "Sprint";
                entradaCorrecta = true;
                break;
            case 2:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                categoriaCompetencia = "Endurance";
                entradaCorrecta = true;
                break;
            case 3:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                categoriaCompetencia = "Drift";
                entradaCorrecta = true;
                break;
            default:
                cout << "\nERROR: No existe una opción con este número, intenta de nuevo.\n";
                entradaCorrecta = false;
                cout << "Presione Enter para continuar...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            }
        }

    } while (!entradaCorrecta);

    return categoriaCompetencia;
}

string menuEstadoCompetencia()
{
    int seleccionMenu = 0;
    bool entradaCorrecta = false;
    string estadoCompetencia = "";

    string menu = "\n\n************************************\n"
                  "Seleccione el estado de la competencia.\n"
                  "1. En proceso.\n"
                  "2. Cancelada.\n"
                  "3. Finalizada.\n"
                  "Seleccione una opcion: ";

    do
    {
        cout << menu;
        cin >> seleccionMenu;

        if (!cin.good())
        {
            cout << "Sólo se permite el ingreso de números" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Presione Enter para continuar...";
            cin.get();
        }
        else
        {
            switch (seleccionMenu)
            {

            case 1:
                estadoCompetencia = "En proceso";
                entradaCorrecta = true;
                break;
            case 2:
                estadoCompetencia = "Cancelada";
                entradaCorrecta = true;
                break;
            case 3:
                estadoCompetencia = "Finalizada";
                entradaCorrecta = true;
                break;
            default:
                cout << "\nERROR: No existe una opción con este número, intenta de nuevo.\n";
                entradaCorrecta = false;
                cout << "Presione Enter para continuar...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            }
        }

    } while (!entradaCorrecta);

    return estadoCompetencia;
}

string menuGanadorCompetencia(const string auto1, const string auto2)
{
    /// Esta función muestra un menú para seleccionar el ganador de la competencia.
    int seleccionMenu = 0;
    bool entradaCorrecta = false;
    string autoGanador = "";

    string menu = "\n\n************************************\n"
                  "Seleccione el ganador de la competencia.\n"
                  "1. " +
                  auto1 + "\n"
                          "2. " +
                  auto2 + "\n"
                          "Seleccione una opcion: ";

    do
    {
        seleccionMenu = mostrarMenu(menu, 2);

        switch (seleccionMenu)
        {

        case 1:
            autoGanador = auto1;
            entradaCorrecta = true;
            break;
        case 2:
            autoGanador = auto2;
            entradaCorrecta = true;
            break;
        default:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nERROR: No existe una opción con este número, intenta de nuevo.\n";
            entradaCorrecta = false;
            cout << "Presione Enter para continuar...";
            cin.get();
            break;
        }

    } while (!entradaCorrecta);

    return autoGanador;
}

void reporteCompetencias(const string &direccionArchivo)
{
    ifstream inArchivosCompetencias(direccionArchivo, ios::in);
    array<string, columnasRegistro> registroCompetencia;
    if (!inArchivosCompetencias)
    {
        cout << "Error: No se pudo abrir el archivo " << direccionArchivo << endl;
    }
    else
    {
        cout << "El archivo se abrió correctamente" << endl;
        cout << endl;
        cout << "============================ \n"
             << "Reporte de competencias \n"
             << "============================ \n";
        cout << endl;
        cout << left << setw(11) << "Codigo" << "|"
             << setw(11) << "Auto 1" << "|"
             << setw(11) << "Auto 2" << "|"
             << setw(10) << "Fecha" << "|"
             << setw(25) << "Categoria" << "|"
             << setw(25) << "Estado" << "|"
             << setw(10) << "Ganador" << "|" << endl;

        while (inArchivosCompetencias >> registroCompetencia[0] >> registroCompetencia[1] >> registroCompetencia[2] >> registroCompetencia[3] >> quoted(registroCompetencia[4]) >> quoted(registroCompetencia[5]) >> registroCompetencia[6])
        {
            cout << left << setw(11) << registroCompetencia[0] << "|"
                 << setw(11) << registroCompetencia[1] << "|"
                 << setw(11) << registroCompetencia[2] << "|"
                 << setw(10) << registroCompetencia[3] << "|"
                 << setw(25) << registroCompetencia[4] << "|"
                 << setw(25) << registroCompetencia[5] << "|"
                 << setw(10) << registroCompetencia[6] << "|" << endl;
        }
    }
}

bool setCompetencia(const array<string, columnasRegistro> &infoCompetencia, string direccionArchivo)
{
    ofstream appArchivoCompetencias(direccionArchivo, ios::app);
    if (!appArchivoCompetencias)
    {
        cout << "Hubo un error, no se pudo abrir el archivo!" << endl;
        return false;
    }
    else
    {
        cout << "El archivo se pudo abrir exitosamente!" << endl;
    }

    string registroCompetencia = convertirRegistroEnString(infoCompetencia);

    appArchivoCompetencias << registroCompetencia << endl;
    appArchivoCompetencias.close();

    return true;
}
// Validador entradas
bool validarCodigo(const string &codigo, const char tipo)
{
    // Esta función valida el formato del código mediante una expresión regular.
    regex regexRule((string(1, tipo) + "\\d{8}"));

    if (!regex_match(codigo, regexRule))
    {
        return false;
    }

    return true;
}

bool validarFormatoFecha(const string &fecha)
{
    // Esta función valida el formato de la fecha en el formato dd/mm/yyyy
    // mediante una expresión regular.
    regex regexRule("(\\d{2})[/](\\d{2})[/](\\d{4})");

    if (!regex_match(fecha, regexRule))
    {
        return false;
    }

    return true;
}

bool validarNombre(const string nombre)
{
    if (nombre.length() > 25)
    {
        return false;
    }
    return true;
}

bool validarNumeroRango(const string &numero, const int max, const int min)
{
    try
    {
        int num = stoi(numero);
        if (num > max || num < min)
        {
            return false;
        }
        return true;
    }
    catch (const invalid_argument &e)
    {
        cerr << "Error: El número ingresado no es válido." << endl;
        return false;
    }
    catch (const out_of_range &e)
    {
        cerr << "Error: El número ingresado está fuera del rango permitido." << endl;
        return false;
    }
}

bool validarSoloNumero(const string &numero)
{

    for (char c : numero)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }

    return true;
}

// Gestor general
bool confirmarAccion(const string &mensaje)
{
    string seleccionUsuario;
    bool entradaCorrecta = false;

    do
    {
        cout << mensaje << " (y/n): ";
        cin >> seleccionUsuario;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (seleccionUsuario == "Y" || seleccionUsuario == "y")
        {
            return true;
        }
        else if (seleccionUsuario == "N" || seleccionUsuario == "n")
        {
            return false;
        }
        else
        {
            cerr << "Error: Respuesta inválida\n";
            entradaCorrecta = false;
        }
    } while (!entradaCorrecta);

    return false;
}

void consultarAutoInventario(const string direccionArchivo)
{
    string codigoAuto = "";

    bool deseaSalir = false;
    cout << "\n============================ \n";
    cout << "Consultar auto del inventario \n";
    cout << "============================ \n";
    do
    {
        bool entradaCorrecta = false;
        cout << "Ingrese el código del automóvil (formato A00000000): ";
        cin.width(9);
        getline(cin, codigoAuto);

        cout << endl;
        if (codigoAuto.empty())
        {
            cerr << "Error: Código de auto no puede estar vacío." << endl;
            entradaCorrecta = false;
        }
        else if (!validarCodigo(codigoAuto, 'A'))
        {
            cerr << "Código inválido, por favor siga el formato." << endl;
            entradaCorrecta = false;
        }
        else
        {
            array<string, columnasRegistro> contenidoAuto = getAuto(codigoAuto, direccionArchivo);
            if (!contenidoAuto.empty() && contenidoAuto[0] == codigoAuto)
            {
                imprimirDatosAuto(contenidoAuto);
            }
            else
            {
                cerr << "Error: El auto no se ha encontrado" << endl;
            }

            deseaSalir = confirmarAccion("¿Desea volver al menu principal?");
        }
    } while (!deseaSalir);
}

void consultarCompetenciaInventario(const string direccionArchivo)
{
    string codigoCompetencia = "";

    bool deseaSalir = false;
    cout << "\n============================ \n";
    cout << "Consultar competencia \n";
    cout << "============================ \n";
    do
    {
        bool entradaCorrecta = false;
        cout << "Ingrese el código de la competencia (formato C00000000): ";
        cin.width(9);
        getline(cin, codigoCompetencia);
        cout << endl;
        // Validar que el código no esté vacío
        // Validar el formato del código
        if (codigoCompetencia.empty())
        {
            cerr << "Error: Código de competencia no puede estar vacío." << endl;
            entradaCorrecta = false;
        }
        else if (!validarCodigo(codigoCompetencia, 'C'))
        {
            cerr << "Código inválido, por favor siga el formato." << endl;
            entradaCorrecta = false;
        }
        else
        {
            array<string, columnasRegistro> contenidoCompetencia = getCompetencia(codigoCompetencia, direccionArchivo);
            // Busca el registro de competencia en el archivo y lo devuelve como un array de strings.
            if (!contenidoCompetencia.empty() && contenidoCompetencia[0] == codigoCompetencia)
            {
                cout << "Competencia encontrada!" << endl;
                imprimirDatosCompetencia(contenidoCompetencia);
            }
            else
            {
                cerr << "Error: La competencia no se ha encontrado" << endl;
            }

            deseaSalir = confirmarAccion("¿Desea volver al menu principal?");
        }
    } while (!deseaSalir);
}

bool ingresarAutoAlInventario(const string direccioArchivo)
{
    string nombreAuto, codigo, equipoPropietario;
    string velocidadMaxima, caballosFuerza;
    string costoAuto, identificacionPropietario;
    array<string, columnasRegistro> datosAuto;

    cout << "\n============================ \n";
    cout << "INGRESAR AUTO A INVENTARIO \n";
    cout << "============================ \n";

    codigo = generarCodigoUnico("A", direccioArchivo);
    cout << "Código de automóvil (generado automáticamente): " << codigo << endl;

    solicitarTexto("Ingrese el nombre del automóvil (máximo 25 caracteres): ", nombreAuto, 25);
    solicitarNumero("Ingrese la velocidad máxima (km/h): ", 4, velocidadMaxima, 999, 1);
    solicitarNumero("Ingrese los caballos de fuerza(hp): ", 4, caballosFuerza, 999, 1);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    solicitarTexto("Ingrese el equipo/propietario del automóvil (máximo 25 caracteres): ", equipoPropietario, 25);
    solicitarNumero("Ingrese el costo del automóvil (sin punto o coma decimal): ", 9, costoAuto, 99999999, 1);

    string mensaje = "Ingrese el número de identificación de quien registra el automóvil";
    mensaje += "\n (más de 8 cifras serán omitidas): ";
    solicitarNumero(mensaje, 9, identificacionPropietario, 99999999, 1);

    formatearParaSalida(nombreAuto); // Formatear para escribir en el archivo
    formatearParaSalida(equipoPropietario);

    datosAuto = array<string, columnasRegistro>{codigo, nombreAuto, velocidadMaxima, caballosFuerza, equipoPropietario, costoAuto, identificacionPropietario};

    return setAuto(datosAuto, direccioArchivo);
}

bool ingresarCompetenciaAlInventario(const string direccionArchivo, const string direccionArchivoAutos)
{
    string categoriaCompetencia, codigo, fechaCompetencia;
    string estadoCompetencia, codigoAuto1;
    string codigoAuto2, autoGanador;
    array<string, columnasRegistro> datosCompetencia;
    array<string, 7> registroCompetencia;

    bool entradaCorrecta = false;

    cout << "\n============================ \n";
    cout << "REGISTRO DE AUTO A COMPETENCIA \n";
    cout << "============================ \n";

    codigo = generarCodigoUnico("C", direccionArchivo);
    cout << "Código único de competencia (generado automáticamente): " << codigo << endl;

    bool deseaSalir = false;
    cout << "\n============================ \n";
    cout << "Ingresar autos del inventario \n";
    cout << "============================ \n";

    array<array<string, columnasRegistro>, 2> contenidoAutos;

    // Solicita el auto 1, valida y confirma solicitud de usuario.
    do
    {
        cout << "Ingrese el código del automóvil 1 (formato A00000000): ";
        cin.width(9);
        getline(cin, codigoAuto1);

        cout << endl;
        if (codigoAuto1.empty())
        {
            cerr << "Error: Código de auto no puede estar vacío." << endl;
            entradaCorrecta = false;
        }
        else if (!validarCodigo(codigoAuto1, 'A'))
        {
            cerr << "Código inválido, por favor siga el formato." << endl;
            entradaCorrecta = false;
        }
        else
        {
            contenidoAutos[0] = getAuto(codigoAuto1, direccionArchivoAutos);
            if (!contenidoAutos[0][0].empty())
            {
                entradaCorrecta = true;
            }
            else
            {
                cerr << "Error: El auto no se ha encontrado" << endl;
                entradaCorrecta = false;
            }
        }
    } while (!entradaCorrecta);

    entradaCorrecta = false;

    // Solicita el auto 2, valida y confirma solicitud de usuario.
    do
    {
        cout << "Ingrese el código del automóvil 2 (formato A00000000): ";
        cin.width(9);
        getline(cin, codigoAuto2);

        cout << endl;
        if (codigoAuto2.empty())
        {
            cerr << "Error: Código de auto no puede estar vacío." << endl;
            entradaCorrecta = false;
        }
        else if (codigoAuto1 == codigoAuto2) // Validar que no se repita el auto 1
        {
            cerr << "No puede colocarse dos veces un auto en una competencia" << endl;
            entradaCorrecta = false;
        }
        else if (!validarCodigo(codigoAuto2, 'A'))
        {
            cerr << "Código inválido, por favor siga el formato." << endl;
            entradaCorrecta = false;
        }
        else
        {
            contenidoAutos[1] = getAuto(codigoAuto2, direccionArchivoAutos);
            if (!contenidoAutos[1][0].empty()) // Validar que el auto exista
            {
                entradaCorrecta = true;
            }
            else
            {
                cerr << "Error: El auto no se ha encontrado" << endl;
                entradaCorrecta = false;
            }
        }
    } while (!entradaCorrecta);

    entradaCorrecta = false;

    categoriaCompetencia = menuCategoriaCompetencia();

    do
    {
        cout << "Ingrese la fecha de la competencia (DD/MM/AAAA):" << endl;
        cin.width(10);
        getline(cin, fechaCompetencia);
        cout << endl;
        if (fechaCompetencia.empty())
        {
            cout << "Fecha no puede estar vacía" << endl;
            entradaCorrecta = false;
            continue;
        }
        else if (!validarFormatoFecha(fechaCompetencia))
        {
            entradaCorrecta = false;
            cerr << "Error: El formato de la fecha no es válido." << endl;
            continue;
        }
        entradaCorrecta = true;

    } while (!entradaCorrecta);

    entradaCorrecta = false;

    estadoCompetencia = menuEstadoCompetencia();

    if (estadoCompetencia == "Finalizada")
    {
        autoGanador = menuGanadorCompetencia(codigoAuto1, codigoAuto2);
        cout << "El ganador de la competencia es: " << autoGanador << endl;
    }
    else
    {
        autoGanador = "N/A";
    }

    formatearParaSalida(categoriaCompetencia); // Formatear para escribir en el archivo
    formatearParaSalida(estadoCompetencia);

    datosCompetencia = array<string, columnasRegistro>{codigo, codigoAuto1, codigoAuto2, fechaCompetencia, categoriaCompetencia, estadoCompetencia, autoGanador};

    if (setCompetencia(datosCompetencia, direccionArchivo))
    {
        cout << "Presione Enter para continuar...";
        cin.get();
        return true;
    }
    else
    {
        cout << "Presione Enter para continuar...";
        cin.get();
        return false;
    }
}

void menuInventarioAutos(const string direccionArchivo)
{

    string menu = "\n\n************************************\n"
                  "Inventario de Autos.\n"
                  "1. Ingresar auto al inventario.\n"
                  "2. Consultar auto del inventario.\n"
                  "3. Modificar auto del inventario.\n"
                  "4. Eliminar auto del inventario\n"
                  "5. Salir de inventario de autos.\n"
                  "Seleccione una opcion: ";
    bool salir = false;
    do
    {
        int seleccion = mostrarMenu(menu, 5);

        switch (seleccion)
        {
        case 1:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ingresarAutoAlInventario(direccionArchivo);
            cout << "Presione Enter para continuar...";
            cin.get();
            break;

        case 2:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            consultarAutoInventario(direccionArchivo);
            cout << "Presione Enter para continuar...";
            cin.get();
            break;

        case 3:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            modificarRegistroAuto(solicitarCodigoAuto(), direccionArchivo);
            cout << "Presione Enter para continuar...";
            cin.get();
            break;

        case 4:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            eliminarAuto(direccionArchivo);
            cout << "Presione Enter para continuar...";
            cin.get();
            break;

        case 5:
            salir = true;
            break;
        }

    } while (!salir);
}

void menuInventarioCompetencias(const string direccionArchivoCompetencias, const string direccionArchivoAutos)
{

    string menu = "\n\n************************************\n"
                  "Registro de Competencias.\n"
                  "1. Inscribir auto en competencia.\n"
                  "2. Consultar competencia.\n"
                  "3. Salir del registro de competencias.\n"
                  "Seleccione una opcion: ";
    bool salir = false;
    do
    {
        int seleccion = mostrarMenu(menu, 3);

        switch (seleccion)
        {

        case 1:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ingresarCompetenciaAlInventario(direccionArchivoCompetencias, direccionArchivoAutos);
            cout << "Presione Enter para continuar...";
            cin.get();
            break;
        case 2:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            consultarCompetenciaInventario(direccionArchivoCompetencias);
            cout << "Presione Enter para continuar...";
            cin.get();
            break;

        case 3:
            salir = true;
            break;
        }

    } while (!salir);
}

int mostrarMenu(const string menu, const int opciones)
{
    // Ayuda a mostrar cualquier menú y validar la entrada del usuario.
    // Devuelve la opción seleccionada por el usuario.
    int seleccionMenu = 0;
    bool entradaCorrecta = false;

    do
    {
        cout << menu;
        cin >> seleccionMenu;

        if (!cin.good() || seleccionMenu < 1 || seleccionMenu > opciones)
        {
            cerr << "Error: Selección inválida. Intente de nuevo." << endl;
            cin.clear();
        }
        else
        {
            entradaCorrecta = true;
        }
    } while (!entradaCorrecta);

    return seleccionMenu;
}

bool solicitarNumero(const string &mensaje, const short anchoCampo, string &numero, int max, int min)
{
    bool entradaCorrecta = false;
    do
    {
        cout << mensaje;
        cin >> setw(anchoCampo) >> numero;
        cin.width(0);
        if (cin.fail())
        {
            cerr << "Error: Entrada inválida. Por favor, ingrese un número válido." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            entradaCorrecta = false;
        }
        else if (!validarSoloNumero(numero))
        {
            cerr << "Error: Sólo está permitido ingresar números";
        }
        else if (!validarNumeroRango(numero, max, min))
        {
            cerr << "El dato que ingresaste está fuera del rango" << endl;
            entradaCorrecta = false;
        }
        else
        {
            entradaCorrecta = true;
        }
    } while (!entradaCorrecta);

    numero = std::string((anchoCampo - 1) - numero.length(), '0') + numero;

    return true;
}

bool solicitarTexto(const string mensaje, string &texto, int maxLongitud)
{
    // Esta función solicita un texto al usuario y valida su longitud,
    // asegurándose de que no esté vacío y no exceda la longitud máxima permitida.
    bool entradaCorrecta = false;
    do
    {
        cout << mensaje;
        getline(cin, texto);

        if (texto.empty())
        {
            cerr << "El texto no puede estar vacío." << endl;
            entradaCorrecta = false;
        }
        else if (texto.length() > maxLongitud)
        {
            cerr << "El texto excede la longitud máxima permitida." << endl;
            entradaCorrecta = false;
        }
        else
        {
            entradaCorrecta = true;
        }
    } while (!entradaCorrecta);

    return true;
}

string convertirRegistroEnString(const array<string, columnasRegistro> &infoAuto)
{
    // Esta función convierte un array de strings en un string formateado para el archivo.
    // Separa los campos con un espacio.
    string registro;
    for (const string &campo : infoAuto)
    {
        if (!registro.empty())
            registro.append(" ");
        registro += campo;
    }

    return registro;
}

// Formateador salida
void formatearParaSalida(string &dato)
{
    if (dato.length() > 25)
    {
        dato = dato.substr(0, 25); // Limitar a 25 caracteres
    }
    dato = u8"\"" + dato + string(25 - dato.length(), ' ') + "\"";
}

string generarCodigoUnico(const string tipoCodigo, const string direccionArchivo)
{

    int numeroGenerado = 1;
    const unsigned int maxNum = 99999999; // 8 dígitos
    string codigo;
    array<string, columnasRegistro> camposRegistro;

    fstream inArchivoAutos(direccionArchivo, ios::in);
    if (!inArchivoAutos)
    {
        cerr << "No se pudo abrir el archivo. Generando el primer código." << endl;
        return tipoCodigo + "00000001";
    }

    cout << "El archivo se abrió correctamente." << endl;

    // Revisa si el archivo está vacío.
    inArchivoAutos.seekg(0, ios::end);
    if (inArchivoAutos.tellg() == 0)
    {
        inArchivoAutos.close();
        return tipoCodigo + "00000001";
    }

    inArchivoAutos.seekg(0, ios::beg);

    vector<string> codigosExistentes;

    while (inArchivoAutos >> camposRegistro[0])
    {
        codigosExistentes.push_back(camposRegistro[0]);
        inArchivoAutos.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    inArchivoAutos.close();

    while (numeroGenerado <= maxNum)
    {
        // Genera el código con ceros a la izquierda.
        codigo = tipoCodigo + string(8 - to_string(numeroGenerado).length(), '0') + to_string(numeroGenerado);

        // Verifica si el código ya existe en el archivo.
        // Si no existe, lo devuelve.
        if (find(codigosExistentes.begin(), codigosExistentes.end(), codigo) == codigosExistentes.end())
        {
            return codigo;
        }
        numeroGenerado++;
    }

    cerr << "No se pudo generar un código único. Se alcanzó el límite máximo." << endl;
    return "";
}