#include <SoftwareSerial.h> 
#include<LiquidCrystal.h>
SoftwareSerial gps(A3,A4);//Rx tx
LiquidCrystal lcd(8,9,10,11,12,13);//Rs en d4-7
#define motor 3
#define eye  A0
#define alc A1 
#define buzzer 7 
#define but 4
//////////////////////////////////////////////////////
int counterr=0;
int butt = 0,duration=0; //on off button
int alcohal=1,rd=0,counter=0,timer=0;
char str[70];
String gpsString="";
char *test="$GPGGA";
String latitude ="";//
String longitude="";//
int temp=0,i;  
boolean Serial_status=0; 
void setup()
{
 Serial.begin(9600);    // BAUDRATE BPS
 pinMode(but,INPUT_PULLUP);
 pinMode(buzzer,OUTPUT);
 pinMode(eye,INPUT);
 pinMode(alc,INPUT);
 //pinMode(motor,OUTPUT);
 lcd.begin(16, 2);//initializing LCD
 lcd.setCursor(0, 0);
 lcd.print("Smart Car");
 delay(1000);
 lcd.clear();
 analogWrite(motor,250);
} 

void loop()
{
 lcd.setCursor(0, 0);
 lcd.print("Safe driving!");
 butt=digitalRead(but);
 alcohal=digitalRead(alc);
 rd=digitalRead(eye);
 if(rd==LOW)
 {
  counter=0;
  timer=0;
  do
  {
   counter=counter+1;
   delay(10);
   rd=digitalRead(eye);
   if(counter>=75)
   {
    digitalWrite(buzzer,HIGH); 
    analogWrite(motor,70);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("sleep deteted");
   }
  }while(rd==LOW);
  delay(1000);
  lcd.clear();
  digitalWrite(buzzer,LOW);
 } 
 butt=digitalRead(but);
 alcohal=digitalRead(alc);
 if(butt==LOW)
 {
  digitalWrite(motor,LOW);
  lcd.clear();
  lcd.setCursor(0, 1);digitalWrite(motor,LOW);
  lcd.print("Accident Alert!");
  digitalWrite(buzzer,HIGH);
  digitalWrite(motor,HIGH);digitalWrite(motor,LOW);
   get_gps();
  init_sms1();
  send_data(latitude);
  send_data(loongitude);
  send_data("Accident alert!");
  send_sms();
  delay(5000);
  lcd.clear();
 } 
 else if(alcohal==LOW)
 { 
  digitalWrite(motor,LOW);
  if(counterr>1)
  {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Alcohal Alert!");
  digitalWrite(buzzer,HIGH);
  get_gps();
  init_sms1();
  send_data("Alcohal detected !");
  send_data("Driver at this location-");
  send_data(latitude);
  send_data(loongitude);
  send_sms();
  delay(5000);
  lcd.clear(); 
  }
  counterr=counterr+1;
  delay(200);
 }
 else
 {
  digitalWrite(buzzer,LOW);
  digitalWrite(motor,HIGH);
 }
}
void init_sms1()
{
 Serial.println("AT+CMGF=1");
 delay(400);
 Serial.println("AT+CMGS=\"+919212602861\"");   // use your 10 digit cell no. here 
 delay(400);
}
 
void send_data(String message)
{
 Serial.println(message);
 delay(200);
}

void send_sms()
{
 Serial.write(26);
}

void gpsEvent()
{
  gpsString="";
  while(1)
  {
   while (gps.available()>0)            //checking serial data from GPS
   {
    char inChar = (char)gps.read();
     gpsString+= inChar;                    //store data from GPS into gpsString
     i++;
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //checking for $GPGGA sentence
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>65)
     {
       Serial_status=1;
       break;
     }
     else
     {
      i=0;
     }
    }
  }
  if(Serial_status)
  break;
  }
}

void get_gps()
{
 Serial_status=0;
 int x=0;
 while(Serial_status==0)
 {
  gpsEvent();
  int str_lenth=i;
  latitude="";
  longitude="";
  int comma=0;
  while(x<str_lenth)
  {
   if(gpsString[x]==',')
   comma++;
   if(comma==2)        //extract latitude from string
   latitude+=gpsString[x+1];     
   else if(comma==4)        //extract longitude from string
   longitude+=gpsString[x+1];
   x++;
  }
  int l1=latitude.length();
  latitude[l1-1]=' ';
  l1=longitude.length();
  longitude[l1-1]=' ';
  i=0;x=0;
  str_lenth=0;
  delay(20);
 }
}
