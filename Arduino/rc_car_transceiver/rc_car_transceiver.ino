#include <RF24.h>

RF24 radio(9, 10);

struct MeuControleRemoto {
  boolean Botao_0;
  boolean Botao_1;
  boolean Botao_2;
  boolean Botao_3;
  boolean Botao_4;
  boolean Botao_5;
  boolean Botao_6;
  boolean Botao_7;
  boolean Botao_JoyA;
  boolean Botao_JoyB;
  byte Joystick_AX;
  byte Joystick_AY;
  byte Joystick_BX;
  byte Joystick_BY;
  byte Pot_A;
  byte Pot_B;
};

MeuControleRemoto ControleRemoto;

void setup() {

  //Serial.begin(115200);
  // delay(100);
  // Serial.println("*TRANSMISSOR*");
  // delay(100);

  radio.begin();
  radio.setAutoAck(false);
  radio.setChannel(100);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.powerUp();
  radio.openWritingPipe(0xE8E8F0F0E1LL);

  delay(100);

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(8, OUTPUT);

  tone(8, 1000, 100);
  delay(100);
  tone(8, 800, 100);
  delay(100);
  tone(8, 600, 100);
  delay(100);
  tone(8, 400, 100);
  delay(100);
  tone(8, 200, 100);
  delay(100);
}

void loop() {

  readVcc();

  // Lendo Botões
  ControleRemoto.Botao_0 = digitalRead(0);
  ControleRemoto.Botao_1 = digitalRead(1);
  ControleRemoto.Botao_2 = digitalRead(2);
  ControleRemoto.Botao_3 = digitalRead(3);
  ControleRemoto.Botao_4 = digitalRead(4);
  ControleRemoto.Botao_5 = digitalRead(5);
  ControleRemoto.Botao_6 = digitalRead(6);
  ControleRemoto.Botao_7 = digitalRead(7);
  ControleRemoto.Botao_JoyA = digitalRead(A2);
  ControleRemoto.Botao_JoyB = digitalRead(A3);

  // LENDO JOYSTICKS
  ControleRemoto.Joystick_AX = map(analogRead(A1), 0, 1023, 0, 255);
  ControleRemoto.Joystick_AY = map(analogRead(A0), 0, 1023, 0, 255);
  ControleRemoto.Joystick_BX = map(analogRead(A4), 0, 1023, 0, 255);
  ControleRemoto.Joystick_BY = map(analogRead(A5), 0, 1023, 0, 255);

  // LENDO POTENCIÔMETROS
  ControleRemoto.Pot_A = map(analogRead(A6), 0, 1023, 0, 255);
  ControleRemoto.Pot_B = map(analogRead(A7), 0, 1023, 0, 255);

  // Transmitindo dados
  for (int i = 0; i < 10; i++) {
    radio.write(&ControleRemoto, sizeof(ControleRemoto));
  }
}

void readVcc() {

  // Ler referência interna 1.1V
  // Ajusta a referência ao Vcc e a medição de referência interna para 1.1V
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC))
    ;
  uint8_t low = ADCL;
  uint8_t high = ADCH;
  int result = (high << 8) | low;
  result = 1125300L / result;  // Calcular Vcc em milivolts

  if (result < 3100) {  // Aciona o buzzer se a tensão da bateria estiver menor que 3,1 volts (para indicar que a bateria esta fraca)
    tone(8, 900, 100);
  }
}