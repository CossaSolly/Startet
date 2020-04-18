#include <SoftwareSerial.h>
#include <MsTimer2.h>
SoftwareSerial mySerial(13, 12);
#include <Servo.h> 

int forwardBackward1 = 5;
int forwardBackward2 = 6;
int CarSpeed = 0;
int CarDirection = 90;
int distance;
int sumD = 0;
int dCount = 0;
int FrontLigtPin  = 7;
int BreakLightPin = 8;
int BellPin       = 11;

int LeftDoorPos  = 50;
int RightDoorPos = 170;

int ServoTurnPin = 2;
int ServoLDPin   = 3;
int ServoRDPin   = 4;

Servo ServoTurn;  
Servo ServoLD;                 
Servo ServoRD; 


int mydelay = 0;
int maxdelay = 25;

char CarMovement = 'F';
char LeftDoor   = 'C'; 
char RightDoor  = 'C';  

long duration;


const char Open  = 'O';
const char Close = 'C';
const char Stop  = 'S';
const char Go    = 'G';

const char Forwad   = 'F';
const char Backward = 'B';
const int trigPin = 9;
const int echoPin = 10;

bool CarCanMove   = false;
String inputString = "";  

int bala = 0;


/*============================================================================================================ */
//http://www.solly.com/setData?gg=1,50,180,90,90
void getDistance(){
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  int maxx = 5;
  
  // Calculating the distance
  if(dCount <= maxx){
   sumD = sumD + duration*0.034/2;
   
  }else{
    

    distance = sumD / maxx;
    sumD = 0;
    dCount = 0;
 
   }
   distance = duration*0.034/2;
   dCount++;
   delay(50);
 
}

/*============================================================================================================ */
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int bellDelay = 0;


/*============================================================================================================ */
void resolveData(String data){

  char temp    = getValue(data, ',' , 0)[0];
 // CarMovement  = getValue(data, ',' , 0)[0];
  CarSpeed     = getValue(data, ',' , 1).toInt();
  CarDirection = getValue(data, ',' , 2).toInt();
  
  if(getValue(data, ',' , 3)[0] == 'C' || getValue(data, ',' , 3)[0] == 'O'){
    if(LeftDoor != getValue(data, ',' , 3)[0]) 
      LeftDoor   = getValue(data, ',' , 3)[0];
  }
  
  if(getValue(data, ',' , 4)[0] == 'C' || getValue(data, ',' , 4)[0] == 'O'){
    if(RightDoor != getValue(data, ',' , 4)[0]) 
       RightDoor  = getValue(data, ',' , 4)[0];
  }
  
  digitalWrite(FrontLigtPin, getValue(data, ',' , 5).toInt());
  
  if(getValue(data, ',' , 6).toInt() > 0){
    digitalWrite(BellPin     , HIGH);
    delay(700);
    digitalWrite(BellPin     , LOW);
  }
  
  if(temp == Stop){
 
     if(temp != CarMovement){     
       digitalWrite(BreakLightPin, HIGH);
       stopCar();
       delay(700);
       digitalWrite(BreakLightPin, LOW);
     }
  }
  CarMovement  = temp;
  
  DoorFnc();
  delay(50);
}

/*============================================================================================================ */
void forWard(){
  analogWrite(forwardBackward1, CarSpeed);
  analogWrite(forwardBackward2, 0);
}

/*============================================================================================================ */
void backWard(){
  analogWrite(forwardBackward2, CarSpeed);
  analogWrite(forwardBackward1, 0);
}

void stopCar(){
    CarSpeed -= 1;
    if(CarSpeed < 0)
       CarSpeed = 0;
      
  analogWrite(forwardBackward2, CarSpeed);
  analogWrite(forwardBackward1, CarSpeed); 
    
}

/*============================================================================================================ */
void DoorFnc(){
 /* 
 if( LeftDoor == 'C' && LeftDoorPos > 20){
   LeftDoorPos--;
   ServoLD.write(LeftDoorPos);
 }else if(LeftDoor == 'O' && LeftDoorPos < 170){
   LeftDoorPos++;
   ServoLD.write(LeftDoorPos);
 }
 
 if( RightDoor == 'O' && RightDoorPos > 20){
   RightDoorPos--;
   ServoRD.write(RightDoorPos);
 }else if(RightDoor == 'C' && RightDoorPos < 170){
   RightDoorPos++;
   
   ServoRD.write(RightDoorPos);
 }*/
 
  if( LeftDoor == 'C' ){
   //LeftDoorPos--;
   ServoLD.write(20);
 }else if(LeftDoor == 'O'){
   //LeftDoorPos++;
   ServoLD.write(170);
 }
 
 if( RightDoor == 'O'){
 //  RightDoorPos--;
   ServoRD.write(20);
 }else if(RightDoor == 'C' ){
//   RightDoorPos++;
   
   ServoRD.write(170);
 }
 
//s Serial.println(RightDoorPos);
//]]]]]]]]]]]]]]]]]]]]]]]]]  Serial.println(LeftDoorPos);
 
 if(CarDirection >= 60 && CarDirection <= 120){
  ServoTurn.write(CarDirection); 
 }
}



/*============================================================================================================ */
void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);
  pinMode(FrontLigtPin , OUTPUT);
  pinMode(BreakLightPin, OUTPUT);
  pinMode(BellPin      , OUTPUT);
 // Serial.println("phakatsi");
  ServoTurn.attach(ServoTurnPin);  
  ServoLD.attach(ServoLDPin);                 
  ServoRD.attach(ServoRDPin);
  
  inputString.reserve(200);
  serialEvent();
  MsTimer2::set(500, controler ); // 500ms period
  MsTimer2::start();

}

void loop() {

 getDistance();
 //DoorFnc();
// Serial.println(distance);
}


/*============================================================================================================ */
void serialEvent() {
  while (mySerial.available()) {
    char inChar = (char)mySerial.read();

    if (inChar == '\n') {
      if(getValue(inputString,':' , 0) == "data");
        resolveData(getValue(inputString,':' , 1));
         Serial.println(inputString);
      inputString = "";
    }else
       inputString += inChar;
  }
}

/*============================================================================================================ */
void controler(){

  serialEvent();
  if(distance < 61){
    double Tospeed = (distance/60.0) * 100;
    Tospeed = (Tospeed/100.0) * 200;
    if(CarSpeed>Tospeed)
    CarSpeed = Tospeed + 0;
  }
   
  if(CarMovement == Forwad){
    forWard();
  }else if(CarMovement == Backward){
    backWard();
  }
     
  //Serial.println("CarSpeed:"   + String(CarSpeed));
  //mydelay = 0;

  String data = "data:";
         data = data + CarMovement + "," + String(CarSpeed) + "," + String(CarDirection) + "," + LeftDoor + "," + RightDoor + "," + String(distance);
         data = data + ",LOL";
  Serial.println(data);
  mySerial.println(data);
}

