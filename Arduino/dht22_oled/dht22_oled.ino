#include <Adafruit_AM2320.h>
#include "U8glib.h"
#include <DHT.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

// #define pino_DHT A0
#define pino_DHT 2
#define DHTTYPE DHT22

DHT dht(pino_DHT, DHTTYPE);

int temperatura;
int umidade;

unsigned long previousMillis = 0;
const long interval = 2000;

void draw() {
  //Retangulo temperatura atual
  u8g.drawRFrame(0, 17, 127, 46, 2);
  u8g.setFont(u8g_font_fub17);
  //Atualiza a temperatura no display
  u8g.setPrintPos(5, 50);
  u8g.print(temperatura);
  //Circulo grau
  u8g.drawCircle(38, 33, 3);
  u8g.drawStr(42, 50, "C/");

  //Atualiza a umidade no display
  u8g.setPrintPos(70, 50);
  u8g.print(umidade);
  u8g.drawStr(100, 50, "%");

  //Box superior amarelo
  u8g.setFont(u8g_font_8x13B);
  u8g.drawRBox(0, 0, 127, 16, 2);
  u8g.setColorIndex(0);
  u8g.drawStr(21, 13, "TEMP/UMIDADE");
  u8g.setColorIndex(1);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {

  unsigned long currentMillis = millis();
  // Timer para ler o valor da temperatura e da umidade
  if (currentMillis - previousMillis >= interval) {
    temperatura = dht.readTemperature();
    umidade = dht.readHumidity();
    previousMillis = currentMillis;
  }

  u8g.firstPage();
  // Chama a funcao de desenho na tela
  do {
    draw();
  } while (u8g.nextPage());
  delay(50);
}