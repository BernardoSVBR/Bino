#include <Servo.h>
#include <SoftwareSerial.h>

Servo servoCabeca;  
int motorDC1DIR1 = 5;    
int motorDC1DIR2 = 6;    
int motorDC2DIR1 = 9;    
int motorDC2DIR2 = 10;   
int trigPin = 4;   
int echoPin = 3;   
int ledPin = 2;    

int obstacleThreshold = 10;  
int tiltThreshold = 45;      

char command;
bool isDanger = false; 
bool isLEDOn = false; 

SoftwareSerial bluetooth(0, 1); 

void setup() {
  servoCabeca.attach(11);  
  pinMode(motorDC1DIR1, OUTPUT);
  pinMode(motorDC1DIR2, OUTPUT);
  pinMode(motorDC2DIR1, OUTPUT);
  pinMode(motorDC2DIR2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, HIGH);

  Serial.begin(9600); 
  bluetooth.begin(9600); 
}

void loop() {
  if (bluetooth.available() > 0) {
    command = bluetooth.read(); 
    if (command == '1') {
      // Cabeça (Direita)
      servoCabeca.write(0);

    } else if (command == '2') {
      // Cabeça (Esquerda)
      servoCabeca.write(180);

    } else if (command == '3') {
      // Cabeça (Centro)
      servoCabeca.write(90);

    } else if (command == '4') {
      // Motor (Frente)
      digitalWrite(motorDC1DIR1, LOW);
      digitalWrite(motorDC1DIR2, HIGH);
      digitalWrite(motorDC2DIR1, LOW);
      digitalWrite(motorDC2DIR2, HIGH);

    } else if (command == '5') {
      // Ré (Motor)
      if (!isDanger) {
        digitalWrite(motorDC1DIR1, HIGH);
        digitalWrite(motorDC1DIR2, LOW);
        digitalWrite(motorDC2DIR1, HIGH);
        digitalWrite(motorDC2DIR2, LOW);

      }
    } else if (command == '6') {
      // Esquerda (Motor)
      digitalWrite(motorDC1DIR1, LOW);
      digitalWrite(motorDC1DIR2, HIGH);
      digitalWrite(motorDC2DIR1, HIGH);
      digitalWrite(motorDC2DIR2, LOW);

    } else if (command == '7') { 
      // Direita (Motor)
      digitalWrite(motorDC1DIR1, HIGH);
      digitalWrite(motorDC1DIR2, LOW);
      digitalWrite(motorDC2DIR1, LOW);
      digitalWrite(motorDC2DIR2, HIGH);
      
    } else if (command == '8') {
      // Parar (Center Motor)
      digitalWrite(motorDC1DIR1, LOW);
      digitalWrite(motorDC1DIR2, LOW);
      digitalWrite(motorDC2DIR1, LOW);
      digitalWrite(motorDC2DIR2, LOW);
    }
  }

  long duration;
  float distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2.0) * 0.0344; 

  if (distance < obstacleThreshold) {
    for (int angle = 0; angle <= 180; angle++) {
      servoCabeca.write(angle);
      delay(15);
    }
    isDanger = true;
  } else {
    if (isDanger) {
      servoCabeca.write(90); 
      isDanger = false;
    }
  }

  if (isDanger) {
    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    static bool ledState = HIGH;

    if (currentMillis - previousMillis >= 500) {
      previousMillis = currentMillis;
      digitalWrite(ledPin, ledState);
      ledState = !ledState;
    }
  } else {
    digitalWrite(ledPin, HIGH);
  }
  
  // Impressão na porta serial para debug
  Serial.println("Olá, mundo!"); 
  int sensorValue = analogRead(A0);
  Serial.print("Valor do sensor: ");
  Serial.println(sensorValue);

  delay(1000); 
}
