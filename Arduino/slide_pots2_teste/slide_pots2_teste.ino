#define led_pin1 6
#define led_pin2 7
#define slide_pot_pin A0
#define pot_pin A1

int val_pot = 0;
int val_slide_pot = 0;

void setup() {
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(slide_pot_pin, INPUT_PULLUP);
}

void loop() {

  val_slide_pot = analogRead(slide_pot_pin);            //Faz a leitura analógica do pino em que o potenciômetro esta ligado
  val_slide_pot = map(val_slide_pot, 0, 1023, 3, 255);  //Utilizando a função map() para transformar uma escala de 0-1023 em uma escala 0 a 255
  analogWrite(led_pin1, val_slide_pot);
  delay(180);

  val_pot = analogRead(pot_pin);            //Faz a leitura analógica do pino em que o potenciômetro esta ligado
  val_pot = map(val_pot, 0, 1023, 0, 255);  //Utilizando a função map() para transformar uma escala de 0-1023 em uma escala 0 a 255
  analogWrite(led_pin2, val_pot);           // Aciona o LED proporcionalmente ao valor da leitura analógica
  delay(180);
}
