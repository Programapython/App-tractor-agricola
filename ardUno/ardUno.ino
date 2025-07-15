#if 1

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
const int TS_LEFT = 967, TS_RT = 193, TS_TOP = 286, TS_BOT = 900;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

//Creacion de los botones
Adafruit_GFX_Button V1D_btn, V2D_btn, V3D_btn;
Adafruit_GFX_Button V1I_btn, V2I_btn, V3I_btn;
Adafruit_GFX_Button ACT_btn;
Adafruit_GFX_Button START_btn, STOP_btn;

//Retardo botones
int dlay = 100;

//Valores de las variables a utilizar
//////////////////////{ESTADO, NIV1, NIV2, NIV3, MODIF_SVALVULAS, V1D, V2D, V3D, V1I, V2I, V3I}
bool valuesAnt[11] = {LOW, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool values[11] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
bool modNivel = LOW;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void){
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.y, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
    pixel_y = map(p.x, TS_TOP, TS_BOT, 0, tft.height());
    //Serial.println(p.x);Serial.println(p.y);
    //Serial.println(tft.height());Serial.println(tft.width());
    //Serial.println(pixel_x);Serial.println(pixel_y);
  }
  return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup(void)
{
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(1); //PORTRAIT
  tft.fillScreen(BLACK);

  //COLUMNA IZQUIERDA
  
  tft.setTextColor(YELLOW); // Color del texto: AMARILLO
  tft.setTextSize(2);  // Tamaño del texto (1-5)
  tft.setCursor(10, 10); // Posición
  tft.print("X       VEHICULO AUTOPROPULSADO       X");    // Texto a mostrar

  tft.setTextColor(WHITE); // Color del texto: BLANCO
  tft.setTextSize(2); // Tamaño del texto (1-5)
  tft.setCursor(10, 40); // PosicióN
  tft.print("NIVEL DE TANQUE ..."); // Texto a mostrar

  tft.fillRect(80, 75, 60, 99, BLUE); //Dibujo del tanque
  tft.drawRect(80, 75, 60, 99, WHITE); //Borde del tanque

  tft.setTextSize(2); // Tamaño del texto (1-5)
  tft.setCursor(60, 200); tft.print("VAL1 VAL2 VAL3"); // Posición X=60, Y=200
  tft.setTextSize(2); // Tamaño del texto (1-5)
  tft.setCursor(10, 240); tft.print("DER"); // Posición X=10, Y=240
  tft.setTextSize(2); // Tamaño del texto (1-5)
  tft.setCursor(10, 280); tft.print("IZQ"); // Posición X=10, Y=280

  ACT_btn.initButton(&tft, 30, 206, 40, 25, WHITE, RED, BLACK, "MD", 2);

  V1D_btn.initButton(&tft, 80, 245, 40, 30, WHITE, CYAN, BLACK, "ON", 2);
  V2D_btn.initButton(&tft, 142, 245, 40, 30, WHITE, CYAN, BLACK, "ON", 2);
  V3D_btn.initButton(&tft, 204, 245, 40, 30, WHITE, CYAN, BLACK, "ON", 2);
  V1I_btn.initButton(&tft, 80, 290, 40, 30, WHITE, CYAN, BLACK, "ON", 2);
  V2I_btn.initButton(&tft, 142, 290, 40, 30, WHITE, CYAN, BLACK, "ON", 2);
  V3I_btn.initButton(&tft, 204, 290, 40, 30, WHITE, CYAN, BLACK, "ON", 2);

  V1D_btn.drawButton(false); V2D_btn.drawButton(false); V3D_btn.drawButton(false);
  V1I_btn.drawButton(false); V2I_btn.drawButton(false); V3I_btn.drawButton(false);
  ACT_btn.drawButton(false);

  //COLUMNA DERECHA

  tft.setTextColor(WHITE);     // Color del texto: VERDE
  tft.setTextSize(2);           // Tamaño del texto (1-5)
  tft.setCursor(280, 40);       // Posición X=50, Y=100
  tft.print("ESTADO GENERAL");    // Texto a mostrar

  tft.setTextColor(RED); // Color del texto: BLANCO
  tft.setCursor(280, 70); // Posición
  tft.print(" DETENIDO ..."); // Texto a mostrar

  tft.setTextColor(WHITE);     // Color del texto: VERDE
  tft.setCursor(280, 120);       // Posición X=50, Y=100
  tft.print("VELOCIDAD");    // Texto a mostrar

  tft.setTextColor(RED); // Color del texto: BLANCO
  tft.setCursor(280, 150); // Posición
  tft.print(" 0 m/s"); // Texto a mostrar

  START_btn.initButton(&tft, 350, 230, 140, 40, WHITE, GREEN, BLACK, "START", 2);
  STOP_btn.initButton(&tft, 350, 280, 140, 40, WHITE, RED, BLACK, "STOP", 2);

  START_btn.drawButton(false); STOP_btn.drawButton(false);
}

void loop(void){
  bool down = Touch_getXY();

  ACT_btn.press(down && ACT_btn.contains(pixel_x, pixel_y));
  V1D_btn.press(down && V1D_btn.contains(pixel_x, pixel_y));
  V2D_btn.press(down && V2D_btn.contains(pixel_x, pixel_y));
  V3D_btn.press(down && V3D_btn.contains(pixel_x, pixel_y));
  V1I_btn.press(down && V1I_btn.contains(pixel_x, pixel_y));
  V2I_btn.press(down && V2I_btn.contains(pixel_x, pixel_y));
  V3I_btn.press(down && V3I_btn.contains(pixel_x, pixel_y));
  START_btn.press(down && START_btn.contains(pixel_x, pixel_y));
  STOP_btn.press(down && STOP_btn.contains(pixel_x, pixel_y));

  //if (ACT_btn.justReleased()){ACT_btn.drawButton();}
  //if (V1D_btn.justReleased()){V1D_btn.drawButton();}
  //if (V2D_btn.justReleased()){V2D_btn.drawButton();}
  //if (V3D_btn.justReleased()){V3D_btn.drawButton();}
  //if (V1I_btn.justReleased()){V1I_btn.drawButton();}
  //if (V2I_btn.justReleased()){V2I_btn.drawButton();}
  //if (V3I_btn.justReleased()){V3I_btn.drawButton();}
  if (START_btn.justReleased()){START_btn.drawButton();}
  if (STOP_btn.justReleased()){STOP_btn.drawButton();}

  if (ACT_btn.justPressed() && values[0]) {
    values[4] = !values[4];
    if (values[4]){Serial.println("ac");}else{Serial.println("de");}
    delay(dlay);
  }
  if (V1D_btn.justPressed() && values[4]) {
    values[5] = !values[5];
    if (values[5]){Serial.println("a1");}else{Serial.println("d1");}
    delay(dlay);
  }
  if (V2D_btn.justPressed() && values[4] && values[0]) {
    values[6] = !values[6];
    if (values[6]){Serial.println("a2");}else{Serial.println("d2");}
    delay(dlay);
  }
  if (V3D_btn.justPressed() && values[4] && values[0]) {
    values[7] = !values[7];
    if (values[7]){Serial.println("a3");}else{Serial.println("d3");}
    delay(dlay);
  }
  if (V1I_btn.justPressed() && values[4] && values[0]) {
    values[8] = !values[8];
    if (values[8]){Serial.println("a4");}else{Serial.println("d4");}
    delay(dlay);
  }
  if (V2I_btn.justPressed() && values[4] && values[0]) {
    values[9] = !values[9];
    if (values[9]){Serial.println("a5");}else{Serial.println("d5");}
    delay(dlay);
  }
  if (V3I_btn.justPressed() && values[4] && values[0]) {
    values[10] = !values[10];
    if (values[10]){Serial.println("a6");}else{Serial.println("d6");}
    delay(dlay);
  }
  if (START_btn.justPressed()) {
    START_btn.drawButton(true);
    values[0] = HIGH;
    Serial.println("s");
    delay(dlay);
  }
  if (STOP_btn.justPressed()) {
    STOP_btn.drawButton(true);
    values[0] = LOW;
    Serial.println("r");
    delay(dlay);
  }

  conex();
}

void conex() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Lee hasta nueva línea
    input.trim();  // Elimina espacios

    //convertir la cadena en un array
    int index = 0;
    char *ptr = strtok((char*)input.c_str(), ",");
    while (ptr != NULL && index < 11) {
      values[index++] = atoi(ptr);
      ptr = strtok(NULL, ",");
    }
  }

  if (valuesAnt[0] == !values[0]){
    if (values[0]){
      tft.fillRect(290, 70, 160, 30, BLACK); tft.fillRect(290, 150, 160, 30, BLACK);
      tft.setTextColor(GREEN); tft.setCursor(280, 70); tft.print(" ACTIVADO ...");
      tft.setTextColor(GREEN); tft.setCursor(280, 150); tft.print(" ... m/s");
    } else {
      tft.fillRect(290, 70, 160, 30, BLACK); tft.fillRect(290, 150, 160, 30, BLACK);
      tft.setTextColor(RED); tft.setCursor(280, 70); tft.print(" DETENIDO ...");
      tft.setTextColor(RED); tft.setCursor(280, 150); tft.print(" 0 m/s");
    }
  }

  if (valuesAnt[1] == !values[1]){
    if(values[1]){tft.fillRect(80, 75, 60, 16, BLUE);}else{tft.fillRect(80, 75, 60, 16, BLACK);}
    modNivel = HIGH;
  }
  if (valuesAnt[2] == !values[2]){
    if(values[2]){tft.fillRect(80, 91, 60, 33, BLUE);}else{tft.fillRect(80, 91, 60, 33, BLACK);}
    modNivel = HIGH;
  }
  if (valuesAnt[3] == !values[3]){
    if(values[3]){tft.fillRect(80, 124, 60, 33, BLUE); tft.fillRect(80, 157, 60, 17, BLUE);}
    else{tft.fillRect(80, 124, 60, 33, BLACK); tft.fillRect(80, 157, 60, 17, RED);}
    modNivel = HIGH;
  }

  if (modNivel){tft.drawRect(80, 75, 60, 99, WHITE); modNivel = LOW;} //Borde del tanque

  if (valuesAnt[4] == !values[4]){ACT_btn.drawButton(!values[4]);}
  if (valuesAnt[5] == !values[5]){V1D_btn.drawButton(!values[5]);}
  if (valuesAnt[6] == !values[6]){V2D_btn.drawButton(!values[6]);}
  if (valuesAnt[7] == !values[7]){V3D_btn.drawButton(!values[7]);}
  if (valuesAnt[8] == !values[8]){V1I_btn.drawButton(!values[8]);}
  if (valuesAnt[9] == !values[9]){V2I_btn.drawButton(!values[9]);}
  if (valuesAnt[10] == !values[10]){V3I_btn.drawButton(!values[10]);}

  for (int i=0; i<11; i++){valuesAnt[i] = values[i];}
}

#endif