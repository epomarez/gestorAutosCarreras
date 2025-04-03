#include <array>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <optional>
#include <regex>
#include <string>

using namespace std;

// Gestor autos
// const size_t registrosAutos{25};
const size_t columnasRegistro{7};
void eliminarAuto(const string &);
bool eliminarRegistroAuto(const string &, const string &);
bool confirmarEliminacionAuto();
string generarCodigoUnico(const string, const string);
array<string, columnasRegistro> getAuto(const string, const string);
void imprimirDatosAuto(const array<string, columnasRegistro> &);
string modificarAuto(const array<array<char, 3>, 2> &);
bool setAuto(const array<string, columnasRegistro> &, const string);
string solicitarCodigoAuto();

// Gestor competencias
const size_t filasCompetencias{7};
const size_t columnasRegistroCompetencia{7};
// bool eliminarCompetencia(const string, const string);
// string generarCodigoCompetencia();
array<string, columnasRegistro> getCompetencia(const string, const string);
bool setCompetencia(const array<string, columnasRegistro> &, const string);
// string modificarCompetencia(const string, const string[]);

// Validador entradas
bool validarCodigoAuto(const string &);
bool validarCodigoCompetencia(const string);
bool validarFormatoFecha(const string &);
bool validarNombre(string);
bool validarNumeroRango(const string &, const int, const int);
bool validarSoloNumero(const string &);

bool confirmarAccion(const string &);
bool solicitarNumero(const string &, const short, string &, int, int);
// Gestor General
//! Debe confirmar ingreso antes de escribir en archivo.
bool ingresarAutoAlInventario(const string);
void consultarAutoInventario(const string);

//? Formateador Salida
void formatearParaSalida(string &);
string formatearSalidaDatosCompetencia(string[]);

//? Formateador Entrada
string convertirRegistroEnString(const array<string, columnasRegistro> &);
// string formatearEntradaDatosCompetencia(string[]);
//* Funcion main
int main()
{
    const string direccionArchivo = "Autos.txt";
    int seleccionMenu = 0;
    bool salir = false;

    string menuInicio = "\n\n************************************\n"
                        "Inventario de Autos.\n"
                        "1. Ingresar auto al inventario.\n"
                        "2. Consultar auto del inventario.\n"
                        "3. Modificar auto del inventario.\n"
                        "4. Eliminar auto del inventario\n"
                        "5. Salir de inventario de autos.\n"
                        "Seleccione una opcion: ";

    do
    {
        cout << menuInicio;
        cin >> seleccionMenu;

        if (!cin.good())
        {
            cout << "Sólo se permite el ingreso de números" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            switch (seleccionMenu)
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

                // case 3:
                //     mostrarEquipoPokemon(equipoPokemon, nombresPokemones, tipoPokemones, cantidadPokemones);
                //     cout << "Presione Enter para continuar...";
                //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
                //     cin.get();
                //     break;

            case 4:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                eliminarAuto(direccionArchivo);
                cout << "Presione Enter para continuar...";
                cin.get();
                break;

            case 5:
                salir = true;
                break;
            default:
                cout << "\nERROR: No existe una opción con este número, intenta de nuevo.\n";
                cout << "Presione Enter para continuar...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            }
        }

    } while (!salir);
    string resultado = ingresarAutoAlInventario("Autos.txt") ? "Auto ingresado correctamente!" : "Error: El auto no se ingresó correctamente";
    cout << resultado;
    return 0;
}

// Validador entradas
bool validarCodigoAuto(const string &codigo)
{
    regex regexRule("A\\d{8}");

    if (!regex_match(codigo, regexRule))
    {
        return false;
    }

    return true;
}

bool validarCodigoCompetencia(const string &codigo)
{
    regex regexRule("C\\d{8}");

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

bool solicitarNumero(const string &mensaje, const short anchoCampo, string &numero, int max, int min)
{
    bool entradaCorrecta = false;
    do
    {
        cout << mensaje;
        cin >> setw(anchoCampo) >> numero;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

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

    return true;
}
// Gestor autos
bool leerRegistro(ifstream &inArchivo, array<string, 7> &registro)
{
    for (string &campo : registro)
    {
        if (!(inArchivo >> campo))
        {
            return false;
        }
    }
    return true;
}

void eliminarAuto(const string &direccionArchivo)
{
    eliminarRegistroAuto(solicitarCodigoAuto(), direccionArchivo);
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
        else if (!validarCodigoAuto(codigoAuto))
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

void formatearParaSalida(string &dato)
{
    dato = "\"" + dato + string(25 - dato.length(), ' ') + "\"";
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

    while (inArchivoAutos >> registroCompetencia[0] >> registroCompetencia[1] >> registroCompetencia[2] >> registroCompetencia[3] >> registroCompetencia[4] >> registroCompetencia[5] >> registroCompetencia[6])
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

// Gestor general
bool ingresarAutoAlInventario(const string direccioArchivo)
{
    string nombreAuto, codigo, equipoPropietario;
    string velocidadMaxima, caballosFuerza;
    string costoAuto, identificacionPropietario;
    array<string, columnasRegistro> datosAuto;
    array<string, 7> registroAuto;

    bool entradaCorrecta = false;

    cout << "\n============================ \n";
    cout << "INGRESAR AUTO A INVENTARIO \n";
    cout << "============================ \n";

    codigo = generarCodigoUnico("A", direccioArchivo);
    cout << "Código de automóvil (generado automáticamente): " << codigo << endl;

    do
    {
        cout << "Ingrese el nombre del automóvil (más de 25 caractéres serán omitidos): ";
        cin.width(25);
        getline(cin, nombreAuto);

        cout << endl;
        if (nombreAuto.empty())
        {
            cout << "Nombre inválido" << endl;
            entradaCorrecta = false;
        }
        else
        {
            entradaCorrecta = true;
        }
    } while (!entradaCorrecta);

    entradaCorrecta = false;

    string mensaje = "Ingrese la velocidad máxima (km/h) del automóvil ";
    mensaje += "\n (igual a 0 & más de 3 cifras serán omitidas): ";
    entradaCorrecta = solicitarNumero(mensaje, 4, identificacionPropietario, 999, 1);

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    entradaCorrecta = false;

    string mensaje = "Ingrese los caballos de fuerza(hp) del automóvil";
    mensaje += "\n (más de 3 cifras serán omitidas): ";
    entradaCorrecta = solicitarNumero(mensaje, 4, identificacionPropietario, 999, 1);

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    entradaCorrecta = false;

    do
    {
        cout << "Ingrese el Equipo/Propietario del automóvil" << endl
             << " (más de 25 caractéres serán omitidos): ";
        cin.width(25);
        getline(cin, equipoPropietario);
        cout << endl;
        if (equipoPropietario.empty())
        {
            cout << "Nombre invalido" << endl;
            entradaCorrecta = false;
        }
        else
        {
            entradaCorrecta = true;
        }
    } while (!entradaCorrecta);

    entradaCorrecta = false;

    string mensaje = "Ingrese el costo del automóvil";
    mensaje += "\n (Sin coma decimal, más de 8 cifras serán omitidas): ";
    entradaCorrecta = solicitarNumero(mensaje, 9, identificacionPropietario, 99999999, 1);

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    entradaCorrecta = false;

    string mensaje = "Ingrese el número de identificación de quien registra el automóvil";
    mensaje += "\n (más de 8 cifras serán omitidas): ";
    entradaCorrecta = solicitarNumero(mensaje, 9, identificacionPropietario, 99999999, 1);

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    formatearParaSalida(nombreAuto);
    formatearParaSalida(equipoPropietario);

    datosAuto = array<string, columnasRegistro>{codigo, nombreAuto, velocidadMaxima, caballosFuerza, equipoPropietario, costoAuto, identificacionPropietario};

    if (setAuto(datosAuto, direccioArchivo))
    {
        return true;
    }
    else
    {
        return false;
    }
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
        else if (!validarCodigoAuto(codigoAuto))
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
// Formateador Entrada?
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