#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const int pinoEcho = 2;
const int pinoTrigger = 4;

long duracao;
float distancia;

const int RXPin = 3;
const int TXPin = 8;
const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

unsigned long retrasoPassado = 0;
const long intervalo = 2000; 

bool gpsJaDeuSinalDeVida = false;

RF24 radio(9, 10);

const byte endereco[] = "00001";

struct __attribute__((__packed__)) DadosPacote {
  int idSensor;
  float leitura;
};

DadosPacote telemetria;

void setup() {
  pinMode(pinoEcho, INPUT);   
  pinMode(pinoTrigger, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT); 

  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  Serial.println("Inicializando gps... Aguarde os primeiros dados.");
  
  if (!radio.begin()) {
    Serial.println("Falha no hardware! Verifique as ligacoes ou ligue o VCC.");
    while (1); 
  }
  
  radio.openWritingPipe(endereco);
  
  radio.setPALevel(RF24_PA_MIN); 
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  
  telemetria.idSensor = 101;
}

void loop() {

  digitalWrite(pinoTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrigger, LOW);

  duracao = pulseIn(pinoEcho, HIGH);
  distancia = duracao * 0.034 / 2;

  telemetria.leitura = distancia;

  Serial.println(telemetria.leitura);

  gps.encode(gpsSerial.read());
  verificarStatusGPS();

  bool sucesso = radio.write(&telemetria, sizeof(telemetria));

  Led();

  delay(1500); 
}

void Led() {
  if(distancia < 10){
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }
  if(distancia < 20 && distancia > 10){
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  }
  if(distancia > 20){
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  }
}

void verificarStatusGPS() {
  
  if (gps.location.isValid()) {
    Serial.println(gps.location.lat(), 6);
    Serial.println(gps.location.lng(), 6);
  } else{
    Serial.println("0.000000");
    Serial.println("0.000000");
  }
}