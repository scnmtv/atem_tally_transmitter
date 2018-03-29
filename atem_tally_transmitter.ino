/*
  custom nrf24L01 atem tally transmitter module with arduino-mega

  29.3.2018 - Ferbi

*/


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins CE & CSN
RF24 radio(49, 53);

// Topology
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio pipe addresses for the 2 nodes to communicate.

// start transmitting
bool stat = true;

void setup() {

  Serial.begin(115200);
  printf_begin();
  Serial.println(F("\n\rradio details: TRANSMITTER\n\r"));

  // Setup and configure rf radio
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);         // MIN, LOW, HIGH, MAX
  radio.setAutoAck(0);                    // Ensure autoACK is enabled
  //  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(pipes[0]);        // Both radios listen on the same pipes by default, and switch when writing
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void loop(void) {

  sendData(B00000110);

  // Try again later
  delay(50);
}

void sendData(int stevilo) {

  radio.stopListening();                                  // First, stop listening so we can talk.

  //printf("Now sending %d as payload... \n\r", stevilo);
  byte gotByte;
  //unsigned long time = micros();                          // Take the time, and send it.  This will block until complete
  //Called when STANDBY-I mode is engaged (User is finished sending)
  if (!radio.write( &stevilo, 1 )) {
    Serial.println(F("failed."));
  } else {
    
    // ce imamo ACK!
    /*
    if (!radio.available()) {
      Serial.println(F("Blank Payload Received."));
    } else {
      while (radio.available() ) {
        unsigned long tim = micros();
        radio.read( &gotByte, 1 );
        printf("Got response %d, round-trip delay: %lu microseconds\n\r", gotByte, tim - time);
        //counter++;
      }
    }
    */
  }
}
