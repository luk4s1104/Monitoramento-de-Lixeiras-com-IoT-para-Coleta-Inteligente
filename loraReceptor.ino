#include <SPI.h> // biblioteca para configurar o lora
#include <nRF24L01.h> // biblioteca para configurar o lora
#include <RF24.h> // biblioteca para configurar o lora

RF24 radio(9, 10); // define os pinos do LoRa ligados ao arduino

const byte endereco[] = "00001"; // define o mesmo endereco(00001) que definimos no lora transmissor

struct __attribute__((__packed__)) DadosPacote { // recebe os dados do sensor por meio do envio do pacote de dados do lora transmissor, mesma coisa que colocamos no transmissor
  int idSensor;
  float leitura;
};

DadosPacote dadosRecebidos; // cria um espaco vazio para receber os dados do lora transmissor

void setup() {
  Serial.begin(9600); // define a taxa de transmissao(9600 bits por segundo), é a mesma taxa 
  
  if (!radio.begin()) { // aqui estamos tentando saber se o lora esta funcionando, mesma coisa de antes do lora transmissor
    Serial.println("Falha no hardware! Verifique as ligacoes ou ligue o VCC."); // meensagem de erro 
    while (1); // ele repete esse loop ate o problema ser resolvido
  }
  
  radio.openReadingPipe(1, endereco); // Configura o rádio para receber as mensagens vindas do endereço "00001"
  
  // Mesma configuração de potência mínima e velocidade do transmissor
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  
  // Inicia o modo de recebimento de dados
  radio.startListening();
  
  Serial.println("Receptor Inicializado! Aguardando dados do transmissor..."); // mensagem que mostra que o lora esta apto a receber dados
}

void loop() {
  if (radio.available()) { // verifica se o radio recebeu algum dado
    
    radio.read(&dadosRecebidos, sizeof(dadosRecebidos));  // lê os dados armazenados e guarda naquele espaco vazio criado anteriormente
    
    Serial.println(dadosRecebidos.leitura); // mostra o valor do sensor que chegou pelo rádio

  }
}