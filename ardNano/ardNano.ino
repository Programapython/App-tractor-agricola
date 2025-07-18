#include <Servo.h>

Servo servoDer; Servo servoIzq; Servo dirServo;

// Pines de componentes
// Entradas
const int SEN_NIVEL[3] = {A0,A1,A2};
const int SEN_DIST[4] = {A3,A4,A5,A6};
// Salidas
const int SERVOS[2] = {3,5}; //PWM
const int SERVO_DIR = 10; //PWM
const int BOMBAS[2] = {6,9}; //PWM
const int MOTOR = 2;
const int VALVULAS[6] = {4,7,8,11,12,A7};

// Variables a controlar
bool NIVELES[3] = {LOW, LOW, LOW};
int DISTANCIAS[6] = {0, 0, 0, 0};
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

void asignarValores(){
  //Asignamos valores a las valvulas
  for (int i=0; i<6; i++){digitalWrite(VALVULAS[i], values[i+1]);}

  //Arranque del motor
  if (values[0] == 0 && antValues[0] == 1){apagar(); Serial.println("1");}
  else if (values[0] == 1 && antValues[0] == 0) {encender(); Serial.println("2");}
  
  //Actualizamos los valores
  for (int i=0;i<7;i++){antValues[i] = values[i];}
}

void enviarValores(){
  Serial.print(NIVELES[0]);Serial.print(",");
  Serial.print(NIVELES[1]);Serial.print(",");
  Serial.print(NIVELES[2]);Serial.print(",");
  Serial.print(DISTANCIAS[0]);Serial.print(",");
  Serial.print(DISTANCIAS[1]);Serial.print(",");
  Serial.print(DISTANCIAS[2]);Serial.print(",");
  Serial.print(DISTANCIAS[3]);Serial.print(",");
  Serial.print(DISTANCIAS[4]);Serial.print(",");
  Serial.println(DISTANCIAS[5]);
}

void encender(){
  for (int i=0; i<45; i++){
    servoDer.write(2*i);
    servoIzq.write(2*i);
    delay(100);}
  delay(500);
  digitalWrite(MOTOR, HIGH);
}

void apagar(){
  digitalWrite(MOTOR, LOW);
  delay(500);
  for (int i=45; i>=0; i--){
    servoDer.write(2*i);
    servoIzq.write(2*i);
    delay(100);}
}

void setup(){
  //Comunicacion serial
  Serial.begin(9600);
  //Sensores nivel
  for (int i = 0; i<3; i++){
    pinMode(SEN_NIVEL[i], INPUT_PULLUP);}
  //Sensores distancia
  for (int i = 0; i<4; i++){
    pinMode(SEN_DIST[i], INPUT);}
  //Servomotores
  servoDer.attach(SERVOS[0]); servoDer.write(0);
  servoIzq.attach(SERVOS[1]); servoIzq.write(0);
  dirServo.attach(SERVO_DIR); dirServo.write(0);
  //Bombas
  for (int i = 0; i<4; i++){
    pinMode(BOMBAS[i], OUTPUT);digitalWrite(BOMBAS[i], OUTPUT);}
  //Motor
  pinMode(MOTOR, OUTPUT);
  //Valvulas
    for (int i = 0; i<6; i++){
    pinMode(VALVULAS[i], OUTPUT);digitalWrite(VALVULAS[i], OUTPUT);}
}

void loop(){
  //Lectura de los sensores de nivel
  for (int i = 0; i<3; i++){NIVELES[i] = !digitalRead(SEN_NIVEL[i]);}
  //Lectura de los sensores de distancia
  for (int i = 0; i<4; i++){DISTANCIAS[i] = ADC_promedio(SEN_DIST[i], 20);}
  
  //Recibir valores
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
  enviarValores();
  delay(100);
}