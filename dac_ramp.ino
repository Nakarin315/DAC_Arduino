#include <SPI.h>
int t_ramping = 1; // Ramping time in milisecond
int step_t = 50; // The total step in ramping


void setup() {
    SPI.begin();
    SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE1));
    pinMode(10, OUTPUT);
    
    int step_t = 50;
    // Increase the total step to 100 when the t_ramping > 10 milisecond 
    // For t_ramping < 10 milisecond the step_t need to be smaller due to the fact that the time delay in each step will never smaller than 17 microseconds 
    if (t_ramping>10){
      int step_t =100;
    }
}
void update_dac(uint16_t Port_ATMega, uint16_t ch, uint16_t num) {
    // Port_ATMega is a ATMega pin of the Arduino which is used to SYNC, ch is the Channel of the DAC output, num is the DAC number rangibg from 0 to 255 (00 to FF)
    // In this case pin 10 of Arduino is used and this coresponds to PORTB 2 (PB2) on ATMega pin
    PORTB&= ~(1<<Port_ATMega);
    SPI.transfer16((ch << 12) | (num << 4));
    PORTB|= (1<<Port_ATMega);
}
void dac_ramp( uint16_t dac_start, uint16_t dac_end){
  int step_run = 0;
  int dac_run = dac_start; // DAC number which is ranging from dac_start to dac_end
  int tmp;
  // Each DAC update need 17 microseconds 
  int time_step = ((float)t_ramping/(int)step_t)*1000-17;
  if(dac_start > dac_end)
      tmp = -((int)(dac_start-dac_end)/step_t);
  else
      tmp = (dac_end-dac_start)/step_t;
 
  while(step_run<step_t){
    step_run+= 1;
    dac_run += tmp;
    update_dac(2, 2,dac_run);
    delayMicroseconds(time_step);
  }
}

void loop() {
    // Ramp up from 0 to 250
    dac_ramp(250, 0);
    // Ramp down from 250 to 0
    dac_ramp( 0, 250);

}
