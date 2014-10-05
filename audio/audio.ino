/*
fht_adc_serial.pde
guest openmusiclabs.com 7.7.14
example sketch for testing the fht library.
it takes in data on ADC0 (Analog0) and processes them
with the fht. the data is sent out over the serial
port at 115.2kb.
*/

#define LOG_OUT 0 // use the log output function
#define FHT_N 256 // set to 256 point fht
#define OCTAVE 0
#define LIN_OUT 1

#include <FHT.h> // include the library
#include <OSSex.h>

void setup() {
  Serial.begin(115200); // use the serial port
  // TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = B11100110; //enable adc, start conversion, auto trigger enable
                      // interrupt flag off, interrupt off, no prescaler
  ADMUX = B00000100; // use adc4  //0x40; // use adc0
  
  DIDR0 = B00010000; // turn off adc4 digital-in; 0x01; // turn off the digital input for adc0
  ADCSRB = B00000000; // free running mode
  Toy.setID(0);
}

int beat[][3] = {
  {0, 200, 25},
};

int oldVal=1;
int newVal=0;
int smallest = 32000;
int biggest = 0;
int timeDiff;
int newTime=0;
int oldTime=0;
void loop() {
  while(1) { // reduces jitter
    //cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = B11110000; //0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fht_input[i] = k; // put real data into bins
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_lin(); // take the output of the fht
    //sei();
    //Serial.println("start");
    //for (byte i = 0 ; i < FHT_N/2 ; i++) {
      newVal = fht_lin_out[0];
      if (newVal < smallest) {
        smallest = newVal;
      }
      oldVal = newVal;
      int diff = fht_lin_out[0] - smallest;
      if (diff > 0) {
        smallest++;
      } else if (diff < 0) {
        smallest--;
      }
      if (diff > biggest) {
        biggest = diff;
      }
      int val = map(diff, 0, biggest, 0, 255);
      if (val > 150) {
       // Toy.setOutput(0, val);
      } else {
        Toy.setOutput(0, 0);
      }
      Serial.print(diff);
      Serial.print('|');
      if (val > 127) {
        newTime = millis();
        if (newTime - oldTime < 50) {
          timeDiff += newTime - oldTime;
        } else {
          timeDiff = newTime - oldTime;
        }
        Serial.print(timeDiff);
          oldTime = newTime;
      } else {
        Serial.print(0);
      }

      Serial.println('|');
      /* Serial.print(fht_lin_out[3]);
      Serial.print('|');
      Serial.print(fht_lin_out[4]);
      Serial.print('|');
      Serial.print(fht_lin_out[5]);
      Serial.print('|');
      Serial.print(fht_lin_out[6]);
      Serial.print('|');
      Serial.println(fht_lin_out[7]);*/
      

    //}
    /*newVal = fht_lin_out[1];
    if (newVal < oldVal) {
     //Toy.runShortPattern(*beat, sizeof(beat) / sizeof(int) / 3);
    } else if (newVal == oldVal) {
    } else {
      //Toy.setOutput(0,0);
      
    }
    oldVal = newVal;  */
  }
}
