#include <WiFiS3.h>

/*
  Control básico de un carrito por Wi-Fi usando Arduino Uno R4

  Este programa convierte al Arduino en un dispositovo capaz de 
  recibir comandos desde un cliente externo, por ejemplo MATLAB,
  para controlar el movimiento de un carrito de dos motores.

  Comandos admitidos:
    'F' o 'f' -> mover hacia adelante durante 5 segundos
    'B' o 'b' -> mover hacia atrás durante 5 segundos
    'S' o 's' -> detener motores

  :
  - Este código asume un driver de motor tipo puente H
    conectado a cuatro pines digitales.
*/


char ssid[] = "nombre_de_tu_red"; // mi red de wifi
char pass[] = "contraseña_de_tu_red"; // mi contraseña de wifi

int status = WL_IDLE_STATUS;
WiFiServer server(5000);   // Puerto TCP 

// Pines de los motores
const int IN1 = 8;
const int IN2 = 7;
const int IN3 = 12;
const int IN4 = 11;

void setup() {
  Serial.begin(9600); 

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pararMotores();

  Serial.println("Iniciando WiFi...");

  // Conectarse a la red Wi-Fi
  while (status != WL_CONNECTED) {
    Serial.print("Conectando a ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(2000);
  }

  Serial.println("WiFi conectado.");
  Serial.print("IP del Arduino: ");
  Serial.println(WiFi.localIP()); // IP del Arduino

  // Iniciar servidor
  server.begin();
  Serial.println("Servidor TCP iniciado en el puerto 5000.");
}

void loop() {
  // Esperar cliente desde MATLAB
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Cliente conectado.");

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        Serial.print("Recibido: ");
        Serial.println(c);

        switch (c) {
          case 'F':
          case 'f':
            moverAdelante();
            Serial.println("Adelante 5 segundos");
            delay(5000);
            pararMotores();
            break;

          case 'B':
          case 'b':
            moverAtras();
            Serial.println("Atras 5 segundos");
            delay(5000);
            pararMotores();
            break;

          case 'S':
          case 's':
            pararMotores();
            Serial.println("Stop");
            break;
        }
      }
    }

    client.stop();
    Serial.println("Cliente desconectado.");
  }
}

// Funciones de movimiento 

void moverAdelante() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moverAtras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void pararMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
