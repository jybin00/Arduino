#include <StepperMulti.h>

unsigned long past = 0;

StepperMulti stepper1(200, 28, 27, 26, 25);
StepperMulti stepper2(200, 10, 11, 12, 13);
double ang = 0;
 
void setup()
{
  Serial.begin(115200);
  stepper1.setSpeed(60);
  stepper2.setSpeed(60);
}
 
void loop()
{
  unsigned long curr = millis();
  if(curr - past == 10000)
  {
    past = curr;
    Serial.println(past);
    stepper2.setStep(300);
  }
    
  /*
  if (Serial.available())
  {
    int order=Serial.parseInt(); 

    stepper1.setStep(order);
    stepper2.setStep(order);
    delay(1000);

/*
    if(order == 0) front();
    if(order == 1) back();
    if(order == 2) left(); 
    if(order == 3) right();
*/
//    Serial.println(order);
 // }
}

void front() 
{
  stepper1.setStep(-100);
  stepper2.setStep(100);
}

void back()
{
  stepper1.setStep(200);
  stepper2.setStep(-200);
}

void left()
{
  stepper1.setStep(200);
  stepper2.setStep(200);
}

void right()
{
  stepper1.setStep(-200);
  stepper2.setStep(-200);
}
