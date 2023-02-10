#ifndef _CONSTANTS_
#define _CONSTANTS_

// Constants definition

const int max_OK_voltage = 4195, min_OK_voltage = 3000;
// Maximum and minimum voltages that trigger Overvoltage or Undervoltage, respectively [mV]

const int num_series_cells = 140, num_parallel_cells = 2;
// Number of cells in series [ad]
// Number of cells in parallel [ad]

const int cell_capacity = 2500;
// Cell capacity [mAh]

const int max_OK_temperature = 600, min_OK_temperature = 5;
// Temperatures in [°C * 10]

const long max_OK_current = 190000;
// Maximum current allowed based on the maximum rules power (80 kW / 420 V ~= 190 A), [mA]

const int max_charging_voltage = 5880, divide_current_voltage = 4195, max_charging_current = 25;
// Maximum voltage that the Accumulator can be charged to [V * 10]
// Voltage at which the current delivered to the cell is divided by the balancing circuit [mV]
// Maximum allowed charging current [A]

const int fan_temperature = 450;
// Temperature at which fans turn on [°C * 10]

const int balancing_voltage = 4950, max_balancing_temperature = 650;
// Accumulator voltage at which balancing starts [mV]
// Maximum temperature of the resistors allowed for balancing [°C * 10]

const short max_difference = 20;
// Maximum voltage difference at which balancing starts [mV]

// FreeRTOS Constants
#define STACK_SIZE_MIN 200
// Minimum size of the task stack in words

#define PRIORITY_LEVELS 5
// Number of priority levels available in FreeRTOS

#define TICK_RATE_MS 25
// Tick rate of the FreeRTOS timer in milliseconds

#endif
