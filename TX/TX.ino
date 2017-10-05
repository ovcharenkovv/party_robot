#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9, 10); // Setup RF24 mudule in 9 -  10 pins
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // avalible pipes

//Left
byte left_c = 2;
byte left_x = 3;
byte left_y = 4;

//Right
byte right_c = 5;
byte right_x = 6;
byte right_y = 7;

word transmit_data[6]; // transmit date array

void setup() {
  Serial.begin(9600);

  radio.begin();                     // Activate the module
  delay(5);
  radio.setAutoAck(1);               // Confirm recieving
  radio.setRetries(0, 15);           // (time between attemps, max amount count)
  radio.enableAckPayload();          // allow data to be sent in response to an incoming signal
  radio.setPayloadSize(32);          // packet size, in bytes

  radio.openWritingPipe(address[0]); // sent to the pipe 0
  radio.setChannel(0x60);            // select the channel (in which there is no noise!)

  radio.setPALevel (RF24_PA_MAX);    // yhe level of transmit power RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS);  // The transmitting speed RF24_2MBPS, RF24_1MBPS, RF24_250KBPS (should be same for transmitter and resiver)

  radio.powerUp();
  radio.stopListening();
}

void loop() {

  transmit_data[0] = analogRead(left_y);
  transmit_data[1] = analogRead(right_y);

  transmit_data[2] = analogRead(left_x);
  transmit_data[3] = analogRead(right_x);

  transmit_data[4] = !analogRead(left_c);
  transmit_data[5] = !analogRead(right_c);

  //Serial.print("Transmited: "); Serial.println(transmit_data[1]);

  radio.write(&transmit_data, sizeof(transmit_data));

  delay(10);
}
