#define BLYNK_TEMPLATE_ID "TMPL66CulTSLS"
#define BLYNK_TEMPLATE_NAME "pemantauan kesehata "
#define BLYNK_AUTH_TOKEN "ub4z49p6zc5J2ldxHW5mW_CIBOJvWGvV"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h> 
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include "heartRate.h"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "asik1";     
char pass[] = "12345678"; 

TwoWire I2C_Suhu = TwoWire(1); 

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MAX30105 particleSensor;

#define I2C_SDA_1 22
#define I2C_SCL_1 21

#define I2C_SDA_2 4
#define I2C_SCL_2 15

double avered = 0, aveir = 0;
double sumirrms = 0, sumredrms = 0;
int i = 0;
int Num = 500;        
float ESpO2 = 0.0;    
double FSpO2 = 0.95;  
double frate = 0.95; 
const float CALIB_A = 110.0; 
const float CALIB_B = 18.0; 
#define FINGER_ON 30000 

const byte RATE_SIZE = 6; 
byte rates[RATE_SIZE]; 
byte rateSpot = 0;
long lastBeat = 0;
float beatAvg = 0;

unsigned long lastSuhuCheck = 0;
const int intervalSuhu = 1000; 

void setup() {
  Serial.begin(115200);
  
  Blynk.begin(auth, ssid, pass);
  
  Wire.begin(I2C_SDA_1, I2C_SCL_1, 100000); 
  
  I2C_Suhu.begin(I2C_SDA_2, I2C_SCL_2, 100000);

  if (!mlx.begin(0x5A, &I2C_Suhu)) {
    Serial.println("MLX90614 TIDAK TERDETEKSI di Pin 4 & 15!");
  }

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 TIDAK TERDETEKSI di Pin 22 & 21!");
  }

  byte ledBrightness = 0x2F; 
  byte sampleAverage = 1;    
  byte ledMode = 2;      
  int sampleRate = 400;   
  int pulseWidth = 411;   
  int adcRange = 16384; 
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
}

void loop() {
  Blynk.run(); 

  uint32_t ir, red;
  double fred, fir;

  particleSensor.check();                

  while (particleSensor.available()) {  
    red = particleSensor.getFIFORed(); 
    ir  = particleSensor.getFIFOIR();  

    if (ir < FINGER_ON) {
      beatAvg = 0;
      ESpO2 = 0.0; 
      lastBeat = millis(); 
    } 
    else {
      i++;
      fred = (double)red;
      fir  = (double)ir;
      avered = avered * frate + fred * (1.0 - frate);
      aveir  = aveir * frate + fir * (1.0 - frate);
      sumredrms += (fred - avered) * (fred - avered);
      sumirrms  += (fir - aveir) * (fir - aveir);

      if ((i % Num) == 0) {
        double R = (sqrt(sumredrms) / avered) / (sqrt(sumirrms) / aveir);
        double SpO2 = CALIB_A - (CALIB_B * R);
        if (SpO2 > 100) SpO2 = 100;
        if (SpO2 > 85) { 
           if (ESpO2 < 1.0) ESpO2 = SpO2; 
           else ESpO2 = FSpO2 * ESpO2 + (1.0 - FSpO2) * SpO2;
        }
        sumredrms = 0.0; sumirrms = 0.0; i = 0;
        
        Blynk.virtualWrite(V3, ESpO2);
      }

      if (checkForBeat(ir) == true) {
        long delta = millis() - lastBeat;
        lastBeat = millis();
        float beatsPerMinute = 60 / (delta / 1000.0);
        if (beatsPerMinute < 160 && beatsPerMinute > 40) {
          rates[rateSpot++] = (byte)beatsPerMinute;
          rateSpot %= RATE_SIZE;
          beatAvg = 0;
          for (byte x = 0 ; x < RATE_SIZE ; x++) beatAvg += rates[x];
          beatAvg /= RATE_SIZE;
          
          Blynk.virtualWrite(V2, beatAvg);
        }
      }
    }
    particleSensor.nextSample(); 
  }

  if (millis() - lastSuhuCheck > intervalSuhu) {
    float suhuMentah = mlx.readObjectTempC();
    float hasilTampil = 0;
    String kondisi = "";

    if (isnan(suhuMentah)) {
      hasilTampil = 0.0;
      kondisi = "Error Jalur I2C_Suhu";
    }
    else if (suhuMentah < 34.0) {
      hasilTampil = 0.0;
      kondisi = "Menunggu Objek...";
    } 
    else {
      hasilTampil = suhuMentah;
      if (hasilTampil <= 37.5) kondisi = "SEHAT (NORMAL)";
      else if (hasilTampil <= 38.9) kondisi = "SAKIT (DEMAM)";
      else kondisi = "DEMAM TINGGI";
    }

    Serial.println("===============================");
    Serial.print("SUHU TUBUH : ");
    Serial.print(hasilTampil, 1); 
    Serial.println(" °C");
    Serial.print("KONDISI    : ");
    Serial.println(kondisi);
    Serial.print("BPM        : ");
    Serial.print(beatAvg, 0);
    Serial.print(" | SpO2: ");
    Serial.print(ESpO2, 1);
    Serial.println("%");
    Serial.println("===============================");

    Blynk.virtualWrite(V1, hasilTampil); 
    Blynk.virtualWrite(V4, kondisi);     
    
    Blynk.virtualWrite(V2, beatAvg);
    Blynk.virtualWrite(V3, ESpO2);

    lastSuhuCheck = millis();
  }
}