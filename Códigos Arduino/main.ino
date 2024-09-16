#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h> // Carrega a biblioteca DHT
#include <RTClib.h>
#include <EEPROM.h>

// Define a ligação ao pino de dados do sensor
#define DHTPIN 4
#define DHTTYPE DHT22 // Simulador
// #define DHTTYPE DHT11 // Bancada
#define LDR_PIN A0
#define UTC_OFFSET -3

// botao
int pushbutton = 2;

// Cria objeto RTC
RTC_DS1307 RTC;

// Cria objeto dht
DHT dht(DHTPIN, DHTTYPE);

// Define o endereço e tipo do LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variáveis para medição de luminosidade
int ValorLDR = 0; // Armazenar a leitura do sensor LDR
int IntensidadeLuz; // Transforma a leitura em uma escala de 0 a 100
int l;

// Configurações da EEPROM
const int maxRecords = 100;
const int recordSize = 8; // Tamanho de cada registro em bytes
int startAddress = 0;
int endAddress = maxRecords * recordSize;
int currentAddress = 0;

// DEFININDO AS ENTRADAS DOS LEDS
int redPin = 11;
int yellowPin = 10;
int greenPin = 9;

// buzzer
const int buzzer = 6;

// Variáveis para criação de caracteres customizados
byte Grau[8] = { 
  B01110, 
  B10001, 
  B01110, 
  B00000, 
  B00000,
  B00000, 
  B00000,
  B00000
};

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

struct DateTimeStruct {
  uint8_t day;
  uint8_t month;
  uint16_t year; // Pode ser uint8_t se quiser apenas os últimos dois dígitos
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

void setup() {
  lcd.begin(16, 2);              
  lcd.clear();
  lcd.backlight();    // Liga luz do LCD

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
  delay(2000); // Aguarda 3 segundos

  // Limpa a linha do texto para mostrar apenas os desenhos
  lcd.clear();

  // Exibe os caracteres personalizados nas posições especificadas
  lcd.setCursor(6, 0);  
  lcd.write(byte(0));
  delay(500);
  
  lcd.setCursor(7, 0);  
  lcd.write(byte(1));
  delay(500);

  lcd.setCursor(8, 0);  
  lcd.write(byte(2));
  delay(500);

  lcd.setCursor(9, 0);  
  lcd.write(byte(3));
  delay(500);

  lcd.setCursor(6, 1);  
  lcd.write(byte(4));
  delay(500);

  lcd.setCursor(7, 1);  
  lcd.write(byte(5));
  delay(500);
  
  lcd.setCursor(8, 1);  
  lcd.write(byte(6));
  delay(500);
  
  lcd.setCursor(9, 1);  
  lcd.write(byte(7));
  delay(500);

  // Exibe "DATA" e "LOGGER" centralizados
  lcd.clear();
  lcd.setCursor(6, 0); // Centraliza "DATA" na primeira linha
  lcd.print("DATA");
  
  lcd.setCursor(5, 1); // Centraliza "LOGGER" na segunda linha
  lcd.print("LOGGER");
  delay(2000); // Aguarda 3 segundos

  // Apaga tudo e exibe "carregando..." centralizado
  lcd.clear();
  lcd.setCursor(1, 0); // Centraliza "carregando..." na primeira linha
  lcd.print("Carregando...");
  delay(2000); // Aguarda 3 segundos
  
  // Limpa o display após a mensagem de carregando
  lcd.clear(); 

  // buzzer
  pinMode(buzzer,OUTPUT); 

  // botao
  pinMode(pushbutton, INPUT_PULLUP); // define o pino do botao como entrada

  // LEDs
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);


  Serial.begin(9600); // Inicializa a serial
  lcd.init();         // Inicializa LCD

  // Cria os caracteres customizados
  lcd.createChar(3, Grau);

  dht.begin();
  pinMode(LDR_PIN, INPUT); // Define o pino do LDR como entrada

  // hora atual
  RTC.begin();    // Inicialização do Relógio em Tempo Real
  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
   
}


void getNextAddress() {
    currentAddress += recordSize;
    if (currentAddress >= endAddress) {
        currentAddress = 0; // Volta para o começo se atingir o limite
    }
}


void loop() {
  mostrarGravarSensores();

  // limpa a memoria do eeprom
  int j = 0;
  if (digitalRead(pushbutton) == LOW){
    for (int j = 0; j <= currentAddress; j++){
      EEPROM.write(j, 0); // limpando
    }
    currentAddress = 0; // resetando address
    Serial.println("Limpando memória... Resetando EEPRON...\n");
    delay(4000); 
  }
}


void mostrarGravarSensores() {
  /*   DHT   */

  float h = dht.readHumidity(); // Lê o valor da umidade
  float t = dht.readTemperature(); // Lê o valor da temperatura

  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(round(t), 1);
  lcd.write(byte(3)); // Mostra o símbolo do grau
  lcd.print("C");

  lcd.setCursor(9, 0);
  lcd.print("U: ");
  lcd.print(round(h), 1);
  lcd.print("%");

  /*Serial.println("");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.println(" %");*/


  /*  LDR   */
  ValorLDR = analogRead(LDR_PIN); // Lê o valor do sensor LDR
  IntensidadeLuz = map(ValorLDR, 0, 1023, 100, 0); // Converte o valor para uma escala de 0 a 100

  lcd.setCursor(0, 1);
  lcd.print("Lumi: ");
  lcd.print(IntensidadeLuz);
  lcd.print("% ");

  /*Serial.print("Intensidade de Luz 0 - 1023 = ");
  Serial.println(ValorLDR);
  Serial.print("Intensidade de Luz 0 - 100% = ");
  Serial.println(IntensidadeLuz);*/


  /*  ALERTAS   */

  ValorLDR = analogRead(LDR_PIN); // Lê o valor do sensor LDR
  l = map(ValorLDR, 0, 1023, 100, 0); // Converte o valor para uma escala de 0 a 100

  // Condições boas
  if ((t >= 18 && t <= 22) && (h >= 36 && h <= 44) && (l >= 5 && l <= 25))
  {     
        digitalWrite(redPin, LOW); 
        digitalWrite(yellowPin, LOW); 
        digitalWrite(greenPin, HIGH);
  } 
  // Condições ruins
  else if ((t < 15 || t > 25) || (h < 30 || h > 50) || (l < 0 || l > 30)) { 
        digitalWrite(redPin, HIGH); 
        digitalWrite(yellowPin, LOW);
        digitalWrite(greenPin, LOW);

        // buzzer alerta
        tone(buzzer,5000); 
        delay(150);
        noTone(buzzer); 
  } 
  // Condições de alerta
  else {
        digitalWrite(redPin, LOW); 
        digitalWrite(yellowPin, HIGH); 
        digitalWrite(greenPin, LOW);
  }


  /*   RTC   */
  
  // Obtém a data e hora atuais
  DateTime now = RTC.now();
  int offsetSeconds = UTC_OFFSET * 3600; // Convertendo horas para segundos, se necessário
  now = now.unixtime() + offsetSeconds; // Adicionando o deslocamento ao tempo atual
  DateTime adjustedTime = DateTime(now);

  // Preenche a estrutura com os valores de data e hora
  DateTimeStruct currentTime;
  currentTime.day = adjustedTime.day();
  currentTime.month = adjustedTime.month();
  currentTime.year = adjustedTime.year();
  currentTime.hour = adjustedTime.hour();
  currentTime.minute = adjustedTime.minute();
  currentTime.second = adjustedTime.second();


  /*   EEPROM   */
  int secondAsInt = adjustedTime.second();
  
  if (secondAsInt % 5 == 0)  // Gravando os dados a cada 5s
  {

    delay(1000);  // delay para poder gravar apenas 1 registro por segundo

    // gravando na memória
    EEPROM.write(currentAddress, t);
    currentAddress += 1;
    EEPROM.write(currentAddress, h);
    currentAddress += 1;
    EEPROM.write(currentAddress, IntensidadeLuz);
    currentAddress += 1;
    EEPROM.put(currentAddress, currentTime);
    currentAddress += 1;
    
    
    // regastando últimos registros da memória
    int i = 0;

    Serial.print("Dados: ");
    
    for (int i = currentAddress - 4; i < currentAddress; i++) {
        DateTimeStruct storedTime;

        if (i == currentAddress - 4) {
            Serial.print(EEPROM.read(i)); // lendo dados de temp do eeprom
            Serial.print("°C/t ");
        }
        else if (i == currentAddress - 3) {
            Serial.print(EEPROM.read(i)); // lendo dados de umi do eeprom
            Serial.print("%/u ");
        }
        else if (i == currentAddress - 2) {
            Serial.print(EEPROM.read(i)); // lendo dados de lumi do eeprom
            Serial.print("%/l ");
        }
        else if (i == currentAddress - 1) {
              EEPROM.get(i, storedTime); // lendo dados de hora do eeprom
              // Exibe os valores lidos
              Serial.print("| Data e Hora: ");
              Serial.print(storedTime.day);
              Serial.print("/");
              Serial.print(storedTime.month);
              Serial.print("/");
              Serial.print(storedTime.year);
              Serial.print(" ");
              Serial.print(storedTime.hour);
              Serial.print(":");
              Serial.print(storedTime.minute);
              Serial.print(":");
              Serial.println(storedTime.second);
        }
    }

    getNextAddress();
    Serial.println("");

  }
}
