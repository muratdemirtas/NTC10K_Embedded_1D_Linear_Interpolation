/**
 * NTCTempSensor.c
 * NTC with 2d lookup table Interpolation source code
 * @author muratdemirtastr@gmail.com
 */
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "lookup_1d.h"
#include "ntc_10k.h"

#define NTC_10K_INTERP_VECTOR_SIZE 19
static void Device_NTC_ADC_Read(uint16_t);

Table1d tablew;

/* interpolation table of NTC sensor*/
static ts_ntc_interp1 celcius_resistance[NTC_10K_INTERP_VECTOR_SIZE]   = {
{110,   0.7576},
{100,   0.9731},
{90,    1.266},
{85,    1.451},
{80,    1.668 },
{70,    2.228 },
{60,    3.020 },
{50,    4.160 },
{40,    5.827 },
{30,    8.31},
{25,    10},
{20,    12.09},
{10,    17.96},
{0,     27.28},
{-10,   42.47},
{-20,   67.77},
{-30,   111.3},
{-40,   188.5},
{-50,   329.5}
};
void Interpolation_Table_Init() {
	tablew.ncols = NTC_10K_INTERP_VECTOR_SIZE;
	for(int i = 0; i < NTC_10K_INTERP_VECTOR_SIZE; i++){
		tablew.table[i] = celcius_resistance[i].celcius;
		tablew.columns[i] = celcius_resistance[i].resistance;
	}
}
static double NTC_GET_Voltage(uint32_t analogValue){
	return analogValue* (NTC_SUPPLY_VOLTAGE/NTC_ADC_MAX_RESOLUTION);
}
static double NTC_GET_Temperature(uint32_t analogValue)
{
	double out_value[2] = {0,0};
	/* get voltage out value*/
	double Vout = NTC_GET_Voltage(analogValue);
	/* get rtc value*/
	double R_NTC = NTC_SUPPLY_VOLTAGE*NTC_VOLTAGE_DIVIDER_RES - NTC_VOLTAGE_DIVIDER_RES*Vout;
	/* get kohm value*/
	if(Vout == 0)
		return -999;
	R_NTC = R_NTC/(Vout*1000);
	if(lookup1d(&tablew, R_NTC ,&out_value[0]))
		return -999;
	else
		return out_value[0];
}
void Device_NTC_ADC_Read(uint16_t value){
	double ntc_val = NTC_GET_Temperature(value);
	if(ntc_val != -999)
		printf("Temp: %.2f C, adc_val: %d\n", ntc_val , value);
}
int main(){
	Interpolation_Table_Init();

	for(int i = 0; i < 4096 ;i++){
		Device_NTC_ADC_Read(i);
	}
	for(int i = 4096; i > 0 ; i--){
		Device_NTC_ADC_Read(i);
	}
	return 0;
}
