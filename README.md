**Manual Técnico:**

**Descrição**
Este projeto utiliza um Arduino como unidade de controle principal para monitorar parâmetros ambientais, como temperatura, umidade e intensidade luminosa. Os sensores conectados ao Arduino realizam as medições, com os dados sendo exibidos em um display LCD e armazenados em uma memória EEPROM para consulta futura. Além disso, os LEDs fornecem feedback visual sobre se os valores estão dentro dos limites desejados, enquanto o RTC (Relógio de Tempo Real) registra o horário de cada medição.

**Componentes Utilizados**
Arduino – Microcontrolador responsável pelo controle do circuito.
DHT11 – Sensor de temperatura e umidade.
LEDs – Indicadores para sinalizar se os valores obtidos estão dentro dos parâmetros ideais.
LDR – Sensor de intensidade luminosa.
RTC – Relógio em tempo real, mantém a hora e a data atualizadas mesmo com o sistema desligado.
EEPROM – Memória para armazenar dados de temperatura, umidade e luminosidade.
Resistores – Limitam a corrente para os LEDs e o LDR.
Protoboard e Jumpers – Utilizados para realizar as conexões.
Bateria (Fonte de Alimentação) – Fonte de 5V para alimentar o Arduino e os componentes.
Botão Reset – Responsável por resetar a memória da EEPROM.
LCD I2C – Display LCD para exibir as informações fornecidas pelo código.
Buzzer – Emite um som de alerta quando os dados estão fora dos padrões predefinidos.
Especificações
Níveis dos Triggers (Gatilhos) Adotados:

Faixa de Temperatura: 15 °C < t < 25 °C
Faixa de Luminosidade: 0% < l < 30%
Faixa de Umidade: 30% < u < 50%

**Explicação do Código**
As principais bibliotecas utilizadas foram:

LiquidCrystal_I2C: Controla o display LCD.
Wire: Gerencia a comunicação I2C.
DHT: Interage com o sensor DHT11 para medir temperatura e umidade.
RTClib: Permite o uso de um módulo RTC.
EEPROM: Facilita a gravação e leitura de dados na memória EEPROM.
Lógica do Código
A função principal, mostrarGravarSensores(), executa continuamente as seguintes tarefas:

**Leitura dos Sensores:**

Obtém temperatura e umidade do sensor DHT11.
Mede a luminosidade via o sensor LDR e converte o valor para uma escala de 0 a 100%.
Exibição dos Dados:

Exibe os valores de temperatura, umidade e luminosidade no display LCD.

**Gerenciamento de Alertas:**

Aciona LEDs e o buzzer com base nas condições:
LED verde: Condições ótimas (18-22 °C, 36-44% de umidade, 5-25% de luminosidade).
LED amarelo: Condições intermediárias.
LED vermelho: Condições fora dos limites.

**Controle do RTC:**

Obtém a hora atual e a exibe.
Armazenamento de Dados na EEPROM:

A cada 5 segundos, grava temperatura, umidade, luminosidade e horário na EEPROM.
Lê e exibe os últimos registros gravados na EEPROM pela interface serial.

**Diferenciais:**

- A EEPROM armazena os dados de forma circular: quando a memória atinge seu limite, os novos dados sobrescrevem os mais antigos;
- Botão de reset da memória EEPROM;
- Carregamento visual do sistema.

**Conclusão**
Este projeto oferece uma solução eficiente para monitoramento e registro de dados ambientais, como temperatura, umidade e luminosidade, utilizando o RTC para marcação precisa do tempo. Os dados coletados são armazenados na EEPROM, permitindo consultas futuras. Os LEDs facilitam a visualização rápida dos resultados, proporcionando uma interface interativa que sinaliza se os parâmetros estão dentro dos limites desejados.

