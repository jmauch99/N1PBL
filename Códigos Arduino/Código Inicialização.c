#include <LiquidCrystal.h>

// Inicializa a biblioteca com os números dos pinos a serem utilizados no Arduino
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

// Define os caracteres personalizados
byte char1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
};

byte char2[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B11111
};

byte char3[8] = {
  B00000,
  B10000,
  B01000,
  B00100,
  B11110,
  B00000,
  B00000,
  B11111
};

byte char4[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
};

byte char5[8] = {
  B10000,
  B10000,
  B10000,
  B01000,
  B00100,
  B00111,
  B00000,
  B00000
};

byte char6[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000
};

byte char7[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000
};

byte char8[8] = {
  B00001,
  B00001,
  B00001,
  B00010,
  B00100,
  B11100,
  B00000,
  B00000,
};

void setup() {
  lcd.begin(16, 2);              
  lcd.clear();                  

  // Cria os caracteres personalizados
  lcd.createChar(0, char1);
  lcd.createChar(1, char2);
  lcd.createChar(2, char3);
  lcd.createChar(3, char4);
  lcd.createChar(4, char5);
  lcd.createChar(5, char6);
  lcd.createChar(6, char7);
  lcd.createChar(7, char8);

  // Exibe o texto "EM(BARCADOS)" na primeira linha
  lcd.setCursor(2, 0); // Centraliza o texto na primeira linha
  lcd.print("EM(BARCADOS)");    
  delay(3000); // Aguarda 3 segundos

  // Limpa a linha do texto para mostrar apenas os desenhos
  lcd.clear();

  // Exibe os caracteres personalizados nas posições especificadas
  lcd.setCursor(6, 0);  
  lcd.write(byte(0));
  delay(1500);
  
  lcd.setCursor(7, 0);  
  lcd.write(byte(1));
  delay(1500);

  lcd.setCursor(8, 0);  
  lcd.write(byte(2));
  delay(1500);

  lcd.setCursor(9, 0);  
  lcd.write(byte(3));
  delay(1500);

  lcd.setCursor(6, 1);  
  lcd.write(byte(4));
  delay(1500);

  lcd.setCursor(7, 1);  
  lcd.write(byte(5));
  delay(1500);
  
  lcd.setCursor(8, 1);  
  lcd.write(byte(6));
  delay(1500);
  
  lcd.setCursor(9, 1);  
  lcd.write(byte(7));
  delay(1500);

  // Exibe "DATA" e "LOGGER" centralizados
  lcd.clear();
  lcd.setCursor(6, 0); // Centraliza "DATA" na primeira linha
  lcd.print("DATA");
  
  lcd.setCursor(5, 1); // Centraliza "LOGGER" na segunda linha
  lcd.print("LOGGER");
  delay(3000); // Aguarda 3 segundos

  // Apaga tudo e exibe "carregando..." centralizado
  lcd.clear();
  lcd.setCursor(1, 0); // Centraliza "carregando..." na primeira linha
  lcd.print("Carregando...");
  delay(10000); // Aguarda 10 segundos

  // Limpa o display após a mensagem de carregando
  lcd.clear(); 
}

void loop() {
  // Não faz nada no loop, pois o código só deve rodar uma vez.
}
