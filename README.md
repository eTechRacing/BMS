isoSPI Battery Management System using an LTC6811-1
The system consists of 15 slave boards and a master board
Each slave board is galvanicly isolated from the rest and from the master board using an isolated 5v-5v DC-DC for the supply and a transformer for the communication.
Each slave board can monitor up to 12 voltages and 8 temperatures.
The boards are connected in a daisy chain, so the master is only connected to the first board, wich transfers the messages to the second and succesively.
The monitoring ICs are not programable so all the settings must be done by isoSPI commands
The voltages are acquired directly by the C pins of the IC while the temperatures are multiplexed and read by a GPIO pin of the IC.
The master board has an e-Tech Racing Shield board wich contains and STM32 microcontroller, two can transcievers, a DCDC and an LDO wich provide 24,5 and 3.3 V.
