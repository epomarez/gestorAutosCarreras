#include <array>
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <regex>

using namespace std;

// Gestor autos
const size_t registrosAutos{25};
const size_t columnasRegistro{7};
bool eliminarAuto(const string);
string generarCodigoAuto();
array<string, 7> getAuto(const string, const string);
string modificarAuto(const array<array<char, 3>, 2> &);
bool setAuto(const array<string, columnasRegistro> &, const string);

// Gestor competencias
const size_t filasCompetencias{7};
const size_t columnasCompetencias{25};
bool eliminarCompetencia(const string);
string generarCodigoCompetencia();
bool insertarCompetencia(const array<string, columnasRegistro> &, const string);
string modificarCompetencia(const string, const string[]);
string obtenerCompetencia(const string);

// Validador entradas
bool validarCodigoAuto(const string &);
bool validarCodigoCompetencia(const string);
bool validarFormatoFecha(const string &);
bool validarNombre(string);
bool validarNumeroRango(const string &, const int, const int);
bool validarSoloNumero(const string &);

// Formateador Salida?
string formatearSalidaDatosAuto(string[]);
string formatearSalidaDatosCompetencia(string[]);

// Formateador Entrada?
string formatearRegistro(const array<string, columnasRegistro> &);
// string formatearEntradaDatosCompetencia(string[]);

int main()
{
    string direccionArchivoAutos = "Autos.txt";
    string direccionArchivoCompetencias = "Competencias.txt";

    ofstream autosArchivo("maravillas.txt", ios::out);
    if (!autosArchivo)
    {
        cout << "Hubo un error";
    }
    else
    {
        cout << "Todo nice, ah!";
    }

    array<string, 7> registroAuto = getAuto("Edrey", direccionArchivoAutos);
    if (registroAuto[0].empty())
    {
        cout << "Auto no encontrado" << endl;
    }
    else
    {
        cout << "Auto encontrado!" << endl;
        for (int i = 0; i < columnasRegistro; i++)
        {
            cout << registroAuto[i];
        }
    }
    system("pause");
}

// Validador entradas definición
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

    string registroAuto = formatearRegistro(infoAuto);

    appArchivoAutos << registroAuto;

    return true;
}

array<string, 7> getAuto(const string codigo, const string direccionArchivo)
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
        cout << registroAuto[1];
        cout << "Hola" << endl;
        if (registroAuto[0] == codigo)
        {
            return registroAuto;
        }
    }

    return {""};
}

bool insertarCompetencia(const array<string, columnasRegistro> &infoCompetencia, string direccionArchivo)
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

    string registroCompetencia = formatearRegistro(infoCompetencia);

    appArchivoAutos << registroCompetencia;

    return true;
}

// Formateador Entrada?
string formatearRegistro(const array<string, columnasRegistro> &infoAuto)
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