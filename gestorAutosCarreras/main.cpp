#include <iostream>
#include <array>

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
bool validarNombre(string);
bool validarSoloNumero(int);
bool validarCodigoAuto(string);
bool validarCodigoCompetencia(string);
bool validarFormatoFecha(string);
bool validarNumeroRango(string, int, int);

// Formateador Salida?
string formatearSalidaDatosAuto(string[]);
string formatearSalidaDatosCompetencia(string[]);

// Formateador Entrada?
string formatearEntradaDatosAuto(string[]);
string formatearEntradaDatosCompetencia(string[]);

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}