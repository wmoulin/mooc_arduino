#include <Servo.h> 
 
Servo myservo;  // création d'un objet myservo issu de la librairie Servo
              
int pos;

void setup() 
{ 
  Serial.begin(9600);
} 
 
void loop() 
{
    Serial.print(analogRead(A0));
    Serial.print(" / ");
    Serial.println(map(analogRead(A0), 37, 790, 0, 179));
}

