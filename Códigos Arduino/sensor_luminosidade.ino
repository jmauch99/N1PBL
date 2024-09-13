#include <LiquidCrystal.h>
int ValorLDR = 0; //Armazenar a leitura do sensor LDR
int IntensidadeLuz; //Transforma a leitura em uma escala de 0 a 100
int pinoLDR = A0; //PINO ANALÓGICO utilizado para ler o LDR

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

byte LuzFraca[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B11111,
  B10001,
  B00000,
};

byte LuzMedia[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000,
};

byte LuzForte[8] = {
  B00000,
  B10001,
  B00000,
  B10001,
  B11111,
  B00000,
  B00000,
};

void setup() {
  lcd.begin(16, 2);              
  lcd.clear();   
  lcd.setCursor(0, 0);
  lcd.createChar(0, LuzFraca);
  lcd.createChar(1, LuzMedia);
  lcd.createChar(2, LuzForte);
  lcd.print("Intensidade: ");
  Serial.begin(9600); // Define a velocidade do monitor serial
  pinMode(pinoLDR, INPUT); // Define o pino que o LDR está ligado como entrada de dados
}

void loop() {
  /*lcd.setCursor(0, 0);
  lcd.autoscroll();
  delay(500);*/
  
  ValorLDR = analogRead(pinoLDR); // Faz a leitura do sensor, em um valor que pode variar de 0 a 1023
  IntensidadeLuz = map(ValorLDR, 4, 1000, 100, 0); // Converte o valor para uma escala de 0 a 100
  
  // Imprime o valor no monitor serial
  Serial.print("Intensidade de Luz 0 - 1023 = ");
  Serial.println(ValorLDR);
  Serial.print("Intensidade de Luz 0 - 100% = ");
  Serial.println(IntensidadeLuz);

    // Limpa o LCD e atualiza com a nova intensidade de luz
  lcd.setCursor(0, 1);
  lcd.print("                "); // Limpa a linha
  lcd.setCursor(0, 1);
  lcd.print(IntensidadeLuz);
  lcd.print("% ");
  
  // Atualiza o ícone no LCD baseado na intensidade
  if (IntensidadeLuz < 33) {
    lcd.write(byte(0)); // LuzFraca
  } else if (IntensidadeLuz < 66) {
    lcd.write(byte(1)); // LuzMedia
  } else {
    lcd.write(byte(2)); // LuzForte
  }
}
