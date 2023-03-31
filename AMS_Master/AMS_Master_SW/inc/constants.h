#ifndef _CONSTANTS_
#define _CONSTANTS_

// ***************************************************************************** //
//                          Definición de constantes                             //
// ***************************************************************************** //

int max_OK_voltage = 4195, min_OK_voltage = 3000;
// Voltajes máximos y mínimos que activan Overvoltage o Undervoltage
// respectivamente [mV]

int ns = 140, np = 2;
// Número de celdas en serie [ad]
// Número de celdas en paralelo [ad]

int max_OK_temperature = 600, min_OK_temperature = 5;
// Temperaturas en [ºC * 10]

long max_OK_current = 190000;
// Máxima corriente permitida a partir de la potencia máxima normativa 
// (80 kW / 420 V ~= 190 A),  [mA]

long max_discharge_current = 165000, max_regen_current = -100000;
// Máxima corriente de descarga y m�xima corriente de recarga con frenada regenerativa [mA]

int limitation_temperature = 500;
// Temperatura a partir de la cual se empieza a limitar corriente [�C * 10]
int initial_limitation_factor = 30;
// Factor de reducci�n inicial de corriente [%]


int max_charging_voltage = 5880, divide_current_voltage = 4195, max_charging_current = 25;
// Máximo voltaje al que se puede cargar el Accumulator [V * 10]
// Voltaje a partir del cual se divide la corriente que se entrega a la celda con el circuito de balanceo
// Corriente máxima permitida para la carga

int fans_temperature = 350;
// Temperatura a partir de la cual se encienden los ventiladores [ºC * 10]

int voltage_balancing = 4950, max_balancing_temperature = 650;
// Voltaje del Accumulator a partir del cual se empìeza a balancear [V * 10]
// Temperatura máxima de las resistencias a la que se permite el balanceo [ºC * 10]

short max_difference = 20;
// Diferencia de voltaje máxima permitida a partir de la cual se empieza a balancear [mV]

#endif