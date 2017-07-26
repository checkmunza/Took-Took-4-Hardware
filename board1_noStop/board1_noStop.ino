//8232b
//Exceed019

//////////////  For Send and Recieve
#include <UnoWiFiDevEd.h>
#define CONNECTOR "rest"
#define SERVER_ADDR "158.108.165.223"
CiaoData data;
/////////////

#define HUMAN_ALCOHOL 90
#define REF_ALCOHOL 130
#define REF_GAS 100

///     PINOUT
#define GAS_PIN A0
#define ALCOHOL_PIN A1
#define SPEAKER_PIN 11
#define ENGINE_PIN 12
#define START_PIN 13
#define ALERT_PIN 7


bool carStatus = false;
int alcoholLevel = 100;

int tmp;

void setup() {
  pinMode(ALCOHOL_PIN, INPUT);
  pinMode(ENGINE_PIN, OUTPUT);
  pinMode(START_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(SPEAKER_PIN, OUTPUT);

  Ciao.begin();///
  Serial.begin(9600);

  digitalWrite(ENGINE_PIN, HIGH);
  delay(100);
  digitalWrite(ENGINE_PIN, LOW);
  delay(100);
  digitalWrite(ENGINE_PIN, HIGH);
  delay(100);
  digitalWrite(ENGINE_PIN, LOW);
}

void loop() {

  Serial.println("Eng");
  ///     Engine Checker
  if(digitalRead(START_PIN) == LOW)
  {
    carStatus = !carStatus;
    if(!carStatus)
    {
      digitalWrite(ENGINE_PIN, LOW);
    }
    while(digitalRead(START_PIN) == LOW)
    {
    }
    digitalWrite(ENGINE_PIN, HIGH);
    delay(100);
    digitalWrite(ENGINE_PIN, LOW);
  }
  ///     STOP****
  if (carStatus)
  {
    
    if(getData("TUK40/stop")=="1")
    {
      carStatus = false;
    }
  ////
    alcoholLevel = analogRead(ALCOHOL_PIN);
   // Serial.println("Alcohol Level = " + String(alcoholLevel));
    sendData(String(alcoholLevel), "TUK40/alcohol");
    if(alcoholLevel >= REF_ALCOHOL)
    {
      digitalWrite(ENGINE_PIN, LOW);
    }
    else if (alcoholLevel >= HUMAN_ALCOHOL )
    {
      digitalWrite(ENGINE_PIN, HIGH);
    }
  }
  else
  {
    digitalWrite(ENGINE_PIN, LOW);
  }
 // Serial.println("carStatus = " + String(carStatus));
  ///
  Serial.println("GAS");
  ///     GAS
  tmp = analogRead(GAS_PIN);
//  Serial.println("GAS = " + String(tmp));
  if(analogRead(GAS_PIN) > REF_GAS)
  {
    analogWrite(SPEAKER_PIN, 150);
  }
  else
  {
    analogWrite(SPEAKER_PIN, 0);
  }
  sendData(String(tmp), "TUK40/gas");
  ///
  Serial.println("ALERT");
  ///   ALERT LED
  if(getData("TUK40/alert") == "1")
  {
    digitalWrite(ALERT_PIN, HIGH);
  }
  else
  {
    digitalWrite(ALERT_PIN, LOW);
  }
  ///
  
  delay(500);
}

void sendData(String value, String variable)
{
  value = "/data/" + variable + "/set/" + value;
  data = Ciao.write(CONNECTOR, SERVER_ADDR, value);
  //Serial.println(value);
}


String getData(String variable)
{
  data = Ciao.read(CONNECTOR, SERVER_ADDR, "/data/" + variable);
  return String(data.get(2));
}

