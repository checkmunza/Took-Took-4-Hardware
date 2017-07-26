//////////////  For Send and Recieve
#include <UnoWiFiDevEd.h>
#define CONNECTOR "rest"
#define SERVER_ADDR "158.108.165.223"
CiaoData data;
/////////////

#define ROOF_TIME 3000
#define CLOSE_SERVO 170

#define MOTOR_A1_PIN 2
#define MOTOR_A2_PIN 3
#define RAIN_PIN A0

#define SWITCH_1_PIN 12
#define SWITCH_2_PIN 13

int isManual = 1;
int roofStatus = 0;

String roofControl;

int switch1 = 0;
int switch2 = 0;
int count;
void setup() {
  Serial.begin(9600);
  Ciao.begin();

  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_A2_PIN, OUTPUT);

  pinMode(12, INPUT);
  pinMode(13, INPUT);
  count = 0;
  sendData("0", "TUK40/people");
  Serial.println("Start");
}

void loop() {
  /*
    ///     Mode Status
    if (getData("/TUK40/mode") == "Auto")
    {
    isManual = 0;
    }
    else
    {
    isManual = 1;
    }
    Serial.println("Mode = " + String(isManual));
    ///
  */
  Serial.println("P");
  ///     count People
  int s1 = digitalRead(SWITCH_1_PIN);
  int s2 = digitalRead(SWITCH_2_PIN);
  if (s1 == 0)
  {
    Serial.println("S1");
    if (switch2 == 1)
    {
      count--;
      sendData(String(count), "TUK40/people");
      switch2 = 0;
      Serial.println(count);
    }
    else
    {
      switch1 = 1;
    }
    while (s1 == 0)
    {
      s1 = digitalRead(SWITCH_1_PIN);
      delay(50);
    }
  }
  if (s2 == 0)
  {
    Serial.println("S2");
    if (switch1 == 1)
    {
      count++;
      sendData(String(count), "TUK40/people");
      switch1 = 0;
      Serial.println(count);
    }
    else
    {
      switch2 = 1;
    }
    while (s2 == 0)
    {
      s2 = digitalRead(SWITCH_2_PIN);
      delay(50);
    }
  }

  Serial.println("Ra");
  ///     Rain Servo
  int rainLevel = analogRead(RAIN_PIN);
  sendData(String(rainLevel), "TUK40/rain");
  //Serial.println("Rain = " + String(rainLevel));
  /*
    if (true)
    {*/
  Serial.println("Ro");
  roofControl = getData("TUK40/roof");
  if (roofControl == "1")
  {
    if (roofStatus == 0)
    {
      //Serial.println("Open Roof");
      motorControl(ROOF_TIME, 0);
      roofStatus = 1;
    }
  }
  /*
  else if (roofControl == "2")
  {
    motorControl(ROOF_TIME, 0);
  }
  else if (roofControl == "3")
  {
    motorControl(ROOF_TIME, 1);
  }
  */
  else
  {
    if (roofStatus == 1)
    {
      //Serial.println("Close Roof");
      motorControl(ROOF_TIME, 1);
      roofStatus = 0;
      //Serial.println("ROOF status = " + String(roofStatus));
    }
  }
  //}
  delay(100);
  ///
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

void motorControl(int duration, int direct)
{
  if (direct == 0)
  {
    digitalWrite(MOTOR_A1_PIN, HIGH);
    digitalWrite(MOTOR_A2_PIN, LOW);
  }
  else
  {
    digitalWrite(MOTOR_A1_PIN, LOW);
    digitalWrite(MOTOR_A2_PIN, HIGH);
  }
  delay(duration);
  digitalWrite(MOTOR_A1_PIN, LOW);
  digitalWrite(MOTOR_A2_PIN, LOW);
}

