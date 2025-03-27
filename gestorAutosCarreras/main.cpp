#include <iostream>
#include <array>
#include <cctype>
#include <regex>

using namespace std;

// Gestor autos
const size_t filasAutos{7};
const size_t columnasAutos{25};
bool insertarAuto(const array<array<char, columnasAutos>, filasAutos> &);
string obtenerAuto(string);
bool eliminarAuto(string);
string modificarAuto(const array<array<char, 3>, 2> &);
string generarCodigoAuto();

// Gestor competencias
const size_t filasCompetencias{7};
const size_t columnasCompetencias{25};
bool insertarCompetencia(const array<array<char, columnasCompetencias>, filasCompetencias> &);
string obtenerCompetencia(string);
bool eliminarCompetencia(string);
string modificarCompetencia(string, string[]);
string generarCodigoCompetencia();

// Validador entradas
bool validarCodigoAuto(const string &);
bool validarCodigoCompetencia(string);
bool validarFormatoFecha(const string &);
bool validarNombre(string);
bool validarNumeroRango(const string &, int, int);
bool validarSoloNumero(const string &);

// Formateador Salida?
string formatearSalidaDatosAuto(string[]);
string formatearSalidaDatosCompetencia(string[]);

// Formateador Entrada?
string formatearEntradaDatosAuto(string[]);
string formatearEntradaDatosCompetencia(string[]);

int main()
{
    cout << "Hello world!" << endl;
    bool resultado = validarNumeroRango("9", 20, 10);
    cout << "holaaaaaaaaaaaaa" << resultado;
    return 0;
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

bool validarNombre(string nombre)
{
    if (nombre.length() > 25)
    {
        return false;
    }
    return true;
}

bool validarNumeroRango(const string &numero, int max, int min)
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
