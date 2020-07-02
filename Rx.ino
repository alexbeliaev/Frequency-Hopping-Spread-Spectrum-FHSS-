#include "U8glib.h"              // lib for OLED
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);      // Tested     I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Tested     Display which does not send AC


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


RF24 radio(7,5);      // CE 7, CSN 5


void setup(void) {
    delay(1000);                              //give it 2[s] to put things in order, otherwise unplug/plug
    Serial.begin(9600);
  
    radio.begin();                            // initialize nRF24L01+
    //radio.stopListening();                  // reviver OFF
    radio.setAutoAck(false);                  // No need for replay  (0 = false,  1 = true)
    radio.setChannel(0);    
    radio.setChannel(4);                      // set RF channel (0 .. 127)
    radio.setDataRate(RF24_1MBPS);            // speed (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS). Note RF24_250KBPS only for NRF24 plus (nRF24L01+) units 
    radio.setPALevel(RF24_PA_HIGH);           // power of the Power Amplifier (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm) 
    //radio.setCRCLength(RF24_CRC_16);        // packets CRC: 8-bit or 16-bit                                               
    radio.openReadingPipe(1,123);             // open pipe to create packets ID "123" (on one channel can have 1 receiver and 6 transmitters)
    radio.startListening();                   // receiver ON
    
}





#define N 40            // number of channels

//byte D[32];
long D[8];            // 4 bytes elements <- 32/4 = 8

            
long B[N];
long C[N];
byte j = 0;
unsigned long Ti = micros();

void loop(void) {


    
    radio.setChannel(j);

    
    if(radio.available()){
        Ti = micros();
        radio.read(&D, 32);              
        B[j] = D[0];           // what channel 
        C[j] = D[1];           // packet number
        if (j<N) {j=j+1;}
    }
  
   

    //C[j] = micros() - Ti;
    
    if( j>0 and (micros() - Ti) > 1500 ){
      Ti = micros();
      j=0;                        // j = j+1
    }




 
    if (j>N-1){         //j>127
       j = 0;

  
      //============= OLED ===================
      float Err = 100.0*(C[N-1] - C[0] + 1  - N)/N;

      u8g.firstPage();    // picture loop
      do {
          draw(C[0],Err);
          } while( u8g.nextPage());
       

      //============= Debug =================== 
       Serial.println(Err);
       
       for(int i = 0; i < N; i++){
         Serial.print(B[i]);
         //B[i] = 0;
         Serial.print(" ");
       }
       Serial.print("\n");


        for(int i = 0; i < N; i++){
         Serial.print(C[i]);
         //C[i] = 0;
         Serial.print(" ");
       }
       Serial.print("\n========\n");

                 
    }
}




//============= OLED ===================
void draw(unsigned long n, float Err) {

    // Print value
    u8g.setFont(u8g_font_unifont);
    
    u8g.drawStr(5, 12, "Lost");
    u8g.setPrintPos(55,12);
    u8g.print(Err);
    u8g.drawStr(100, 12, "%");

    u8g.drawStr(5, 30, "Total");
    u8g.setPrintPos(55,30);
    u8g.print(n);
}
