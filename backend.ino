#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#define pino_umidade 12
#define pino_gas 2
#define pino_pir 3
#define pino_led 10


const char* ssid = ""; //Nome do Wifi
const char* password = ""; //Senha do wifi

ESP8266WebServer server(80); //porta

void hadleRoot() {

    // Para retornar HTML
    // String ConteudoHTML;
    // ConteudoHTML = "Sensor de umidade: ";
    // ConteudoHTML += analogRead(pino_umidade);
    // ConteudoHTML +="\n";
    // ConteudoHTML += "Sensor de gas: ";
    // ConteudoHTML += analogRead(pino_gas);
    // ConteudoHTML +="\n";
    // ConteudoHTML += "Sensor de pir: ";
    // ConteudoHTML += analogRead(pinor_pir);

    String response = "{";
 
    response+= "\"Umidade\": \""+analogRead(pino_umidade)+"\"";
    response+= ",\"Gas\": \""+leitura_gas()+"\"";
    response+= ",\"Pir\": \""+analogRead(pinor_pir)+"\"";
    response+="}";
    // Para retornar HTML
    // server.send(200, "text/html", ConteudoHTML);

    server.send(200, "text/json", response);
}

void handleNotFound(){

  String message = "Não encontrado\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}

void setup(void){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(pino_led, OUTPUT);
  pinMode(pino_pir, INPUT);
  Serial.println("");

  // Esperando se conectar
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  // para adicionar mais caminhos no web sever 
  // server.on("/led", [](){
  // server.send(200, "text/plain", "this works as well");
  // });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
  int presenca = analogRead(pinor_pir);
  if presenca == 0 {
    digitalWrite(pino_led, HIGH);
  } else {
      delay(5000); // 5 segundos para nao apagar instantaneamente
      digitalWrite(pino_led, LOW);
    }
    
}

int leitura_gas()
{
  int sinalGas = analogRead(pino_gas);
  sinalGas = map(sinalGas, 300, 750, 0, 100);
  return sinalGas;

}

