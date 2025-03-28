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
bool eliminarAuto(const string, const string);
string generarCodigoAuto();
array<string, 7> getAuto(const string, const string);
string modificarAuto(const array<array<char, 3>, 2> &);
bool setAuto(const array<string, columnasRegistro> &, const string);

// Gestor competencias
const size_t filasCompetencias{7};
const size_t columnasRegistroCompetencia{7};
// bool eliminarCompetencia(const string, const string);
string generarCodigoCompetencia();
array<string, columnasRegistro> getCompetencia(const string, const string);
bool setCompetencia(const array<string, columnasRegistro> &, const string);
string modificarCompetencia(const string, const string[]);

// Validador entradas
bool validarCodigoAuto(const string &);
bool validarCodigoCompetencia(const string);
bool validarFormatoFecha(const string &);
bool validarNombre(string);
bool validarNumeroRango(const string &, const int, const int);
bool validarSoloNumero(const string &);

//? Formateador Salida
string formatearSalidaDatosAuto(string[]);
string formatearSalidaDatosCompetencia(string[]);

//? Formateador Entrada
string formatearRegistro(const array<string, columnasRegistro> &);
// string formatearEntradaDatosCompetencia(string[]);

int main()
{
    string direccionArchivoAutos = "Autos.txt";
    string direccionArchivoCompetencias = "Competencias.txt";

    array<string, 7> registroAuto = getCompetencia("Edreya", direccionArchivoAutos);
    if (!registroAuto[0].empty())
    {
        cout << formatearRegistro(registroAuto) << endl;
    }
    else

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

// Gestor autos
bool eliminarAuto(const string codigo, const string direccionArchivo)
{
    array<string, 7> registroAuto;

    try
    {
        ifstream inArchivoAutos(direccionArchivo, ios::in);
        ofstream outArchivoTemp("temp.txt", ios::out);
        bool autoEncontrado = false;

        cout << "El archivo se abrió correctamente" << endl;
        while (inArchivoAutos >> registroAuto[0] >> registroAuto[1] >> registroAuto[2] >> registroAuto[3] >> registroAuto[4] >> registroAuto[5] >> registroAuto[6])
        {
            if (!(registroAuto[0] == codigo))
            {
                outArchivoTemp << registroAuto[0] << registroAuto[1] << registroAuto[2] << registroAuto[3] << registroAuto[4] << registroAuto[5] << registroAuto[6];
            }
            else
            {
                autoEncontrado = true;
            }
        }

        if (autoEncontrado)
        {
            remove(direccionArchivo.c_str());
            rename("temp.txt", direccionArchivo.c_str());
            cout << "Registro eliminado correctamente" << endl;
        }
        else
        {
            remove("temp.txt");
            cout << "No se encontró el registro del auto " << codigo << endl;
        }
    }
    catch (const exception &e)
    {
        cerr << "No se pudo abrir el archivo" << endl;
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

    string registroAuto = formatearRegistro(infoAuto);

    appArchivoAutos << registroAuto;

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
    cout << "Auto no encontrado!" << endl;
    return {""};
}

// Gestor competencias
bool setCompetencia(const array<string, columnasRegistro> &infoCompetencia, string direccionArchivo)
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
    return {""};
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