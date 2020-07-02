#include <SPI.h>                 // lib for SPI bus
#include <nRF24L01.h>            // setup RF24
#include <RF24.h>                // lib for nRF24L01+

RF24  radio(7, 5);               // (CE=7, CSN=5) create object "radio" for RF24,

void setup(){

   Serial.begin(9600);
 
   radio.begin();                             // create abject "radio"
    delay(2000);                              // give it 2[s] to put things in order, otherwise unplug/plug
    
    //radio.powerDown();
    radio.stopListening();                    // just in case, can ignore      
    radio.setAutoAck(false);                  // No need for replay  (0 = false,  1 = true)
    radio.setPayloadSize(32);                 // default is 32, it does affect the N bytes sent, but not the speed ...   
    radio.setDataRate(RF24_1MBPS);            // speed (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS). Note RF24_250KBPS only for NRF24 plus (nRF24L01+) units 
    radio.setPALevel(RF24_PA_HIGH);           // Power of the Power Amplifier (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    //radio.setCRCLength(RF24_CRC_16);        // packets CRC: 8-bit or 16-bit
    radio.openWritingPipe(123);               // open pipe to create packets ID "123" (on one channel can have 1 receiver and 6 transmitters)
    radio.powerUp();                          // powerUp включение или пониженное потребление powerDown                                                        
}


#define N 40            // number of channels

//byte D[32];            
//int D[16];            // 2 bytes elements <- 32/2 = 16
long D[8];              // 4 bytes elements <- 32/4 = 8

long B[N];
unsigned long n = 0;

void loop(){

    unsigned long T = micros();       
    for (byte j = 0; j < N; j++)            // jump through channels 2,4,..,122
    {
            //D[1] = micros() - T;
            D[1] = n;
            n+=1;
            D[0] = j;                       // packet number            
            //B[j] = D[0]; 
            radio.setChannel(j);
            radio.write(&D, 32);            // send 32 bytes of data
            delayMicroseconds(393);         // single packet takes 605.53, with "delayMicroseconds(393)" it takes 1[ms]

    }

/*      
    T = micros() - T;                         // now length is 1191 [us]
    float t = 1.0*T/N;
    //Serial.println(T);
    Serial.println(t);

    
     for (int i = 0; i < N; i++) {
         Serial.print(B[i]);
         Serial.print(" ");
     }
     Serial.print("\n");
*/


}
