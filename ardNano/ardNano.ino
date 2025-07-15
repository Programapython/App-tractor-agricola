#include <Servo.h>

Servo myservo1; Servo myservo2;

// Pines de componentes
const int SEN_NIVEL[3] = {2,3,4};
const int SEN_DIST[6] = {A0,A1,A2,A3,A4,A5};
const int VALVULAS[6] = {5, 6, 7, 8, 9, 12};
const int SERVOS[2] = {10,11};
const int MOTOR = 1;
// Variables a controlar
bool NIVELES[3] = {LOW, LOW, LOW};
int DISTANCIAS[6] = {0, 0, 0, 0, 0, 0};
//////////////////////{START, VAL1, VAL2, VAL3, VAL4, VAL5, VAL6}
bool antValues[7] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW};
bool values[7] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW};
//Retardo a utilizar
int dlay = 100;

int ADC_promedio(int pin, int n){
  long suma=0;
  for(int i=0;i<n;i++){
    suma=suma+analogRead(pin);
  }  
  return(suma/n);
}

int asignarValores(){
  for (int i=0; i<6; i++){digitalWrite(VALVULAS[i], values[i+1]);}
  if (values[0] == 0 && antValues[0] == 1){myservo.write(0);}
  else if (values[0] == 1 && antValues[0] == 0) {myservo1.write(90); myservo2.write(90);
  delay(5); digitalWrite(MOTOR, HIGH);}

  for (int i=0;i<6;i++){
    antValues[i] == values[i];
  }
}

void setup(){
  myservo1.attach(SERVOS[0]); myservo2.attach(SERVOS[1]);
  Serial.begin(9600);
  for (int i = 0; i<3; i++){
    pinMode(SEN_NIVEL[i], INPUT_PULLUP);}
  for (int i = 0; i<6; i++){
    pinMode(SEN_DIST[i], INPUT);}

  myservo.write(0);
  myservo.write(0);
}

void loop(){
  //int data_nivel;

  for (int i = 0; i<3; i++){
    NIVELES[i] = !digitalRead(SEN_NIVEL[i]);
  }

  for (int i = 0; i<6; i++){
    DISTANCIAS[i] = ADC_promedio(SEN_DIST[i], 20);
  }
  
  //data_nivel = NIVELES[2]*100+NIVELES[1]*10+NIVELES[0];
  
  if (Serial.available()>0){
    String input = Serial.readStringUntil('\n'); // Lee hasta nueva línea
    input.trim();  // Elimina espacios

    //convertir la cadena en un array
    int index = 0;
    char *ptr = strtok((char*)input.c_str(), ",");
    while (ptr != NULL && index < 7) {
      values[index++] = atoi(ptr);
      ptr = strtok(NULL, ",");
    }
  }
  asignarValores();

  Serial.print(NIVELES[0]);Serial.print(",");
  Serial.print(NIVELES[1]);Serial.print(",");
  Serial.print(NIVELES[2]);Serial.print(",");
  Serial.print(DISTANCIAS[0]);Serial.print(",");
  Serial.print(DISTANCIAS[1]);Serial.print(",");
  Serial.print(DISTANCIAS[2]);Serial.print(",");
  Serial.print(DISTANCIAS[3]);Serial.print(",");
  Serial.print(DISTANCIAS[4]);Serial.print(",");
  Serial.println(DISTANCIAS[5]);
  delay(1000);
}

/*
float distancia(int pin, int n){
  long suma=0;
  for(int i=0;i<n;i++)
  {
    suma=suma+analogRead(pin);
  }  
  float adc=suma/n;
  float distancia_cm = 17569.7 * pow(adc, -1.2062);
  return(distancia_cm);
}
*/
