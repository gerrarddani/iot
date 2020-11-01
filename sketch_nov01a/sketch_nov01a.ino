#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4         
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);

#define BLYNK_YELLOW "#c1c423"
#define BLYNK_BLUE   "#1d23c2"

char auth[] = "z25DCzZkKYL2OholB_RXWIz7UDX-Qa3W";
char ssid[] = "Si cantik";
char pass[] = "saladbuah";

int PLampuCahaya = 25;
int PinLampuSuhu = 43;
int PPot = 34;
int suhu, cahaya, akhirSuhu, akhirCahaya;
int statusSistem = 1;

WidgetLED LampuCahaya(V6);
WidgetLED LampuSuhu(V7);

void warnaLedCahaya()
{
  if(cahaya <= batasCahaya){ 
    LampuCahaya.setColor(BLYNK_YELLOW); 
    Serial.println("Lampu Indikator Cahaya: Kuning");
    digitalWrite(PLampuCahaya, LOW);
  }else{
    LampuCahaya.setColor(BLYNK_BLUE);
    Serial.println("Lampu Indikator Cahaya: Biru");
    digitalWrite(PLampuCahaya, HIGH);
  }
}

void warnaLedSuhu()
  if(suhu <= akhirSuhu){ 
    LampuSuhu.setColor(BLYNK_YELLOW); 
    Serial.println("Lampu Indikator Suhu: Kuning");
    digitalWrite(PLampuSuhu, LOW);
  }else{
    LampuSuhu.setColor(BLYNK_BLUE);
    Serial.println("Lampu Indikator Suhu: Biru");
    digitalWrite(PLampuSuhu, HIGH);
  } 
}

void mengirimSensorCahaya()
{
  delay(10000);
  cahaya = analogRead(pinPot);    
  int lux = 0.009768*analogRead(pinPot)+10;   
  if (isnan(lux)) {
    Serial.println("Gagal membaca LDR sensor!");
    return;
  }else{
    Blynk.virtualWrite(V0, lux);    
    Blynk.virtualWrite(V2, lux);  
  }  
}

void mengirimSensorSuhu()
{
  delay(10000);
  int temp = dht.readTemperature(); 
  suhu = temp;
  if (isnan(temp)) {
    Serial.println("Gagal membaca DHT sensor!");
    return;
  }else{
    Blynk.virtualWrite(V1, temp); 
    Blynk.virtualWrite(V3, temp);  
  }
  
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(PLampuCahaya, OUTPUT);
  pinMode(PlampuSuhu, OUTPUT);
  dht.begin();
  Blynk.begin(auth, ssid, pass, IPAddress(192.168.43.99), 9443);
  LampuCahaya.on();
  LampuSuhu.on();
}

void loop()
{
  Blynk.run();
  if(statusSistem == 1){
    mengirimSensorCahaya();
    mengirimSensorSuhu();
    warnaLedCahaya();
    warnaLedSuhu();
  }
}

BLYNK_WRITE(V4)
{
  int nilaiPin = param.asInt(); 
  akhirCahaya = nilaiPin;
  Serial.print("Nilai TH Cahaya: ");
  Serial.println(nilaiPin);
}

BLYNK_WRITE(V5)
{
  int nilaiPin = param.asInt(); 
  batasSuhu = nilaiPin;
  Serial.print("Nilai TH Suhu: ");
  Serial.println(nilaiPin);
}

BLYNK_WRITE(V8)
{
  int pinValue = param.asInt(); 
  if(pinValue == 1){            
    Serial.println("Sistem Menyala");
    statusSistem = 1;
  }else{                        
    Serial.println("Sistem Dimatikan");
    statusSistem = 0;
    LampuCahaya.off();
    LampuSuhu.off();
    digitalWrite(PLampuSuhu, LOW); 
    digitalWrite(PLampuCahaya, LOW); 
  }
}
