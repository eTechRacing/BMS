#include <xc.h>
//int current_control()

int fans_control(int fans_temperature, int max_cell_temperature, int Fans1_Control, int Fans2_Control){
	if ( max_cell_temperature > fans_temperature ){
		Fans1_Control = 1;
		Fans2_Control = 1;
		return 1;
	}
	else if ( max_cell_temperature > fans_temperature ){
		Fans1_Control = 0;
		Fans2_Control = 0;
		return 0;	
	}
    return 0;
}

//int pwm_fans_control(int current_limitation, int max_cell_temperature, int Fans1_Control, int Fans2_Control, int Fans3_Control, int Fans4_Control, int Fans5_Control)
//{

//}
int overvoltage_check(int max_cell_voltage, int max_OK_voltage){
	if( max_cell_voltage > max_OK_voltage ) {
        //shutdown_signal = 1;
        return 0;
    }
    else {
    	return 1;
    }
}