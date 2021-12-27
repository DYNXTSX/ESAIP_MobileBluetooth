#include <SoftwareSerial.h>   // librairie pour creer une nouvelle connexion serie max 9600 baud
#include <LiquidCrystal.h>
#define PIN_LED 8

SoftwareSerial BTSerial(6, 7); // RX | TX  = > BT-TX=10 BT-RX=11
LiquidCrystal lcd(12,11,5,4,3,2);

int heures, minutes, secondes, hAlarme = 15, minutesAlarme = 45;
char heure[50] = "";

void setup()
{
  Serial.begin(9600);
  Serial.println("Enter a command:");
  BTSerial.begin(9600);  // HC-05 9600 baud 
  pinMode(PIN_LED, OUTPUT);
  lcd.begin(16,2);
  heures = 15;
  minutes = 44;
  secondes = 50;
}

void loop()
{
  gestionHeure();
  getMsg();  
}

void gestionHeure(){
  if(secondes == 60){
    secondes = 0;
    minutes++;
  }
  if(minutes == 60){
    minutes = 0;
    heures++;
  }
  if(heures == 24){
    heures = 0;
  }
  //verification heure = alarme
  if(heures == hAlarme && minutes == minutesAlarme && secondes == 0){
    pshiiiit(); //fonction qui actionne le diffuseur
  }
  //affichage sur écran LCD
  sprintf(heure,"%2d:%2d:%2d | %2d:%2d",heures,minutes,secondes,hAlarme,minutesAlarme);
  lcd.home();
  lcd.write(heure);
  delay(1000); //attendre une seconde
  secondes++;
}

void pshiiiit(){
  digitalWrite(PIN_LED,HIGH);
  delay(1000);
  digitalWrite(PIN_LED,LOW);
  secondes++;
}

String getMsgInfo(String data, char separator, int index){
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void getMsg(){
  String messageComplet;
  String msg;
  String fonction;
  //message du type FONCTION_MSG??? ==> les "???" indiquent la fin du message
  
  while (BTSerial.available()){
    messageComplet = BTSerial.readString();
    Serial.println(messageComplet);
  }
  while (Serial.available()){
    messageComplet = Serial.readString();
    BTSerial.println(messageComplet);
  }

  //mes variables
  fonction = getMsgInfo(messageComplet, '_', 0);
  Serial.println(fonction);
  msg = getMsgInfo(messageComplet, '_', 1);
  Serial.println(msg);
  
  if(fonction == "user"){
    getionBluetooth(msg);
  }
  else if(fonction == "heures"){
    hAlarme = msg.toInt();
  }
  else if(fonction == "minutes"){
    minutesAlarme = msg.toInt();
  }
  else if(fonction == "pshit"){
    pshiiiit();
  }
}


void getionBluetooth(String nom){
  lcd.setCursor(0,1);
  lcd.print(nom);
}
