#include<SoftwareSerial.h>
#include<TinyGPS++.h>

static const int RXPinGsm = 3, TXPinGsm = 4;

static const uint32_t GPSBaud = 9600;

SoftwareSerial gsm(RXPinGsm, TXPinGsm);
TinyGPSPlus gps;

float latitude, longitude;
String error;
bool hasErrors = false;
String rightMessage = "Location";
String phoneNumber = "+4XXXXXXXXXX";
String goo = "https://www.google.com/maps/search/?api=1&query=47.5951518%2C-122.3316393";


void setup() {

  //setup for the gsm module:
     gsm.begin(115200);
     delay(500);
     gsm.println("AT+IPR=9600"); // change the gsm module baud rate to 9600
     delay(1000);
     gsm.begin(9600);
     delay(100);

      setForReceiving();
      Serial.begin(GPSBaud); //setup and begin the gps module
      
      

     

  

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (Serial.available() > 0 ){
    if(gps.encode(Serial.read())){
      getInfo(); 
    }  
  }
  if ( gsm.available() > 0){
      String message = gsm.readString();
      if(message.indexOf(rightMessage) > 0){
        if(message.indexOf("+4XXXXXXXXXX") > 0){
          sendSMS();
        }
      }
  }
  
}

void getInfo(){
  if(gps.location.isValid()){
    
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    hasErrors = false;
  }else{
    
    error = "Location not available";
    hasErrors = true;
  }
}

void setForReceiving(){
  gsm.println("AT+CNMI=2,2,0,0,0"); 
  delay(1000);
}

void sendSMS(){
   gsm.println("AT+CMGF=1"); //TEXT MODE
   delay(100);
   gsm.println("AT+CMGS=\"+4XXXXXXXXXX\"");
   delay(100);
   if (hasErrors == true){
    gsm.print(error);
    gsm.write(26);
    delay(100);
   }else{
     gsm.print("Latitude: ");
     gsm.println(latitude,6);
     gsm.print("Longitude: ");
     gsm.println(longitude,6);
     gsm.print("https://www.google.com/maps/search/?api=1&query=");
     gsm.print(latitude,6);
     gsm.print("%2C");
     gsm.println(longitude,6);
     delay(100);
     gsm.write(26);
     delay(100);
   }
}
