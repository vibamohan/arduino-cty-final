#include <SR04.h>

SR04 ultraSonic = SR04(12, 11);

long dist; 

const int LED1 = 6;
const int LED2 = 5;
const int LED3 = 4;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  Serial.begin(9600);
  delay(1000);

}

void loop() {
  dist = ultraSonic.Distance();
  Serial.print(dist);
  Serial.println("cm");
  delay(100);
  if (dist < 20){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
  } else if (dist < 50) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
  } else if (dist < 75){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  } else if(dist < 100){
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  } 

}
