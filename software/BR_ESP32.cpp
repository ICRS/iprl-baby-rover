/*Initial programme flashed onto the baby rover's ESP32-S3-DevkitC board. This was done via platformio.
Can do basic movement and connect to your laptop over hotspot for remote control
TODO: Implement speed control
TODO: Implement cammast
*/

#include <Arduino.h>
#include <WiFi.h>


/* --------------MOTOR DRIVER PINS-------------*/
#define IN1R 2
#define IN2R 1
#define IN3R 41
#define IN4R 42
#define ENAR 39
#define ENBR 40
#define IN1L 4
#define IN2L 5
#define IN3L 6
#define IN4L 7
#define ENAL 15
#define ENBL 16

const char* ssid = //enter hotspot name here;
const char* password = //enter wifi pw here;


void MoveRightSide(int speed, bool dir){ //dir True = fwd, false = back
  digitalWrite(IN1R, dir);
  digitalWrite(IN2R, !dir);
  digitalWrite(IN3R, dir);
  digitalWrite(IN4R, !dir);
  analogWrite(ENAR, speed);
  analogWrite(ENBR, speed);
  analogWrite(ENAL, speed);
  analogWrite(ENBL, speed);
}

void MoveLeftSide(int speed, bool dir){
  digitalWrite(IN1L, dir);
  digitalWrite(IN2L, !dir);
  digitalWrite(IN3L, dir);
  digitalWrite(IN4L, !dir);
  analogWrite(ENAR, speed);
  analogWrite(ENBR, speed);
  analogWrite(ENAL, speed);
  analogWrite(ENBL, speed);
}

void MoveFwd(int speed){
  MoveRightSide(speed, true);
  MoveLeftSide(speed, true);
}

void MoveBack(int speed){
  MoveRightSide(speed, false);
  MoveLeftSide(speed, false);
}

void MoveRight(int speed){
  MoveRightSide(speed, false);
  MoveLeftSide(speed, true);
}

void MoveLeft(int speed){
  MoveRightSide(speed, true);
  MoveLeftSide(speed, false);
}

void Stop(){
  MoveRightSide(0, true);
  MoveLeftSide(0, true);
}

int defaultSpeed = 196;
WiFiClient client;

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(IN1R, OUTPUT);
  pinMode(IN2R, OUTPUT);
  pinMode(IN3R, OUTPUT);
  pinMode(IN4R, OUTPUT);
  pinMode(ENAR, OUTPUT);
  pinMode(ENBR, OUTPUT);
  pinMode(IN1L, OUTPUT);
  pinMode(IN2L, OUTPUT);
  pinMode(IN3L, OUTPUT);
  pinMode(IN4L, OUTPUT);
  pinMode(ENAL, OUTPUT);
  pinMode(ENBL, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to network...");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to network!:");
  Serial.println(WiFi.localIP());
  //If it doesn't connect to laptop it won't initialise
  while(!client.connect(IPAddress(192,168,61,151), 10000)){
    Serial.println("Connection to host failed");
    delay(1000);
  }
  Serial.println("Connected to server!");
}

void loop() {
  if (!client.connected()){
    Stop();
    Serial.println("Reonnecting to server...");
    while (!client.connect(IPAddress(192, 168, 61, 151), 10000)) {
      Serial.println("Reconnection failed, retrying...");
      delay(2000);
    }
  }
  // Reads the command over wifi and does the corresponding movement over if statements
  // this can also be done over Serial, if you basically change client with Serial
  if (client.available()){
    char command = client.read();
    Serial.print(command);
  if (command == 'f'){
    MoveFwd(defaultSpeed);
  }
  else if (command== 'b'){
    MoveBack(defaultSpeed);
  }
  else if (command=='l'){
    MoveLeft(defaultSpeed);
  }
  else if (command=='r'){
    MoveRight(defaultSpeed);
  }
  else if (command=='s'){
    Stop();
  }
  }
}

