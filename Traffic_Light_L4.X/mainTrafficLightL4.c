//TEJ3M mainTrafficLightL4.c
/*********************************************************************
Module:
main ()
author: Daniel Liang
date: January 19, 2016
Hardware Notes:
* PIC24FV32KA302 operating at 8MHz
Outputs:
* 12 LEDs for four traffic lights
* Stepper motor for level crossing
********************************************************************/
/*******************************************************************
Include Files
********************************************************************/
#include "p24fv32ka302.h"
#include "configBits.h"
#include "delay.h"
/*******************************************************************
Local Function Prototypes
********************************************************************/
void initTimer (void);
void delay (unsigned long milli);
/*******************************************************************
Global Variable Declarations
*****************'***************************************************/
const int on = 1;
const int off = 0;
//outputs
const int greenNS = 6;
const int yellowNS = 7;
const int redNS = 8;
const int greenEW = 9;
const int yellowEW = 10;
const int redEW = 11;
//inputs
const int ped = 0;
const int adv = 1;
const int rail = 2;
//Has button been pressed?
int pedOn = 0;
int advOn = 0;
//stepper motor and level crossing variables
const int cw = 1;
const int ccw = 0;
const int stepdelay = 40;
int D[] = {12, 15, 13, 14}; //stepper motor outputs
/*******************************************************************
main() function
********************************************************************/
int main(void){
   //initialize peripherals
   initTimer();
   //PORTB all outputs
   TRISB=0;
   // initialize PORTB
   LATB=0;
   //PORTA all digital inputs
   TRISA=0xFF;
   ANSA=0;
   //repeat traffic cycle forever
   while(1){
      //NS CORRIDOR GREEN CYCLE
      //turn all red lights on
      digitalWrite(redNS, on);
      digitalWrite(redEW, on);
      //wait for 1 second, while checking button inputs ever 10 milliseconds
      int a;
      for(a = 1; a <= 100; a++){
          delay(10);
          if(digitalRead(rail)) railroad(redNS);
          if(digitalRead(ped)) pedOn = 1;
          if(digitalRead(adv)) advOn = 1;
      }
      //turn red light off
      digitalWrite(redNS, off);
      //if the advance green button was pressed, do advance green cycle
      if(advOn){
          advance();
          advOn = 0;
      }
      //turn green light on
      digitalWrite(greenNS, on);
      //wait for 7 seconds while checking button inputs every 70 milliseconds
      int b;
      for(b = 1; b <= 100; b++){
          delay(70);
          if(digitalRead(rail)) railroad(greenNS);
          if(digitalRead(ped)) pedOn = 1;
      }
      //turn green light off, turn yellow light on
      digitalWrite(greenNS, off);
      digitalWrite(yellowNS, on);
      //wait for 2 seconds while checking button inputs every 20 milliseconds
      int c;
      for(c = 1; c <= 100; c++){
          delay(20);
          if(digitalRead(rail)) railroad(yellowNS);
          if(digitalRead(ped)) pedOn = 1;
      }
      //turn yellow light off, turn red light on
      digitalWrite(yellowNS, off);
      digitalWrite(redNS, on);
      //both red lights are on
      //wait for 1 second while checking button inputs every 10 milliseconds
      int d;
      for(d = 1; d <= 100; d++){
          delay(10);
          if(digitalRead(rail)) railroad(redNS);
          if(digitalRead(ped)) pedOn = 1;
          if(digitalRead(adv)) advOn = 1;
      }
      //EW CORRIDOR GREEN CYCLE
      //turn red light off
      digitalWrite(redEW, off);
      //if pedestrian button was clicked, do pedestrian cycle
      if(pedOn){
          pedestrian();
          pedOn = 0;
      }
      //turn green light on
      digitalWrite(greenEW, on);
      //wait for 7 seconds while checking button inputs every 70 milliseconds
      int e;
      for(e = 1; e <= 100; e++){
          delay(70);
          if(digitalRead(rail)) railroad(greenEW);
          if(digitalRead(adv)) advOn = 1;
      }
      //turn green light off, turn yellow light on
      digitalWrite(greenEW, off);
      digitalWrite(yellowEW, on);
      //wait for 2 seconds while checking button inputs every 20 milliseconds
      int f;
      for(f = 1; f <= 100; f++){
          delay(20);
          if(digitalRead(rail)) railroad(yellowEW);
          if(digitalRead(adv)) advOn = 1;
      }
      //turn yellow light off
      digitalWrite(yellowEW, off);
      //return to beginning of NS corridor green cycle
   }
}//end maintrafficLight
/*******************************************************************
List of Functions
********************************************************************/
//This function assigns a state to pins RB6 to RB15
void digitalWrite(int pin, int power){
    switch(pin){
        case 6:
            LATBbits.LATB6 = power;
        break;
        case 7:
            LATBbits.LATB7 = power;
        break;
        case 8:
            LATBbits.LATB8 = power;
        break;
        case 9:
            LATBbits.LATB9 = power;
        break;
        case 10:
            LATBbits.LATB10 = power;
        break;
        case 11:
            LATBbits.LATB11 = power;
        break;
        case 12:
            LATBbits.LATB12 = power;
        break;
        case 13:
            LATBbits.LATB13 = power;
        break;
        case 14:
            LATBbits.LATB14 = power;
        break;
        case 15:
            LATBbits.LATB15 = power;
        break;
    }
}
//This function reads the state of the input pins RA0 to RA4
int digitalRead(int pin){
    switch(pin){
        case 0:
            return PORTAbits.RA0;
        break;
        case 1:
            return PORTAbits.RA1;
        break;
        case 2:
            return PORTAbits.RA2;
        break;
        case 3:
            return PORTAbits.RA3;
        break;
        case 4:
            return PORTAbits.RA4;
        break;
    }
}
//This function flashes yellow lights on EW
void pedestrian(){
    int i;
    for(i = 1; i <= 3; i++){
        digitalWrite(yellowEW, on);
        delay(500);
        digitalWrite(yellowEW, off);
        delay(500);
    }
}
//This function flashes green lights on NS
void advance(){
    int i;
    for(i = 1; i <= 3; i++){
        digitalWrite(greenNS, on);
        delay(500);
        digitalWrite(greenNS, off);
        delay(500);
    }
}
//This function flashes all red lights, lowers and raises level crossing bar
int railroad(int lightOn){
    //turn active intersection light
    digitalWrite(lightOn, off);
    //flash all red lights 2 times
    digitalWrite(redEW, on);
    digitalWrite(redNS, on);
    delay(500);
    digitalWrite(redEW, off);
    digitalWrite(redNS, off);
    delay(500);
    digitalWrite(redEW, on);
    digitalWrite(redNS, on);
    delay(500);
    digitalWrite(redEW, off);
    digitalWrite(redNS, off);
    //rotate stepper motor 90 degrees counterclockwise
    rotate(ccw, 3); //takes 480 ms
    delay(20); //total of 500 ms
    //flash all red lights 3 times
    digitalWrite(redEW, on);
    digitalWrite(redNS, on);
    delay(500);
    digitalWrite(redEW, off);
    digitalWrite(redNS, off);
    delay(500);
    digitalWrite(redEW, on);
    digitalWrite(redNS, on);
    delay(500);
    digitalWrite(redEW, off);
    digitalWrite(redNS, off);
    delay(500);
    digitalWrite(redEW, on);
    digitalWrite(redNS, on);
    delay(500);
    digitalWrite(redEW, off);
    digitalWrite(redNS, off);
    //rotate stepper motor 90 degrees clockwise
    rotate(cw, 3); //takes 480 ms
    delay(20); //total of 500 ms
    //flash all red lights 2 times
    digitalWrite(redEW, on);
    digitalWrite(redNS, on);
    delay(500);
    digitalWrite(redEW, off);
    digitalWrite(redNS, off);
    delay(500);
    digitalWrite(redEW, on);
    digitalWrite(redNS, on);
    delay(500);
    digitalWrite(redEW, off);
    digitalWrite(redNS, off);
    //turn on the light that was supposed to be on
    digitalWrite(lightOn, on);
    //turn on the correct red light
    if(lightOn <= redNS) digitalWrite(redEW, on);
    else digitalWrite(redNS, on);
}
//This function rotates the stepper motor a specified number of cycles and direction
void rotate(int direction, int cycles){
    int i;
    int _cycles;
    if(direction == ccw){//turn motor counterclockwise
        for(_cycles = 1; _cycles <= cycles; _cycles++){
           for (i = 0; i < 4; i++){//input 4-step code
              digitalWrite(D[i], on);
              delay(stepdelay);
              digitalWrite(D[i], off);
            }
        }//each cycle is 30 degrees or 4 steps
    }
    else{//turn motor clockwise
        int j;
        for(_cycles = 1; _cycles <= cycles; _cycles++){
           for (j = 3; j >= 0; j--){
              digitalWrite(D[j], on);
              delay(stepdelay);
              digitalWrite(D[j], off);
           }
        }
    }
}