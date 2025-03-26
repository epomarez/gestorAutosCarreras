#include <iostream>

using namespace std;

// Gestor autos
bool insertarAuto(int[], string);
string obtenerAuto(string);
bool eliminarAuto(string);
string modificarAuto(string, string[]);
string generarCodigoAuto();

// Gestor competencias
bool insertarCompetencia(int[], string);
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
string formatearDatosAuto(string[]);
string formatearDatosCompetencia(string[]);

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
