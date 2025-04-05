// TODO: Al ingresar, e ingresar texto con tilde, aparece como null en el .txt.
// TODO: La opción para salir del inventario no está configurada propiamente.
// TODO: Falta la funcionalidad de competencias.
// TODO: El menú de autos es sólo un submenu de otro principal.
#include <array>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <optional>
#include <locale>
#include <regex>
#include <windows.h>
#include <string>

using namespace std;

// Gestor autos
const size_t columnasRegistro{7};
bool confirmarEliminacionAuto();
void eliminarAuto(const string &);
bool eliminarRegistroAuto(const string &, const string &);
array<string, columnasRegistro> getAuto(const string, const string);
void imprimirDatosAuto(const array<string, columnasRegistro> &);
void modificarRegistroAuto(const string &, const string &);
bool setAuto(const array<string, columnasRegistro> &, const string);
string solicitarCodigoAuto();

// Gestor competencias
const size_t columnasRegistroCompetencia{7};
array<string, columnasRegistro> getCompetencia(const string, const string);
void imprimirDatosCompetencia(const array<string, columnasRegistro> &);
bool setCompetencia(const array<string, columnasRegistroCompetencia> &, const string);

// Validador entradas
bool validarCodigo(const string &, const char);
bool validarFormatoFecha(const string &);
bool validarNombre(string);
bool validarNumeroRango(const string &, const int, const int);
bool validarSoloNumero(const string &);

// Gestor General
//! Debe confirmar ingreso antes de escribir en archivo.
bool confirmarAccion(const string &);
void consultarAutoInventario(const string);
void consultarCompetenciaInventario(const string);
string generarCodigoUnico(const string, const string);
bool ingresarAutoAlInventario(const string);
bool ingresarCompetenciaAlInventario(const string, const string);
void menuInventarioAutos(const string);
void menuInventarioCompetencias(const string, const string);
bool solicitarNumero(const string &, const short, string &, int, int);
bool solicitarTexto(const string, string &, int);
int mostrarMenu(const string, const int);

//? Formateador Salida
void formatearParaSalida(string &);
string formatearSalidaDatosCompetencia(string[]);

//? Formateador Entrada
string convertirRegistroEnString(const array<string, columnasRegistro> &);

//* MAIN
int main()
{
    setlocale(LC_CTYPE, "Spanish");
    consultarAutoInventario("Autos.txt");
    consultarCompetenciaInventario("Competencias.txt");
    return 0;
}

// Gestor autos

void eliminarAuto(const string &direccionArchivo)
{
    eliminarRegistroAuto(solicitarCodigoAuto(), direccionArchivo);
}

bool eliminarRegistroAuto(const string &codigo, const string &direccionArchivo)
{
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
        streampos direccionRegistroEncontrado;

        while (inArchivoAutos >> registroAuto[0] >> quoted(registroAuto[1]) >> registroAuto[2] >> registroAuto[3] >> quoted(registroAuto[4]) >> registroAuto[5] >> registroAuto[6])
        {
            if (registroAuto[0].empty() || !(registroAuto[0] == codigo))
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
        bool deseaEliminar, entradaCorrecta;

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
        streampos direccionRegistroEncontrado;

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
                bool entradaCorrecta = false;

                string mensaje = "Ingrese la velocidad máxima (km/h) del automóvil ";
                mensaje += "\n (igual a 0 & más de 3 cifras serán omitidas): ";
                entradaCorrecta = solicitarNumero(mensaje, 4, registroAuto[2], 999, 1);

                cin.clear();

                mensaje = "Ingrese los caballos de fuerza(hp) del automóvil";
                mensaje += "\n (más de 3 cifras serán omitidas): ";
                entradaCorrecta = solicitarNumero(mensaje, 4, registroAuto[3], 999, 1);

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
        bool deseaEliminar, entradaCorrecta;

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

bool setAuto(const array<string, columnasRegistro> &infoAuto, string direccionArchivo)
{

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
            entradaCorrecta = true;
        }
    } while (!entradaCorrecta);

    return codigoAuto;
}

// Gestor competencias
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

array<string, columnasRegistro> getCompetencia(const string codigo, const string direccionArchivo)
{
    array<string, 7> registroCompetencia;

    fstream inArchivoAutos(direccionArchivo, ios::in);
    if (!inArchivoAutos)
    {
        cerr << "No se pudo abrir el archivo" << endl;
    }
    else
    {
        cout << "El archivo se abrió correctamente" << endl;
    }

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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                estadoCompetencia = "En proceso";
                entradaCorrecta = true;
                break;
            case 2:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                estadoCompetencia = "Cancelada";
                entradaCorrecta = true;
                break;
            case 3:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

// Validador entradas
bool validarCodigo(const string &codigo, const char tipo)
{
    regex regexRule((string(1, tipo) + "\\d{8}"));

    if (!regex_match(codigo, regexRule))
    {
        return false;
    }

    return true;
}

bool validarFormatoFecha(const string &fecha)
{
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
    int num = stoi(numero);
    if (num > max || num < min)
    {
        return false;
    }
    return true;
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
            if (!contenidoAuto.empty())
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
            if (!contenidoCompetencia.empty())
            {
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
    solicitarNumero("Ingrese la velocidad máxima (km/h)", 4, velocidadMaxima, 999, 1);
    solicitarNumero("Ingrese los caballos de fuerza(hp)", 4, caballosFuerza, 999, 1);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    solicitarTexto("Ingrese el equipo/propietario del automóvil (máximo 25 caracteres): ", equipoPropietario, 25);
    solicitarNumero("Ingrese el costo del automóvil (sin punto o coma decimal)", 9, costoAuto, 99999999, 1);

    string mensaje = "Ingrese el número de identificación de quien registra el automóvil";
    mensaje += "\n (más de 8 cifras serán omitidas): ";
    solicitarNumero(mensaje, 9, identificacionPropietario, 99999999, 1);

    formatearParaSalida(nombreAuto);
    formatearParaSalida(equipoPropietario);

    datosAuto = array<string, columnasRegistro>{codigo, nombreAuto, velocidadMaxima, caballosFuerza, equipoPropietario, costoAuto, identificacionPropietario};

    return setAuto(datosAuto, direccioArchivo);
}

bool ingresarCompetenciaAlInventario(const string direccionArchivo, const string direccionArchivoAutos)
{
    string categoriaCompetencia, codigo, fechaCompetencia;
    string estadoCompetencia, codigoAuto1;
    string codigoAuto2, identificacionPropietario;
    array<string, columnasRegistro> datosCompetencia;
    array<string, 7> registroCompetencia;

    bool entradaCorrecta = false;

    cout << "\n============================ \n";
    cout << "REGISTRO DE AUTO A COMPETENCIA \n";
    cout << "============================ \n";

    codigo = generarCodigoUnico("C", direccionArchivo);
    cout << "Código único de competencia (generado automáticamente): " << codigo << endl;

    string codigoAuto;

    bool deseaSalir = false;
    cout << "\n============================ \n";
    cout << "Consultar auto del inventario \n";
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
        else if (!validarCodigo(codigoAuto, 'A'))
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
        else if (codigoAuto2 == codigoAuto1)
        {
            cerr << "No puede colocarse dos veces un auto en una competencia" << endl;
        }
        else if (!validarCodigo(codigoAuto, 'A'))
        {
            cerr << "Código inválido, por favor siga el formato." << endl;
            entradaCorrecta = false;
        }
        else
        {
            contenidoAutos[1] = getAuto(codigoAuto2, direccionArchivoAutos);
            if (!contenidoAutos[1][0].empty())
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

    formatearParaSalida(categoriaCompetencia);
    formatearParaSalida(estadoCompetencia);

    datosCompetencia = array<string, columnasRegistro>{codigo, codigoAuto1, codigoAuto2, fechaCompetencia, categoriaCompetencia, estadoCompetencia};

    if (setCompetencia(datosCompetencia, direccionArchivo))
    {
        return true;
    }
    else
    {
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
            consultarAutoInventario(direccionArchivoCompetencias);
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            entradaCorrecta = true;
        }
    } while (!entradaCorrecta);

    return seleccionMenu;
}
// Formateador Entrada?

bool solicitarNumero(const string &mensaje, const short anchoCampo, string &numero, int max, int min)
{
    bool entradaCorrecta = false;
    do
    {
        cout << mensaje;
        cin >> setw(anchoCampo) >> numero;

        if (!validarSoloNumero(numero))
        {
            cerr << "El dato que ingresaste no es un número o añadiste otros caracteres" << endl;
            entradaCorrecta = false;
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
    dato = u8"\"" + dato + string(25 - dato.length(), ' ') + "\"";
}

string generarCodigoUnico(const string tipoCodigo, const string direccionArchivo)
{

    int numeroGenerado = 1;
    const unsigned int maxNum = 99999999;
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
        codigo = tipoCodigo + string(8 - to_string(numeroGenerado).length(), '0') + to_string(numeroGenerado);

        if (find(codigosExistentes.begin(), codigosExistentes.end(), codigo) == codigosExistentes.end())
        {
            return codigo;
        }
        numeroGenerado++;
    }

    cerr << "No se pudo generar un código único. Se alcanzó el límite máximo." << endl;
    return "";
}
