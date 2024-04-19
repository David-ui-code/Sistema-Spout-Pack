#include <SPI.h>
#include <Ethernet.h>
#include <WebSocketServer.h>
#include <LiquidCrystal.h>
 
 
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
 
 
int PinSensor = 8;
int VarSensor = 0;
int PinBuzzer = 9;

// Defina o endereço IP e a porta do servidor
IPAddress ip(192, 168, 0, 106); //* 192, 168, 1, 177
EthernetServer server(80);

void setup() {
  Serial.begin(9600);

  // Inicialize a conexão Ethernet e comece a escutar na porta 80
  Ethernet.begin(mac, ip);
  server.begin();
}
{
 
 lcd.begin(16, 2);
 lcd.setCursor( 0, 0);
 lcd.print(" Alarme por ");
 lcd.setCursor( 0, 1);
 lcd.print("Movimento d'agua");
 delay(3000);
 lcd.clear();
}

void loop() {
  EthernetClient client = server.available();

  if (client) {
    while (client.connected()) {
      if (client.available()) {
        // Ler os dados do cliente (não usado neste exemplo)
        client.readStringUntil('\r\n');

        // Enviar a página HTML para o cliente
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Motor Current Monitor</title></head><body><h1>Motor Current Monitor</h1><p>Current Reading: <span id=\"currentReading\">Waiting for data...</span> mA</p><script>const socket=new WebSocket('ws://"+Ethernet.localIP().toString()+":8888');socket.addEventListener('open',function(event){console.log('Connected to Arduino');});socket.addEventListener('message',function(event){document.getElementById('currentReading').textContent=event.data+' mA';});socket.addEventListener('close',function(event){console.log('Disconnected from Arduino');});</script></body></html>");

        // Ler o valor do sensor e enviá-lo para o cliente através do WebSocket
        unsigned int x = 0;
        float AcsValue = 0.0, Samples = 0.0, AvgAcs = 0.0, AcsValueF = 0.0;
        for (int x = 0; x < 10; x++) {
          AcsValue = analogRead(A0);
          Samples = Samples + AcsValue;
          delay(3);
        }
        AvgAcs = Samples / 10.0;
        float voltage = AvgAcs * (5.0 / 1024.0);
        AcsValueF = (2.5 - voltage) * 1000 / 0.185;
        client.println(AcsValueF);

        // Aguardar um curto período de tempo antes de fechar a conexão
        delay(100);

        // Fechar a conexão
        client.stop();
      }
    }
  }
  {
 VarSensor = digitalRead(PinSensor);
 lcd.setCursor( 0, 0);
 lcd.print("Agua sem ");
 lcd.setCursor( 0, 1);
 lcd.print("movimento");
 
 if (VarSensor == 1)
 {
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Atençao!!!");
 lcd.setCursor(0, 1);
 lcd.print("Movimento n'Agua");
 digitalWrite(PinBuzzer, HIGH);
 delay(1000);
 digitalWrite(PinBuzzer, LOW);
 }
 
 else
 {
 digitalWrite(PinBuzzer, LOW);
 }
 delay(500);
}
}