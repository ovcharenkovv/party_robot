#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>
#include <Wire.h>

// Constants
#define pinRF        2
#define pinRB        4
#define Rpwm_pin     5

#define pinLF        7
#define pinLB        8
#define Lpwm_pin     3

#define servoPin     6

#define defaultLeft  496
#define defaultRight 516

//Radio
RF24 radio(9, 10);
word recieved_data[6];
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

// Variables
word righty;
word lefty;
word leftx;

word maxSpeed;

//Servo yesServo;


void setup() {
  Serial.begin(9600);

  radio.begin();                            // Activate the module
  delay(5);
  radio.setAutoAck(1);                      // Confirm recieving
  radio.setRetries(0, 15);                  // (time between attemps, max amount count)
  radio.enableAckPayload();                 // allow data to be sent in response to an incoming signal
  radio.setPayloadSize(32);                 // packet size, in bytes

  radio.openReadingPipe(1, address[0]);     // listem the pipe 0
  radio.setChannel(0x60);                   // select the channel (in which there is no noise!)

  radio.setPALevel (RF24_PA_MIN);           // the level of transmit power RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS);           // The transmitting speed RF24_2MBPS, RF24_1MBPS, RF24_250KBPS (should be same for transmitter and resiver)

  radio.powerUp();
  radio.startListening();

//  yesServo.attach(servoPin);

  //Engine Setup
  pinMode(pinLF, OUTPUT);
  pinMode(pinLB, OUTPUT);
  pinMode(pinRF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(Lpwm_pin, OUTPUT);
  pinMode(Rpwm_pin, OUTPUT);

  maxSpeed = 160;
}


// Setting speed for the engine
void set_speed_left(byte speed) {
  analogWrite(Lpwm_pin, speed);
}
void set_speed_right(byte speed) {
  analogWrite(Rpwm_pin, speed);
}

// Actions with left engine
void left_stop() {
  set_speed_left(0);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, HIGH);
}

void left_forward(byte speed) {
  set_speed_left(speed);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);
}

void left_back(byte speed) {
  set_speed_left(speed);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);
}


// Actions with right engine
void right_stop() {
  set_speed_right(0);
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, HIGH);
}

void right_forward(byte speed) {
  set_speed_right(speed);
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, LOW);
}

void right_back(byte speed) {
  set_speed_right(speed);
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);
}



void loop() {

  byte pipeNo;

  while ( radio.available(&pipeNo)) {  // listening all pipes
    radio.read( &recieved_data, sizeof(recieved_data) );

    //    transmit_data[0] = analogRead(LEFT_Y);
    //    transmit_data[1] = analogRead(RIGHT_Y);
    //    transmit_data[2] = analogRead(LEFT_X);
    //    transmit_data[3] = analogRead(RIGHT_X);
    //    transmit_data[4] = !analogRead(LEFT_C);
    //    transmit_data[5] = !analogRead(RIGHT_C);

    lefty = recieved_data[0]; // default 496
    righty = recieved_data[1]; // default 516
    leftx = recieved_data[2]; 


    Serial.print("Recived Left: "); Serial.println(lefty);
    Serial.print("Recived Right: "); Serial.println(righty);

    // process left engine
//    yesServo.write(
//      map(leftx, 0, 1023, 60, 120)
//    );

    // process left engine

    if (lefty == defaultLeft)
    {
      left_stop();
    }

    else if (lefty > defaultLeft)
    {
      left_forward(
        map(lefty, defaultLeft, 1023, 50, maxSpeed)
      );
    }

    else if (lefty < defaultLeft)
    {
      left_back(
        map(lefty, defaultLeft, 0, 50, maxSpeed)
      );
    }


    // process right engine

    if (righty == defaultRight)
    {
      right_stop();
    }

    else if (righty > defaultRight)
    {
      right_forward(
        map(righty, defaultRight, 1023, 50, maxSpeed)
      );
    }

    else if (righty < defaultRight)
    {
      right_back(
        map(righty, defaultRight, 0, 50, maxSpeed)
      );
    }

    delay(10);
  }
}
