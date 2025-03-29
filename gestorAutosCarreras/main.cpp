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
bool eliminarAuto(const string &, const string);
string generarCodigoUnico(const string, const string);
array<string, 7> getAuto(const string, const string);
string modificarAuto(const array<array<char, 3>, 2> &);
bool setAuto(const array<string, columnasRegistro> &, const string);

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

// Gestor General
bool ingresarAutoAlInventario(const string);

//? Formateador Salida
void formatearParaSalida(string &);
string formatearSalidaDatosCompetencia(string[]);

//? Formateador Entrada
string convertirRegistroEnString(const array<string, columnasRegistro> &);
// string formatearEntradaDatosCompetencia(string[]);
//* Funcion main
int main()
{

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

// Gestor autos
bool leerRegistro(ifstream &inArchivo, array<string, 7> &registro)
{
    for (auto &campo : registro)
    {
        if (!(inArchivo >> campo))
        {
            return false;
        }
    }
    return true;
}

bool eliminarAuto(const string &codigo, const string direccionArchivo)
{
    try
    {
        ifstream inArchivoAutos(direccionArchivo, ios::in);
        if (!inArchivoAutos)
        {
            cout << "Error: No se pudo abrir el archivo" << direccionArchivo << endl;
            throw direccionArchivo;
        }

        cout << "El archivo se abrió correctamente" << endl;

        ofstream outArchivoTemp("temp.txt", ios::out);
        array<string, columnasRegistro> registroAuto;

        bool autoEncontrado = false;

        while (leerRegistro(inArchivoAutos, registroAuto))
        {
            if (!(registroAuto[0] == codigo))
            {
                for (size_t i = 0; i < registroAuto.size(); ++i)
                {
                    outArchivoTemp << registroAuto[i] << (i < registroAuto.size() - 1 ? " " : "\n");
                }
            }
            else
            {
                autoEncontrado = true;
            }
        }

        outArchivoTemp.close();
        inArchivoAutos.close();

        if (autoEncontrado)
        {
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

void formatearParaSalida(string &dato)
{
    dato = "\"" + string(25 - dato.length(), ' ') + dato + "\"";
}

string generarCodigoUnico(const string tipoCodigo, const string direccionArchivo)
{

    int numeroGenerado = 1;
    const unsigned int maxNum = 99999999;
    string codigo;
    array<string, 7> camposRegistro;

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
    array<string, 7> registroAuto;

    fstream inArchivoAutos(direccionArchivo, ios::in);
    if (!inArchivoAutos)
    {
        cerr << "No se pudo abrir el archivo" << endl;
    }
    else
    {
        cout << "El archivo se abrió correctamente" << endl;
    }

    while (inArchivoAutos >> registroAuto[0] >> registroAuto[1] >> registroAuto[2] >> registroAuto[3] >> registroAuto[4] >> registroAuto[5] >> registroAuto[6])
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
    array<string, 7> datosAuto;
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

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    entradaCorrecta = false;

    do
    {
        cout << "Ingrese la velocidad máxima (km/h) del automóvil " << endl
             << "(distinta de 0 & más de 3 cifras serán omitidas): ";
        cin >> setw(4) >> velocidadMaxima;
        cout << endl;
        if (!(validarSoloNumero(velocidadMaxima)))
        {
            cerr << "El dato que ingresaste no es un número o añadiste otros caracteres" << endl;
            entradaCorrecta = false;
        }
        else if (!(validarNumeroRango(velocidadMaxima, 999, 1)))
        {
            cerr << "El dato que ingresaste está fuera del rango" << endl;
            entradaCorrecta = false;
        }
        else
        {
            entradaCorrecta = true;
        }
    } while (!entradaCorrecta);

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    entradaCorrecta = false;

    do
    {
        cout << "Ingrese los caballos de fuerza(hp) del automóvil" << endl
             << "(más de 3 cifras serán omitidas): ";
        cin >> setw(4) >> caballosFuerza;

        if (!(validarSoloNumero(caballosFuerza)))
        {
            cerr << "El dato que ingresaste no es un número o añadiste otros caracteres" << endl;
            entradaCorrecta = false;
        }
        else if (!(validarNumeroRango(caballosFuerza, 999, 1)))
        {
            cerr << "El dato que ingresaste está fuera del rango" << endl;
            entradaCorrecta = false;
        }
        else
        {
            entradaCorrecta = true;
        }

    } while (!entradaCorrecta);

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

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    entradaCorrecta = false;

    do
    {
        cout << "Ingrese el costo del automóvil" << endl
             << "(Sin coma decimal, más de 8 cifras serán omitidas): ";
        cin >> setw(9) >> costoAuto;
        cout << endl;

        if (!(validarSoloNumero(costoAuto)))
        {
            cerr << "El dato que ingresaste no es un número o añadiste otros caracteres" << endl;
            entradaCorrecta = false;
        }
        else if (!(validarNumeroRango(costoAuto, 99999999, 1)))
        {
            cerr << "El dato que ingresaste está fuera del rango" << endl;
            entradaCorrecta = false;
        }
        else
        {
            entradaCorrecta = true;
        }

    } while (!entradaCorrecta);

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    entradaCorrecta = false;

    do
    {
        cout << "Ingrese el número de identificación de quien registra el automóvil" << endl
             << "(más de 8 cifras serán omitidas): ";
        cin >> setw(9) >> identificacionPropietario;
        cout << endl;

        if (!(validarSoloNumero(identificacionPropietario)))
        {
            cerr << "El dato que ingresaste no es un número o añadiste otros caracteres" << endl;
            entradaCorrecta = false;
        }
        else if (!(validarNumeroRango(identificacionPropietario, 99999999, 1)))
        {
            cerr << "El dato que ingresaste está fuera del rango" << endl;
            entradaCorrecta = false;
        }
        else
        {
            entradaCorrecta = true;
        }

    } while (!entradaCorrecta);

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