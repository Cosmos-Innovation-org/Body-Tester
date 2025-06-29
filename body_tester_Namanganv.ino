#include <DS3231.h>
#include <DFPlayerMini_Fast.h>
#include <Adafruit_Thermal.h>
#include "MAX30100_PulseOximeter.h"
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <NewPing.h>
#include <HX711.h>
#include <math.h>
#include <SoftwareSerial.h>       
#include "adalogo.h"
SoftwareSerial dwin(12,11);
SoftwareSerial printerS(7,6);
SoftwareSerial sonic(10,9); 
Adafruit_Thermal printer(&printerS);        

SoftwareSerial softwareSerial(5,4);     
DFPlayerMini_Fast player;          

#define DT  A0                                                
#define SCK A1  

#define REPORTING_PERIOD_MS     250
PulseOximeter pox;
uint32_t tsLastReport = 0;
DS3231  rtc(SDA, SCL);                     
int maxDistance = 207;
HX711 scale;           
Adafruit_MLX90614 mlx = Adafruit_MLX90614();    
float calibration_factor = -14.95;//-14.95                           
float units;                                                  
//bool isStarted = false;

unsigned char incomingData[100];
char textString[43];

String Data ="";
String DIA ="";
String SYS ="";
String Pulse ="";
int dia = 0;
int sys = 0;
int puls = 0;

bool isErkak = 1;

int roundedTemperature = 0;
int roundedTVI = 0;
int roundedWeight = 0;


int tanga = 0;                                              
float ounces;
int age = 20;
int height = 0;
float heightForTVI = 0;
float cminm = 0;
int SO2 = 0;
int SO2ForPrinting = 0;
int pageId = 0;
float temperature = 0;
float weight = 0;
float tvi = 0;
int Distance = 0;   
bool isReceived = false;   
//int v = 13;   // Выбор автоматической спецификации QR-кода
//int r = 3;    // Уровень коррекции ошибок (3 - высокий уровень)
//int nL = 17;  // Длина данных (низший байт)
//int nH = 0;   // Длина данных (старший байт)
//String qrData = "http://37.140.216.164/bt/transaction?key=";
//String qrData = "http://polito.uz"; // Данные для QR-кода      
String sex = " ";
void (*resetFunc )(void) = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial3.begin(9600);
dwin.begin(9600);
printerS.begin(9600);
printer.begin();
printer.setTimes(1L,1L);
sonic.begin(9600);
rtc.begin();
scale.begin(DT, SCK);                                       
scale.set_scale();                                         
//scale.tare();                                               
scale.set_scale(calibration_factor); 
softwareSerial.begin(9600);   
player.begin(softwareSerial);             // инициализация
player.volume(29);       
Weight();
  // The following lines can be uncommented to set the date and time
    // rtc.setDOW(SATURDAY);     // Set Day-of-Week to SUNDAY
   //  rtc.setTime(11, 31, 00);     // Set the time to 12:00:00 (24hr format)
    // rtc.setDate(13, 07, 2024);  
pinMode(8,INPUT);
}

void loop() {
   //dwin.listen();
   // Height();
   //GetSO2();
   tanga=digitalRead(8);
   //GetTime();
   if(tanga==1 ){
     player.stop();
     delay(100);
     ChangePage(2);
     //Serial.println("Hello");
     //isStarted = false;
    }
    ReceiveData();
}

void ReceiveData(){
  dwin.listen();    
  if(dwin.available()>0){
      int i=0;
      while(dwin.available()>0){
          char incomingByte = dwin.read();
          delay(2);
          incomingData[i]=incomingByte;
          //Serial.print(incomingData[i],HEX);
          i++;
  }
  //Serial.println(incomingData[4]);
  if(incomingData[4]==16 ){
    age=incomingData[8];
    Serial.print(age);
  }
  else if(incomingData[4]==149){
   switch(incomingData[8]){
     case 1:
      //Serial2.println("12345");
      player.play(8); 
      //isStarted = true;
      //Serial.print(incomingData[4]);
       //delay(3500);
      resetFunc();
     break;

     case 3:
      //player.stop();
      delay(50);
      player.play(2);
     break;

     case 6:
      sex = "Ayol";
      isErkak = 0;
      //player.stop();
      delay(50);
      player.play(3); 
     break;

      case 9:
        sex = "Erkak";
        isErkak = 1;
        //player.stop();
        delay(50);
        player.play(3); 
      break;

      default:
      break;
  }
 }
 else if(incomingData[4]==144){
  switch(incomingData[8]){

    case 1:
      //player.stop();
      delay(50);
      player.play(6);
      for(int i=0;i<=9;i++){
       weight += Weight();
       delay(150);
      }
      delay(250);
      weight = weight/10;
      SendFloatWeight(abs(weight+5.70f));
      delay(250); 
      for(int i=0;i<=9;i++){
       height = Height();
       delay(150);
      }
      delay(250);
      //height = height/10;
      SendIntHeight(height);
      delay(250);
      cminm = height/100.0;
      heightForTVI = cminm*cminm;
      tvi = abs(weight)/heightForTVI;
      roundedWeight = round(weight*10);
      roundedTVI = round(tvi*10);
      tvi = roundedTVI / 10.0;
      weight = roundedWeight / 10.0;
      //player.stop();
      //Serial.println(age);
      delay(2500);
      ChangePage(9);
      player.play(1); 
      delay(1500);

      PrintWeightAndHeight(age,abs(weight+5.70f),height,tvi,sex);
      delay(1000);
      SendIntHeight(0);
      SendFloatWeight(0);
      SendIntAge(20);
      sex ="";
      age = 20;
      height = 0;
      weight = 0;
      break;

      
    case 2: 
     //player.stop();
     delay(50);
     player.play(7); 
     delay(2000); 
     if (!mlx.begin()) {
      Serial.println("Error connecting to MLX sensor. Check wiring.");
      mlx.begin();
      delay(2500);
      ChangePage(9);
      player.play(6); 
      delay(1500);
      PrintTemperature(age,temperature,sex);
      SendFloatTemperature(0);
      SendIntAge(20);
      sex=" ";
      age = 20;
      temperature = 0;
     }
      for(int i=0;i<=39;i++){
         temperature = mlx.readObjectTempC();
         //Serial.println(temperature);
         delay(150);
      }
     if(temperature>35){
       temperature=temperature+1.1;
     }
      //temperature = temperature/40;
      SendFloatTemperature(temperature);
      roundedTemperature = round(temperature*10);
      temperature = roundedTemperature / 10.0;
      //player.stop();
      delay(2500);
      ChangePage(9);
      player.play(1); 
      delay(1500);
      PrintTemperature(age,temperature,sex);
      delay(1000);
      SendFloatTemperature(0);
      SendIntAge(20);
      sex ="";
      age = 20;
      temperature = 0;
     break;   

    case 3:
      //player.stop();
      delay(50);
      player.play(5);
      delay(2000);
      if (!pox.begin()) {
      Serial.println("FAILED");
      } else {
      Serial.println("SUCCESS");
      }
      while(SO2 == 0){
       GetSO2();
      }
      //Serial.println(SO2);
      delay(100);
      if(SO2<50){
          while(SO2 < 50){
            GetSO2();
          }
       }else{
         SendIntSO2(SO2);
         SO2ForPrinting = SO2;
       }
      while(SO2 != 0){
        GetSO2();
      }
      delay(2500);
      ChangePage(9);
      player.play(1); 
      delay(1500);
      Serial.println(SO2ForPrinting);
      PrintSO2(age,SO2ForPrinting,sex);
      delay(1000);
      SendIntSO2(0);
      SendIntAge(20);
      sex=" ";
      age = 20;
      break;
    case 4:
      //player.stop();
      delay(50);
      player.play(4);  
     break;
    case 5:
      //player.stop();
      delay(50);
      if(Serial3.available()){
       Serial3.flush();
      }
       Serial3.write("1");
      ////////it has to be optimaze
      while(!Serial3.available()){
        ///kutinggggg
        //Serial.println("davlenaaaaaa");
      }
          Data = Serial3.readString();
          //Serial.println(Data.length()); 
          //Serial.println(Data); 
        if(Data.length() == 41){
            DIA = Data.substring(16,19);
            SYS = Data.substring(19,22);
            Pulse = Data.substring(27,30);
            DIA.trim();
            SYS.trim();
            Pulse.trim();
            dia = DIA.toInt();
            sys = SYS.toInt();
            puls = Pulse.toInt();
            //Serial.println(dia);
            //Serial.println(sys);
            //Serial.println(puls);
            SendIntBPHigh(dia);
            SendIntBPLow(sys);
            SendIntPuls(puls);
            //player.stop();
          }
      delay(2500);
      ChangePage(9);
      player.play(1); 
      delay(1500);
      PrintBPD(age,dia,sys,puls,sex);
      //delay(1000);
      SendIntBPHigh(0);
      SendIntBPLow(0);
      SendIntPuls(0);
      SendIntAge(20);
      Data = "";
      age = 20;
      sex = "";
      break;
    case 6:
      //player.stop();
      delay(50);
      player.play(4);  
     break;
    
    /*case 7:
     /* player.stop();
      delay(100);
      player.play(1); 
      delay(1500);
      Serial.println(SO2ForPrinting);
      PrintSO2(age,SO2ForPrinting,sex);
      delay(500);
      SendIntSO2(0);
      SendIntAge(20);
      sex=" ";
      break;*/
    /*case 8:
      /*player.stop();
      delay(100);
      player.play(1); 
      delay(1500);
      PrintBPD(age,dia,sys,puls,sex);
      SendIntBPHigh(0);
      SendIntBPLow(0);
      SendIntPuls(0);
      SendIntAge(20);
      Data = "";
      break;*/
    case 9:
      if(pageId==0){
        player.stop();
        if(Serial3.available()){
         Serial3.flush();
        }
        Serial3.write("1");
        while(!Serial3.available()){
          ///kutinggggg
          //Serial.println("kutvomman");
        }
          Data = Serial3.readString();
            //Serial.println(Data); 
          if(Data.length() == 41){
            DIA = Data.substring(16,19);
            SYS = Data.substring(19,22);
            Pulse = Data.substring(27,30);
            DIA.trim();
            SYS.trim();
            Pulse.trim();
            dia = DIA.toInt();
            sys = SYS.toInt();
            puls = Pulse.toInt();
            //Serial.println(dia);
            //Serial.println(sys);
            //Serial.println(puls);
          }
          SendIntBPHigh(dia);
          SendIntBPLow(sys);
          SendIntPuls(puls);
          //player.stop();
          delay(2500);
          pageId=6;
          ChangePage(pageId);
       }
      if(pageId==6){
        player.play(7); 
        delay(2000);
        if (!mlx.begin()) {
         Serial.println("Error connecting to MLX sensor. Check wiring.");
         mlx.begin();
         delay(2500);
         pageId = 5;
         ChangePage(pageId);
        }  
        for(int i=0;i<=39;i++)
        {
         temperature = mlx.readObjectTempC();
         delay(150); 
        }
        if(temperature>35){
       temperature=temperature+1.1;
       }
       // temperature = temperature/40;
        SendFloatTemperature(temperature);
        roundedTemperature = round(temperature*10);
        temperature = roundedTemperature / 10.0;
        delay(2500);
        pageId = 5;
        ChangePage(pageId);
       }
      if(pageId == 5){
       // player.stop();
        delay(50);
        player.play(6);
        for(int i=0;i<=9;i++){
         weight += Weight();
         delay(150);
        }
        delay(250);
        weight = weight/10;
        SendFloatWeight(abs(weight+5.70f));
        delay(250); 
        for(int i=0;i<=9;i++){
         height = Height();
         delay(150);
        }
        delay(250);
        //height = height/10;
        SendIntHeight(height);
        delay(250);
        cminm = height/100.0;
        heightForTVI = cminm*cminm;
        tvi = abs(weight)/heightForTVI;
        roundedWeight = round(weight*10);
        weight = roundedWeight / 10.0;
        roundedTVI = round(tvi*10);
        tvi = roundedTVI / 10.0;
        delay(2500);
        pageId = 8;
        ChangePage(pageId);
       }
      if(pageId == 8){
       player.play(5);
       delay(2000);
       if (!pox.begin()) {
       Serial.println("FAILED");
       } else {
       Serial.println("SUCCESS");
       }
       while(SO2 == 0){
         GetSO2();
       }
       delay(50);

       if(SO2<50){
          while(SO2 < 50){
            GetSO2();
          }
       }else{
         SendIntSO2(SO2);
         SO2ForPrinting = SO2;
       }
       while(SO2 != 0){
        GetSO2();
        delay(50);
       }
       //delay(1500);
       pageId = 10;
       ChangePage(pageId);
       }
     if(pageId == 10){
       player.play(9); 
       delay(1000);
       Print(age,abs(weight+5.70f),height,SO2ForPrinting,temperature,dia,sys,puls,tvi,sex);
       delay(1000);
       SendFloatTemperature(0);
       SendIntHeight(0);
       SendFloatWeight(0);
       SendIntSO2(0);
       SendIntBPHigh(0);
       SendIntBPLow(0);
       SendIntPuls(0);
       SendIntAge(20);
       pageId = 0;
       age = 20;
       sex="";
       height = 0;
       weight = 0;
       temperature = 0;
      }
        break;
      
    default:
     Weight();
    break;
  }
    }
 /* if(incomingData[3]=byte(0x83)){
    age=incomingData[8];
    Serial.println(age);
    printer.println("Shaxzo jinni ");
    printer.println("Mushtari jinni");
    printer.println("Sardor jinni");
    printer.println(age);
    printer.println(" ");
    printer.println(" ");
    if(age==1){
      SendFloatWeight(ounces);
      SendIntHeight(22);
      Serial.println("qalesaaaaaaaaaaaaan");
      }
    }*/

    memset(incomingData,0,sizeof(incomingData));
 }
}
//////////// change pages
void ChangePage(int pageId){
  dwin.listen();    
  dwin.write(0x5A);
  dwin.write(0xA5);
  dwin.write(0x07);
  dwin.write(0x82);
  dwin.write((byte)0x00);
  dwin.write(0x84);
  dwin.write(0x5A);
  dwin.write(0x01);
  dwin.write((byte)0x00);
  dwin.write((byte)pageId);
 }



//////// weight olchash
float Weight(){
  //Serial.print("Reading: ");                                  
  for (int i = 0; i < 10; i ++) {                             
    units = + scale.get_units(), 10;                         
  }
  units = units / 10;                                         
  ounces = units * 0.035274*28.35*0.01;                                
  //Serial.print(ounces);                                     
  //Serial.println("кг");    
 
  return ounces;
  }



//////////// send floatt
void SendFloatWeight(float weight){
  dwin.write(0x5A);
  dwin.write(0xA5);
  dwin.write(0x07);
  dwin.write(0x82);
  dwin.write(0x20);
  dwin.write(byte(0x00));

  byte hex[4]={0};

  FloatToHex(weight,hex);

  dwin.write(hex[3]);
  dwin.write(hex[2]);
  dwin.write(hex[1]);
  dwin.write(hex[0]);
 }

void SendFloatTemperature(float temperature){
  dwin.write(0x5A);
  dwin.write(0xA5);
  dwin.write(0x07);
  dwin.write(0x82);
  dwin.write(0x40);
  dwin.write(byte(0x00));

  byte hex[4]={0};

  FloatToHex(temperature,hex);

  dwin.write(hex[3]);
  dwin.write(hex[2]);
  dwin.write(hex[1]);
  dwin.write(hex[0]);
  }


  ///send integer
void SendIntHeight(int height){
  dwin.write(0x5A);
  dwin.write(0xA5);
  dwin.write(0x05);
  dwin.write(0x82);
  dwin.write(0x30);
  dwin.write(byte(0x00));
  dwin.write(highByte(height));
  dwin.write(lowByte(height));
  }
void SendIntAge(int age){
  dwin.write(0x5A);
  dwin.write(0xA5);
  dwin.write(0x05);
  dwin.write(0x82);
  dwin.write(0x10);
  dwin.write(byte(0x00));
  dwin.write(highByte(age));
  dwin.write(lowByte(age));
  }
void SendIntPuls(int puls){

  dwin.write(0x5A);
  dwin.write(0xA5);
  dwin.write(0x05);
  dwin.write(0x82);
  dwin.write(0x80);
  dwin.write(byte(0x00));
  dwin.write(highByte(puls));
  dwin.write(lowByte(puls));
  }

void SendIntSO2(int so2){
  dwin.write(0x5A);
  dwin.write(0xA5);
  dwin.write(0x05);
  dwin.write(0x82);
  dwin.write(0x50);
  dwin.write(byte(0x00));
  dwin.write(highByte(so2));
  dwin.write(lowByte(so2));
 }
  
void SendIntBPHigh(int bphigh){
  dwin.write(0x5A);
  dwin.write(0xA5);
  dwin.write(0x05);
  dwin.write(0x82);
  dwin.write(0x60);
  dwin.write(byte(0x00));
  dwin.write(highByte(bphigh));
  dwin.write(lowByte(bphigh));
  }
   
void SendIntBPLow(int bplow){
  dwin.write(0x5A);
  dwin.write(0xA5);
  dwin.write(0x05);
  dwin.write(0x82);
  dwin.write(0x70);
  dwin.write(byte(0x00));
  dwin.write(highByte(bplow));
  dwin.write(lowByte(bplow));
  }
  ////// height
int Height(){
  sonic.listen();
  //
  
  byte address = 0x01;
  byte command = 0x01;
  byte checksum = (0x55 + 0xaa + address + command) & 0x00FF;

  sonic.write(0x55);
  sonic.write(0xaa);
  sonic.write(address);
  sonic.write(command);
  sonic.write(checksum);
  delay(50);

 
  if (sonic.available() >= 7) {
    byte startByte = sonic.read();   
    byte userAddress = sonic.read();  
    byte responseAddress = sonic.read();
    byte responseCommand = sonic.read();
    byte highData = sonic.read();     
    byte lowData = sonic.read();      
    byte responseChecksum = sonic.read();
    
    byte expectedChecksum = (startByte + userAddress + responseAddress + responseCommand + highData + lowData) & 0x00FF;
    if (responseChecksum == expectedChecksum) {
      
       int distance = (highData << 8) | lowData;

       Distance = distance/10;
       //heightForTVI = Distance/100;
       //Serial.print("Рост: ");
       //Serial.print(heightForTVI);
       Serial.println(Distance);

    } else {
      Serial.println("Invalid checksum");
    }
  }
  return maxDistance-Distance;
}

void GetSO2(){
   pox.update();
   if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        //pox.getHeartRate();
        //if(pox.getSpO2() > 0){
         SO2 = pox.getSpO2();
        //}
        Serial.println(pox.getSpO2());
        tsLastReport = millis();
       }

  }

////////////////////////////printing
void PrintWeightAndHeight(int age, float weight , int height,int tvi,String sex){
  
  printer.setSize('M'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.println("COSMOS INNOVATION");
  printer.println(" ");
  printer.boldOff();  
  printer.justify('L');
  printer.setSize('S');
  printer.justify('C');
  printer.boldOn(); 
  printer.println("   Innovatsion kelajakni");
  printer.println("  biz bilan birga quring ");
  printer.boldOff();  
  printer.feed(1);
  printer.println("cosmosinnovation@gmail.com");
  printer.println("Tel:+998930501217");
  printer.feed(1);
  printer.justify('C');
  printer.println("------------------------------");
  printer.justify('L');
  printer.print("  Yoshingiz.............");
  printer.println(age);
  printer.feed(1);
  printer.print("  Vazningiz.............");
  printer.println(weight);
  printer.feed(1);
  printer.print("  Bo'yingiz.............");
  printer.println(height);
  printer.feed(1);
  printer.print("  Jins................");
  printer.println(sex);
  printer.feed(1);
  printer.print("  Tana vazn indeksi.....");
  printer.println(tvi);
  printer.justify('C');
  printer.feed(1);
  printer.println(rtc.getDateStr());
  printer.println(rtc.getTimeStr());
  
  QRCodeAndCut();
  
}
void PrintTemperature(int age, float temperature ,String sex){
 
  printer.setSize('M'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.println("COSMOS INNOVATION");
  printer.println(" ");
  printer.boldOff();  
  printer.justify('L');
  printer.setSize('S');
  printer.justify('C');
  printer.boldOn(); 
  printer.println("   Innovatsion kelajakni");
  printer.println("  biz bilan birga quring ");
  printer.boldOff();  
  printer.feed(1);
  printer.println("cosmosinnovation@gmail.com");
  printer.println("Tel:+998930501217");
  printer.feed(1);
  printer.justify('C');
  printer.println(" ------------------------------");
  printer.print(" Yoshingiz.............");
  printer.println(age);
  printer.feed(1);
  printer.print(" Tana harorati.........");
  printer.println(temperature);
  printer.feed(1);
  printer.print(" Jins................");
  printer.println(sex);
  printer.feed(1);
  printer.justify('C');
  printer.feed(1);
  printer.println(rtc.getDateStr());
  printer.println(rtc.getTimeStr());
  Cut();
}
void PrintBPD(int age,int dia,int sys, int pulse,String sex){
  
  printer.setSize('M'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.println("COSMOS INNOVATION");
  printer.println(" ");
  printer.boldOff();  
  printer.justify('L');
  printer.setSize('S');
  printer.justify('C');
  printer.boldOn(); 
  printer.println("   Innovatsion kelajakni");
  printer.println("  biz bilan birga quring ");
  printer.boldOff();  
  printer.feed(1);
  printer.println("cosmosinnovation@gmail.com");
  printer.println("Tel:+998930501217");
  printer.feed(1);
  printer.justify('C');
  printer.println(" -----------------------------");
  printer.print(" Yoshingiz.............");
  printer.println(age);
  printer.feed(1);
  printer.print(" SYS/DIA...............");
  printer.print(dia);
  printer.print("/");
  printer.println(sys);
  printer.feed(1);
  printer.print(" Pulse.............");
  printer.println(pulse);
  printer.feed(1);
  printer.print(" Jins................");
  printer.println(sex);
  printer.feed(1);
  printer.justify('C');
  printer.feed(1);
  printer.println(rtc.getDateStr());
  printer.println(rtc.getTimeStr());
  Cut();
  
}
void PrintSO2(int age,int so2,String sex){
  
  printer.setSize('M'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.println("COSMOS INNOVATION");
  printer.println(" ");
  printer.boldOff();  
  printer.justify('L');
  printer.setSize('S');
  printer.justify('C');
  printer.boldOn(); 
  printer.println("   Innovatsion kelajakni");
  printer.println("  biz bilan birga quring ");
  printer.boldOff();  
  printer.feed(1);
  printer.println("cosmosinnovation@gmail.com");
  printer.println("Tel:+998930501217");
  printer.feed(1);
  printer.justify('C');
  printer.println(" -----------------------------");
  printer.print(" Yoshingiz.............");
  printer.println(age);
  printer.feed(1);
  printer.print(" SpO2..................");
  printer.print(so2);
  printer.println("%");
  printer.feed(1);
  printer.print(" Jins................");
  printer.println(sex);
  printer.feed(1);
  printer.justify('C');
  printer.feed(1);
  printer.println(rtc.getDateStr());
  printer.println(rtc.getTimeStr());
  Cut();
}
void Print(int age, float weight , int height, int so2,float temperature,int dia,int sys,int puls,int tvi,String sex){
  
  printer.setSize('M'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.println("COSMOS INNOVATION");
  printer.println(" ");
  printer.boldOff();  
  printer.justify('L');
  printer.setSize('S');
  printer.justify('C');
  printer.boldOn(); 
  printer.println("   Innovatsion kelajakni");
  printer.println("  biz bilan birga quring ");
  printer.boldOff();  
  printer.feed(1);
  printer.println("cosmosinnovation@gmail.com");
  printer.println("Tel:+998930501217");
  printer.feed(1);
  printer.justify('C');
  printer.println(" ------------------------------");
  printer.print(" Yoshingiz.............");
  printer.println(age);
  printer.println(" ");
  printer.print(" Vazningiz.............");
  printer.println(weight);
  printer.println(" ");
  printer.print(" Bo'yingiz.............");
  printer.println(height); 
  printer.println(" "); 
  printer.print(" SpO2..................");
  printer.print(so2);
  printer.println("%");
  printer.println(" "); 
  printer.print(" Tana harorati.........");
  printer.println(temperature);
  printer.println(" ");
  printer.print(" SYS/DIA...............");
  printer.print(dia);
  printer.print("/");
  printer.println(sys);
  printer.print(" Pulse.............");
  printer.println(puls);
  printer.println(" ");
  printer.print(" Jins................");
  printer.println(sex);
  printer.println(" ");
  printer.print(" Tana vazn indeksi.....");
  printer.println(tvi);
  printer.justify('C');
  printer.feed(1);
  printer.println(rtc.getDateStr());
  printer.println(rtc.getTimeStr());
  QRCodeAndCut();
}
///////float to hex
void FloatToHex(float f, byte* hex){
  byte* f_byte=reinterpret_cast<byte*>(&f);
  memcpy(hex,f_byte,4);
}
void QRCodeAndCut(){

  /*printer.println(" ");
  printer.println(" ");

  printerS.write(0x1B);
  printerS.write(0x61);
  printerS.write(0x01); // 1 - выравнивание по центру
  
  printerS.write(0x1D);
  printerS.write(0x6B);
  printerS.write(0x61);
  printerS.write(v);
  printerS.write(r);
  printerS.write(nL);
  printerS.write(nH);
  printerS.print(qrData);
  
  printerS.write(0x0A);
  printerS.write(0x1D);*/

  ////////////////////////////------ Recomendaditon-----------///////////////////////////////

  if(tvi <=16){

  printer.setSize('B'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.feed(2);
  printer.println("Tavsiya:");
  printer.setSize('S'); 
  printer.justify('C');
  printer.boldOff(); 
  printer.feed(1);
  printer.boldOn(); 
  printer.setSize('M'); 
  printer.println("Vaznning kuchli yetishmovchigi");
  printer.boldOff(); 
  printer.setSize('S');  
  printer.feed(1);
  printer.println("Shoshilinch nazorat va mutahasis maslahati talab etiladi");

  }
  else if(tvi >16 and tvi <=18.5)
  {
  printer.setSize('B'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.feed(2);
  printer.println("Tavsiya:");
  printer.setSize('S'); 
  printer.justify('C');
  printer.boldOff(); 
  printer.feed(1);
  printer.boldOn(); 
  printer.setSize('M'); 
  printer.println("Vazn yetishmovchiligi");
  printer.boldOff(); 
  printer.setSize('S');  
  printer.feed(1);
  printer.println("Kuch va chidamlilikni pasayoshiga, repraduktiv funksiyalar buzulishiga olib keladi");
  
  
  }
  else if(tvi >18.5 and tvi <=25)
  {
  printer.setSize('B'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.feed(2);
  printer.println("Tavsiya:");
  printer.setSize('S'); 
  printer.justify('C');
  printer.boldOff(); 
  printer.feed(1);
  printer.boldOn(); 
  printer.setSize('M'); 
  printer.println("Normal");
  printer.boldOff(); 
  printer.setSize('S');  
  printer.feed(1);
  printer.println("Vaznni saqlash tavsiya etiladi");


  }
  else if(tvi >25 and tvi <=30)
  {

  printer.setSize('B'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.feed(2);
  printer.println("Tavsiya:");
  printer.setSize('S'); 
  printer.justify('C');
  printer.boldOff(); 
  printer.feed(1);
  printer.boldOn(); 
  printer.setSize('M'); 
  printer.println("Ortiqcha vazn yig'ish boshlangan");
  printer.boldOff(); 
  printer.setSize('S');  
  printer.feed(1);
  printer.println("Metabolik sindrom yog'li gepatoz rivojlanish havfi mavjud");

  }
  else if(tvi >30 and tvi <=35)
  {
  printer.setSize('B'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.feed(2);
  printer.println("Tavsiya:");
  printer.setSize('S'); 
  printer.justify('C');
  printer.boldOff(); 
  printer.feed(1);
  printer.boldOn(); 
  printer.setSize('M'); 
  printer.println("Semizlik");
  printer.boldOff(); 
  printer.setSize('S');  
  printer.feed(1);
  printer.println("Qandli diabet, ateroskleroz, yurak qon tomir tizimi kasalliklari bilan kasallanish havfini oshiradi");

  }
  else if(tvi >35 and tvi <=40)
  {
  printer.setSize('B'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.feed(2);
  printer.println("Tavsiya:");
  printer.setSize('S'); 
  printer.justify('C');
  printer.boldOff(); 
  printer.feed(1);
  printer.boldOn(); 
  printer.setSize('M'); 
  printer.println("Semizlikning ortishi");
  printer.boldOff(); 
  printer.setSize('S');  
  printer.feed(1);
  printer.println("Mutahasis bilan bog'lanishingiz, shifokor tavsiyasiga ko'ra harakat qilishingiz kerak");

  }
  else if(tvi >40)
  {
  printer.setSize('B'); 
  printer.justify('C');
  printer.boldOn(); 
  printer.feed(2);
  printer.println("Tavsiya:");
  printer.setSize('S'); 
  printer.justify('C');
  printer.boldOff(); 
  printer.feed(1);
  printer.boldOn(); 
  printer.setSize('M'); 
  printer.println("Kuchli semizlik");
  printer.boldOff(); 
  printer.setSize('S');  
  printer.feed(1);
  printer.println("Hamma a'zolar ishdan chiqishi mumkin, hayot uchun havfli");
  }



  printer.println(" ");
  printer.println(" ");
  printer.println(" ");
  printer.println(" ");
  printer.print("\x1B\x69");
  printer.feed(2);
  printer.printBitmap(adalogo_width, adalogo_height, adalogo_data);
}

void Cut(){

  printer.println(" ");
  printer.println(" ");
  printer.println(" ");
  printer.println(" ");
  printer.print("\x1B\x69");
  printer.feed(2);
  printer.printBitmap(adalogo_width, adalogo_height, adalogo_data);
}


