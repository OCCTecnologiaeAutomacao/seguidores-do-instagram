// OCC - Tecnologia e Automacao
// Módulo contador de seguidores do instagram.
// Data de inicio - 02/02/2018
// Data de Alteracao - 
// Data de Homologacao - 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include "JsonStreamingParser.h"
#include "InstagramStats.h"
#include <WiFiClientSecure.h>
//#include <WiFiUdp.h>
//#include <EEPROM.h>
//#include <ESP8266TelegramBOT.h>
//needed for library
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
//#include <WiFiManager.h>   
String DATAINICIO = "02/02/2018";
WiFiClient  clientt;
WiFiServer server(81);
char c,BIP ='N';
LiquidCrystal_I2C lcd(0x3F,16,2);
boolean point;
char ssid[] = "SSID";
char password[] = "Sua chave de rede";
WiFiClientSecure client;
InstagramStats instaStats(client);
unsigned long delayBetweenChecks = 60000; //mean time between api requests
unsigned long whenDueToCheck = 0;
String userName = "nome do usuario no instagram"; 

void setup()
{
Serial.begin(115200);
  Serial.println("Inicializando, aguarde...");
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(20);
  lcd.setCursor(0,0);lcd.print("Aguarde...");
  lcd.setCursor(0,1);lcd.print("Inicializando...");
   WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  HORAANTERIOR = EEPROM.read(26);
  diaanterior = EEPROM.read(27);
  mesanterior = EEPROM.read(28);
  anoanterior = EEPROM.read(29);
  seg_hora=EEPROM.read(45);
  seg_dia=EEPROM.read(46);
  seg_mes=EEPROM.read(47);
  seg_anterior=EEPROM.read(48);
  seg_total=EEPROM.read(49);
  seg_ano=EEPROM.read(50);
lcd.setCursor(0,0);lcd.print("                ");
lcd.setCursor(0,1);lcd.print("                ");

}
void loop(){
lcd.setCursor(0,0);lcd.print("   Instagram     ");
unsigned long timeNow = millis();
if (timeNow > whenDueToCheck){getInstagramStatsForUser();whenDueToCheck = timeNow + delayBetweenChecks;}
}


void getInstagramStatsForUser() {
  Serial.println("Coletando os dados do usuario " + userName );
  InstagramUserStats response = instaStats.getUserStats(userName);
  Serial.print("Numero de seguidores: ");
  Serial.println(response.followedByCount);
  seg_total=response.followedByCount;
  if(seg_total>seg_anterior){seg_hora=seg_hora+1;seg_dia=seg_dia+1;seg_mes=seg_mes+1;seg_ano=seg_ano+1;seg_anterior=seg_total;
  EEPROM.write(45,seg_hora);EEPROM.write(46,seg_dia);EEPROM.write(47,seg_mes);EEPROM.write(48,seg_anterior);EEPROM.write(49,seg_total);
  EEPROM.write(50,seg_ano);EEPROM.commit();GRAVA_NUVEM();}
  Serial.println(HORAANTERIOR);Serial.println(diaanterior);Serial.println(mesanterior);Serial.println(anoanterior);Serial.println(seg_hora);Serial.println(seg_dia);Serial.println(seg_mes);
  Serial.println(seg_anterior);Serial.println(seg_total);
  lcd.setCursor(0,1);lcd.print("Seguidores ");lcd.print(seg_total);
}


