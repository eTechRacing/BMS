#include <xc.h>

int max_cell_voltage(int voltage[], int ns){
    int max_cell_voltage = 0;
    int i, max_cell_voltage_number;
    for( i = 0; i < ns; i++){
        if( voltage[i] > max_cell_voltage ){
            max_cell_voltage = voltage[i];
            max_cell_voltage_number = i+1;
        }
    }
    return max_cell_voltage;
}

int min_cell_voltage(int voltage[], int ns){
    int min_cell_voltage = 5000;
    int i, min_cell_voltage_number;
    for( i = 0; i < ns; i++){
       if( voltage[i] < min_cell_voltage ){
            min_cell_voltage = voltage[i];
            min_cell_voltage_number = i+1;
        }
    }
    return min_cell_voltage;
}

long total_voltage(int voltage[], int ns){
    long total_voltage = 0;
    int i;
    for( i = 0; i < ns; i++)
            {
                total_voltage = total_voltage + voltage[i];
    }
    return total_voltage;
}

int SoC_from_OCV(int OCV){
    static const int OCVs[] = { 2925,2985,3062,3121,3173,3219,3259,3296,3329,
    3359,3387,3412,3436,3458,3479,3498,3516,3533,3549,3563,3575,3583,3586,3589,
    3591,3593,3596,3598,3601,3603,3606,3609,3612,3616,3619,3623,3627,3632,3637,
    3642,3648,3654,3660,3666,3672,3678,3684,3690,3696,3702,3709,3716,3722,3728,
    3734,3740,3746,3753,3760,3767,3774,3781,3787,3792,3796,3801,3805,3810,3814,
    3818,3822,3826,3829,3833,3836,3840,3843,3847,3850,3853,3856,3859,3862,3865,
    3868,3870,3873,3876,3878,3881,3883,3886,3888,3890,3893,3895,3898,3900,3902,
    3905,3907,3909,3911,3913,3916,3918,3920,3922,3924,3926,3928,3930,3933,3935,
    3937,3939,3941,3944,3946,3948,3951,3953,3955,3958,3960,3963,3966,3969,3971,
    3974,3978,3981,3985,3989,3993,3997,4002,4006,4011,4015,4020,4024,4029,4033,
    4036,4040,4044,4047,4050,4053,4056,4059,4061,4064,4066,4068,4071,4073,4075,
    4077,4079,4080,4082,4084,4085,4087,4089,4090,4092,4093,4095,4096,4098,4100,
    4102,4103,4105,4107,4109,4111,4113,4115,4117,4120,4122,4125,4128,4131,4134,
    4137,4141,4144,4148,4153,4158,4163,4168,4174,4181,4188,4199 };
    // OCV en función del SoC [mV]

    static const int SoCs[] = { 0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,
    80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,
    180,185,190,195,200,205,210,215,220,225,230,235,240,245,250,255,260,265,270,
    275,280,285,290,295,300,305,310,315,320,325,330,335,340,345,350,355,360,365,
    370,375,380,385,390,395,400,405,410,415,420,425,430,435,440,445,450,455,460,
    465,470,475,480,485,490,495,500,505,510,515,520,525,530,535,540,545,550,555,
    560,565,570,575,580,585,590,595,600,605,610,615,620,625,630,635,640,645,650,
    655,660,665,670,675,680,685,690,695,700,705,710,715,720,725,730,735,740,745,
    750,755,760,765,770,775,780,785,790,795,800,805,810,815,820,825,830,835,840,
    845,850,855,860,865,870,875,880,885,890,895,900,905,910,915,920,925,930,935,
    940,945,950,955,960,965,970,975,980,985,990,995,1000 };
    // SoC para cada valor de OCV [ad]
   
    static const int count = sizeof(OCVs)/sizeof(OCVs[0]);
    
    int i;
    int dOCV, dSoC;
    
    if (OCV < OCVs[0]) {
        return SoCs[0];
    }
    
    if (OCV > OCVs[count-1]) {
        return SoCs[count-1];
    }
    
    for (i = 0; i < count-1; i++) {
        if (OCVs[i+1] > OCV) {
            break;
        }
    }
    
    dOCV = OCVs[i+1] - OCVs[i];
    dSoC = SoCs[i+1] - SoCs[i];
    return SoCs[i] + (OCV - OCVs[i]) * dSoC / dOCV;
}

int max_cell_temperature(int temperature[120]){
    
    int i;
    int max_cell_temperature = 0;
    int temperature_type = 0;
    
    for( i=0; i<120; i++){
        if( i != (4*temperature_type)){
            if( temperature[i] > max_cell_temperature ){
                max_cell_temperature = temperature[i];
            }
        }
    }
    return max_cell_temperature;
}

int min_cell_temperature(int temperature[120]){
    
    int i;
    int min_cell_temperature = 0;
    int temperature_type = 0;
    
    for( i=0; i<120; i++){
        if( i != (4*temperature_type)){
            if( temperature[i] < min_cell_temperature ){
                min_cell_temperature = temperature[i];
            }
        }
    }
    return min_cell_temperature;
}

int avg_cell_temperature(int temperature[120]){
    int i;
    int avg_cell_temperature = 0;
    int temperature_type = 0;
    for( i=0; i<120; i++){
        if( i != (4*temperature_type)){
            avg_cell_temperature = avg_cell_temperature + temperature[i];
        }
    }
    avg_cell_temperature = avg_cell_temperature/90;
    return avg_cell_temperature;
}

int max_onboard_temperature(int temperature[120]){
    
    int i;
    int max_onboard_temperature = 0;
    int temperature_type = 0;
    
    for( i=0; i<120; i++){
        if( i == (4*temperature_type)){
            if( temperature[i] > max_onboard_temperature ){
                max_onboard_temperature = temperature[i];
            }
        }
    }
    return max_onboard_temperature;
}

int min_onboard_temperature(int temperature[120]){
    
    int i;
    int min_onboard_temperature = 0;
    int temperature_type = 0;
    
    for( i=0; i<120; i++){
        if( i == (4*temperature_type)){
            if( temperature[i] < min_onboard_temperature ){
                min_onboard_temperature = temperature[i];
            }
        }
    }
    return min_onboard_temperature;
}

int avg_onboard_temperature(int temperature[120]){
    int i;
    int avg_onboard_temperature = 0;
    int temperature_type = 0;
    for( i=0; i<120; i++){
        if( i == (4*temperature_type)){
            avg_onboard_temperature = avg_onboard_temperature + temperature[i];
        }
    }
    avg_onboard_temperature = avg_onboard_temperature/90;
    return avg_onboard_temperature;
}
