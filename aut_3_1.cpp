/*
  Descripcion: Implementacion en C++ del "Sistema Integral de Monitoreo de Produccion Industrial"
  - 4 arreglos unidimensionales (produccion, horasOperacion, consumoEnergia, temperaturaMaquinas)
  - Uso de funciones para cada entrada y analisis
  - Menu interactivo
  - Validacion de entradas
*/

#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include <vector>

using namespace std;

// Lectura segura de un entero con rango
int leerEnteroValido(const string &mensaje, int minValor = std::numeric_limits<int>::min(),
                     int maxValor = std::numeric_limits<int>::max()) {
    int x;
    while (true) {
        cout << mensaje;
        if (cin >> x) {
            if (x < minValor || x > maxValor) {
                cout << "Valor fuera de rango (" << minValor << " - " << maxValor << "). Intente nuevamente.\n";
                continue;
            }
            return x;
        }
        // Si falla la lectura
        cout << "Entrada invalida. Ingrese un numero entero.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Lectura segura de un float con rango
float leerFloatValido(const string &mensaje, float minValor = -std::numeric_limits<float>::infinity(),
                      float maxValor = std::numeric_limits<float>::infinity()) {
    float x;
    while (true) {
        cout << mensaje;
        if (cin >> x) {
            if (x < minValor || x > maxValor) {
                cout << "Valor fuera de rango. Intente nuevamente.\n";
                continue;
            }
            return x;
        }
        cout << "Entrada invalida. Ingrese un numero.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Ingresar datos: produccion (enteros)
void ingresarProduccion(vector<int> &produccion) {
    cout << "\n--- Ingresar produccion diaria (unidades) ---\n";
    for (size_t i = 0; i < produccion.size(); ++i) {
        produccion[i] = leerEnteroValido("Dia " + to_string(i + 1) + " - Unidades producidas: ", 0);
    }
}

// Ingresar datos: horas (float)
void ingresarHoras(vector<float> &horas) {
    cout << "\n--- Ingresar horas de operacion por dia ---\n";
    for (size_t i = 0; i < horas.size(); ++i) {
        horas[i] = leerFloatValido("Dia " + to_string(i + 1) + " - Horas trabajadas: ", 0.0f, 24.0f);
    }
}

// Ingresar datos: consumo (float)
void ingresarConsumo(vector<float> &consumo) {
    cout << "\n--- Ingresar consumo energetico (kWh) por dia ---\n";
    for (size_t i = 0; i < consumo.size(); ++i) {
        consumo[i] = leerFloatValido("Dia " + to_string(i + 1) + " - kWh consumidos: ", 0.0f);
    }
}

// Ingresar datos: temperatura (float)
void ingresarTemperatura(vector<float> &temp) {
    cout << "\n--- Ingresar temperatura de maquinas (C) por dia ---\n";
    for (size_t i = 0; i < temp.size(); ++i) {
        temp[i] = leerFloatValido("Dia " + to_string(i + 1) + " - Temperatura (C): ", -50.0f, 500.0f);
    }
}

// Analisis produccion
void analizarProduccion(const vector<int> &produccion) {
    size_t n = produccion.size();
    if (n == 0) { cout << "No hay datos de produccion.\n"; return; }

    long long total = 0;
    size_t diaMax = 0, diaMin = 0;
    for (size_t i = 0; i < n; ++i) {
        total += produccion[i];
        if (produccion[i] > produccion[diaMax]) diaMax = i;
        if (produccion[i] < produccion[diaMin]) diaMin = i;
    }
    double promedio = static_cast<double>(total) / static_cast<double>(n);

    cout << "\n--- Analisis: Produccion diaria ---\n";
    cout << "Total producido en " << n << " dias: " << total << " unidades\n";
    cout << "Dia de mayor produccion: Dia " << (diaMax + 1) << " con " << produccion[diaMax] << " unidades\n";
    cout << "Dia de menor produccion: Dia " << (diaMin + 1) << " con " << produccion[diaMin] << " unidades\n";
    cout << "Promedio de produccion por dia: " << promedio << " unidades\n";
}

// Analisis horas
void analizarHoras(const vector<float> &horas) {
    size_t n = horas.size();
    if (n == 0) { cout << "No hay datos de horas.\n"; return; }

    double total = 0.0;
    size_t diaMin = 0;
    for (size_t i = 0; i < n; ++i) {
        total += horas[i];
        if (horas[i] < horas[diaMin]) diaMin = i;
    }
    double promedio = total / static_cast<double>(n);

    cout << "\n--- Analisis: Horas de operacion ---\n";
    cout << "Promedio de horas trabajadas: " << promedio << " hrs/dia\n";

    cout << "Umbral por defecto de sobrecarga: 8.0 hrs.\n";
    float umbral = leerFloatValido("Ingrese umbral X para considerar sobrecarga (hrs) o 0 para usar 8.0: ", 0.0f, 24.0f);
    if (umbral == 0.0f) umbral = 8.0f;

    cout << "Dias con sobrecarga (horas > " << umbral << "): ";
    bool alguno = false;
    for (size_t i = 0; i < n; ++i) {
        if (horas[i] > umbral) {
            cout << (i + 1) << " ";
            alguno = true;
        }
    }
    if (!alguno) cout << "Ninguno";
    cout << "\n";

    cout << "Dia con menos horas trabajadas: Dia " << (diaMin + 1) << " con " << horas[diaMin] << " hrs\n";
}

// Analisis consumo
void analizarConsumo(const vector<float> &consumo) {
    size_t n = consumo.size();
    if (n == 0) { cout << "No hay datos de consumo.\n"; return; }

    double total = 0.0;
    for (size_t i = 0; i < n; ++i) total += consumo[i];
    double promedio = total / static_cast<double>(n);

    // Buscar dia con consumo mas cercano al promedio
    size_t diaCercano = 0;
    double minDiff = fabs(consumo[0] - promedio);
    for (size_t i = 1; i < n; ++i) {
        double diff = fabs(consumo[i] - promedio);
        if (diff < minDiff) {
            minDiff = diff;
            diaCercano = i;
        }
    }

    cout << "\n--- Analisis: Consumo energetico ---\n";
    cout << "Consumo total: " << total << " kWh\n";
    cout << "Consumo promedio: " << promedio << " kWh/dia\n";
    cout << "Dia con consumo mas cercano al promedio: Dia " << (diaCercano + 1) << " (" << consumo[diaCercano] << " kWh)\n";
}

// Analisis temperatura
void analizarTemperatura(const vector<float> &temp) {
    size_t n = temp.size();
    if (n == 0) { cout << "No hay datos de temperatura.\n"; return; }

    double total = 0.0;
    size_t diaMax = 0;
    for (size_t i = 0; i < n; ++i) {
        total += temp[i];
        if (temp[i] > temp[diaMax]) diaMax = i;
    }
    double promedio = total / static_cast<double>(n);

    cout << "\n--- Analisis: Temperatura de maquinas ---\n";
    cout << "Temperatura promedio: " << promedio << " C\n";
    cout << "Maxima temperatura registrada: " << temp[diaMax] << " C (Dia " << (diaMax + 1) << ")\n";

    float limite = leerFloatValido("Ingrese temperatura limite segura (C) - valores por encima se consideran sobrecalentamiento: ", -50.0f, 500.0f);
    cout << "Dias con sobrecalentamiento (> " << limite << " C): ";
    bool alguno = false;
    for (size_t i = 0; i < n; ++i) {
        if (temp[i] > limite) {
            cout << (i + 1) << " ";
            alguno = true;
        }
    }
    if (!alguno) cout << "Ninguno";
    cout << "\n";
}

// Mostrar datos
void mostrarDatos(const vector<int> &produccion, const vector<float> &horas,
                  const vector<float> &consumo, const vector<float> &temp) {
    size_t n = produccion.size();
    cout << "\n--- Datos actuales (por dia) ---\n";
    cout << "Dia\tProduccion\tHoras\tConsumo(kWh)\tTemp(C)\n";
    for (size_t i = 0; i < n; ++i) {
        cout << (i + 1) << "\t" << produccion[i] << "\t\t" << horas[i] << "\t" << consumo[i] << "\t\t" << temp[i] << "\n";
    }
}

int main() {
    cout << "Sistema Integral de Monitoreo de Produccion Industrial\n";
    cout << "(Actividad Unidad 3 - Vectores)\n";

    int n = leerEnteroValido("Ingrese la cantidad de dias a registrar (por ejemplo 7): ", 1, 1000);

    vector<int> produccion(n, 0);
    vector<float> horasOperacion(n, 0.0f);
    vector<float> consumoEnergia(n, 0.0f);
    vector<float> temperatura(n, 0.0f);

    int opcion;
    do {
        cout << "\n--- Menu principal ---\n";
        cout << "1) Ingresar produccion diaria\n";
        cout << "2) Ingresar horas de operacion\n";
        cout << "3) Ingresar consumo energetico\n";
        cout << "4) Ingresar temperaturas (arreglo personalizado)\n";
        cout << "5) Analizar produccion diaria\n";
        cout << "6) Analizar horas de operacion\n";
        cout << "7) Analizar consumo energetico\n";
        cout << "8) Analizar temperaturas (personalizado)\n";
        cout << "9) Mostrar datos ingresados\n";
        cout << "0) Salir\n";

        opcion = leerEnteroValido("Seleccione una opcion: ", 0, 9);

        switch (opcion) {
            case 1: ingresarProduccion(produccion); break;
            case 2: ingresarHoras(horasOperacion); break;
            case 3: ingresarConsumo(consumoEnergia); break;
            case 4: ingresarTemperatura(temperatura); break;
            case 5: analizarProduccion(produccion); break;
            case 6: analizarHoras(horasOperacion); break;
            case 7: analizarConsumo(consumoEnergia); break;
            case 8: analizarTemperatura(temperatura); break;
            case 9: mostrarDatos(produccion, horasOperacion, consumoEnergia, temperatura); break;
            case 0: cout << "Saliendo... recuerde guardar su archivo .cpp en la nube.\n"; break;
            default: cout << "Opcion no valida.\n"; break;
        }

    } while (opcion != 0);

    return 0;
}
