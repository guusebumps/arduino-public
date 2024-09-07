#define ledPin1 6         // Define o pino que terá um LED e um resistência ligada ao terra
#define ledPin2 7         // Define o pino que terá um LED e um resistência ligada ao terra
#define slide_pot_pin A0  // Define o pino que terá um LED e um resistência ligada ao terra
#define pot_pin A1        // Define o pino que terá um LED e um resistência ligada ao terra

int valPot = 0;  //Variável que armazena o valor da leitura do potenciômetro

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);  // Configura o pino do LED como saída
  pinMode(ledPin2, OUTPUT);  // Configura o pino do LED como saída
  pinMode(slide_pot_pin, INPUT_PULLUP);
}

void loop() {

  int value_slide_pot = analogRead(slide_pot_pin);

  value_slide_pot = map(value_slide_pot, 0, 1023, 0, 255);  //Utilizando a função map() para transformar uma escala de 0-1023 em uma escala 0 a 255
  analogWrite(ledPin1, value_slide_pot);                     // Aciona o LED proporcionalmente ao valor da leitura analógica
  Serial.print("value_slide_pot= ");
  Serial.println(value_slide_pot);
  delay(100);

  valPot = analogRead(pot_pin);            //Faz a leitura analógica do pino em que o potenciômetro esta ligado
  valPot = map(valPot, 0, 1023, 0, 255);  //Utilizando a função map() para transformar uma escala de 0-1023 em uma escala 0 a 255
  analogWrite(ledPin2, valPot);            // Aciona o LED proporcionalmente ao valor da leitura analógica
  Serial.print("valPot= ");
  Serial.println(valPot);
  delay(100);  
}
