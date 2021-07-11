
/******Since the esp01chip is faulty so commented this part erased the code 
related to it but showed the libraries to be used in real comercial model*******/
#define BLYNK_PRINT Serial
//#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = "paste your api";
char ssid[] = "Aditya";
char pass[] = "Adi3112";
BlynkTimer timer;
#define ESP8266_BAUD 9600
ESP8266 wifi(&Serial);
#define modee 11
int swt=0;


//***Since Arduino couldnt take load of lcd screen along with other components***
//#include <LiquidCrystal.h>
//LiquidCrystal lcd(8,9,10,11,12,13);//RS,EN,D4,D5,D6,D7


#include <Servo.h>
#define sen1 A3                                                             

//left side waste monitoring ultrasensor
#define trigPin1 2
#define echoPin1 3
//right side waste monitoring ultrasensor
#define trigPin2 8  
#define echoPin2 9

//Garbage detection used as subsitute for ir sensor 
#define trigPin3 5 
#define echoPin3 6




int left=0;
int right=0;
long duration, distance,sensor1,sensor2; 
int onetime=0,onetime1=0 ;
int wet=0,moisture=0,object=0,cabin2=0,c1=0,c2=0;
Servo myservo_right;
Servo myservo_left;



void setup() 
{
     pinMode(sen1,INPUT);
     pinMode(trigPin1, OUTPUT);
     pinMode(echoPin1, INPUT);
     pinMode(trigPin2, OUTPUT);
     pinMode(echoPin2, INPUT);
     pinMode(trigPin3, OUTPUT);
     pinMode(echoPin3, INPUT);
     pinMode(modee,INPUT_PULLUP);
     myservo_left.attach(7);  
     myservo_left.write(155);
     myservo_right.attach(4); 
     myservo_right.write(25);
     Serial.begin(9600);
     Serial.begin(ESP8266_BAUD);
     swt=digitalRead(modee);
     if(swt==LOW)
     {
       Blynk.begin(auth, wifi, ssid, pass);
     }
  timer.setInterval(2000L, sendSensor); 
}

void loop() 
{
     swt=digitalRead(modee);
     Serial.println(swt);
     if(swt==LOW)
     {
       Blynk.run();
     }
     timer.run();
     ultrasensor(trigPin1, echoPin1);
     sensor1 = distance;
     delay(10);
     ultrasensor(trigPin2, echoPin2);
     sensor2 = 16;    
     delay(10);
     Serial.print("Wet level empty: ");
     Serial.println(sensor1);
     Serial.print("dry level empty: ");
     Serial.println(sensor2);
     left=(16-sensor1);
     right=(16-sensor2);
     delay(1000);
     Serial.print("Wet level filled: ");
     Serial.println(left);
     Serial.print("dry level filled: ");
     Serial.println(right);
     delay(1000);
    if(left<0){left=0;}
    if(right<0){right=0;}
    if(left>100){left=100;}
    if(right>100){right=100;}
 //******kept this code as a part to show the intention of measuring level of waste
  if(left>10){Blynk.notify("dustbin is full:- id-0001");}
  if(right>10){Blynk.notify("dustbin is full:- id-0001");}
    if(left>10)Serial.println("Please Clear wet bin");
    if(right>10)Serial.println("Please clear dry bin");


 ultrasensor(trigPin3, echoPin3);;
 moisture=analogRead(sen1);
 Serial.print("distance of waste = ");
 Serial.println(distance);
 
 if(distance<25)
 {
    //took multiple moisture readings for precision
      moisture=analogRead(sen1);
      moisture=analogRead(sen1);
      Serial.print("moisture = ");
      Serial.println(moisture);
      delay(500);
      moisture=analogRead(sen1);
      Serial.print("moisture = ");
      Serial.println(moisture);
      delay(500);
      if(moisture>=700)                         
      {
        Serial.print(" WET  GARBAGE");
       myservo_right.write(85);
       delay(3000);
       myservo_right.write(25);
       delay(2000);
      }
     else 
    {
        Serial.println(" dry  GARBAGE\n");
        myservo_left.write(85);
        delay(3000);
        myservo_left.write(155);
        delay(2000);
        Serial.println("moisture");
   }    
  }    
 }
 void sendSensor()
{ 
  Blynk.virtualWrite(V5, left); // Humidity for gauge
  Blynk.virtualWrite(V6, right); // Temperature for gauge
}

 void ultrasensor(int trigPin,int echoPin)
 { 
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; //speed of sound usedd is 340m/s
 }
 