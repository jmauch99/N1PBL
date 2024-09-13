#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h> // Carrega a biblioteca DHT

// Define a ligação ao pino de dados do sensor
#define DHTPIN 4
#define DHTTYPE DHT22
#define LDR_PIN A0

// Cria objeto dht
DHT dht(DHTPIN, DHTTYPE);

// Define o endereço e tipo do LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variáveis para medição de luminosidade
int ValorLDR = 0; // Armazenar a leitura do sensor LDR
int IntensidadeLuz; // Transforma a leitura em uma escala de 0 a 100
int l;


// DEFININDO AS ENTRADAS DOS LEDS
int redPin = 11;
int yellowPin = 10;
int greenPin = 9;


// Variáveis para criação de caracteres customizados
byte LuzFraca[8] = { 
  B00000, 
  B10001, 
  B00000, 
  B00000, 
  B11111, 
  B10001, 
  B00000 
};

byte LuzMedia[8] = { 
  B00000, 
  B10001, 
  B00000, 
  B00000, 
  B11111, 
  B00000, 
  B00000 
};

byte LuzForte[8] = { 
  B00000, 
  B10001, 
  B00000, 
  B10001, 
  B11111, 
  B00000, 
  B00000 
};

// Intervalos de tempo para alternar entre as informações (em milissegundos)
const unsigned long intervaloTempUmidade = 10000; // 10 segundos
const unsigned long intervaloLuminosidade = 10000; // 10 segundos

// Variáveis para controle de tempo
unsigned long tempoAnterior = 0;
unsigned long tempoAtual = 0;
int telaAtual = 0;

void setup() {
  // LEDs
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);


  Serial.begin(9600); // Inicializa a serial
  lcd.init();         // Inicializa LCD
  lcd.backlight();    // Liga luz do LCD

  // Cria os caracteres customizados
  lcd.createChar(0, LuzFraca);
  lcd.createChar(1, LuzMedia);
  lcd.createChar(2, LuzForte);

  dht.begin();
  pinMode(LDR_PIN, INPUT); // Define o pino do LDR como entrada
  tempoAnterior = millis(); // Inicializa o tempo
}

void loop() {
  /* debugando leds
  digitalWrite(redPin, HIGH); 
  digitalWrite(yellowPin, HIGH); 
  digitalWrite(greenPin, HIGH);*/ 

  tempoAtual = millis();

  if (tempoAtual - tempoAnterior >= intervaloTempUmidade && telaAtual == 0) {
    mostrarTemperaturaEUmidade();
    telaAtual = 1;
    tempoAnterior = tempoAtual;
  } 
  else if (tempoAtual - tempoAnterior >= intervaloLuminosidade && telaAtual == 1) {
    mostrarLuminosidade();
    telaAtual = 0;
    tempoAnterior = tempoAtual;
  }




}

void mostrarTemperaturaEUmidade() {
  float h = dht.readHumidity(); // Lê o valor da umidade
  float t = dht.readTemperature(); // Lê o valor da temperatura

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t, 1);
  lcd.write((byte)0); // Mostra o símbolo do grau
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Umid: ");
  lcd.print(h, 1);
  lcd.print("%");

  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.println(" %");

  ValorLDR = analogRead(LDR_PIN); // Lê o valor do sensor LDR
  l = map(ValorLDR, 0, 1023, 100, 0); // Converte o valor para uma escala de 0 a 100

  // Condições boas
  if ((t >= 18 && t <= 22) && (h >= 36 && h <= 44) && (l >= 5 && l <= 25))
  {
        digitalWrite(redPin, LOW); 
        digitalWrite(yellowPin, LOW); 
        digitalWrite(greenPin, HIGH);
  } 
  else if ((t >= 15 && t < 18 || t > 22 && t <= 25) && (h >= 30 && h < 36 || h > 44 && h <= 50) && (l >=0 && l < 5 || l > 25 && l <= 30)) { 
        digitalWrite(redPin, LOW); 
        digitalWrite(yellowPin, HIGH); 
        digitalWrite(greenPin, LOW);
  } 
  else {
        digitalWrite(redPin, HIGH); 
        digitalWrite(yellowPin, LOW); 
        digitalWrite(greenPin, LOW);
  }

}

void mostrarLuminosidade() {
  ValorLDR = analogRead(LDR_PIN); // Lê o valor do sensor LDR
  IntensidadeLuz = map(ValorLDR, 0, 1023, 100, 0); // Converte o valor para uma escala de 0 a 100

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luminosidade:");
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

  Serial.print("Intensidade de Luz 0 - 1023 = ");
  Serial.println(ValorLDR);
  Serial.print("Intensidade de Luz 0 - 100% = ");
  Serial.println(IntensidadeLuz);
}



