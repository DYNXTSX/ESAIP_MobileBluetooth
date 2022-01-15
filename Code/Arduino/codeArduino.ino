#include <SoftwareSerial.h>   // librairie pour creer une nouvelle connexion serie max 9600 baud
#include <LiquidCrystal.h>
#define PIN_LED 8

SoftwareSerial BTSerial(6, 7); // RX | TX 
LiquidCrystal lcd(12,11,5,4,3,2);

int heures, minutes, secondes, hAlarme = 15, minutesAlarme = 45;
char heure[50] = "";

void setup()
{
  pinMode(6, INPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  Serial.println("==> Entrer une commande :");
  BTSerial.begin(9600);  // HC-05 9600 baud 
  pinMode(PIN_LED, OUTPUT);
  lcd.begin(16,2);
}

void loop()
{
  gestionHeure();
  int i = 0;
  char someChar[32] = {0};
  String messageComplet;
  String msg;
  String fonction;
    
  if(Serial.available()) {
    do{
      someChar[i++] = Serial.read();
      delay(3);
    }while (Serial.available() > 0);
    BTSerial.println(someChar);
    Serial.println(someChar);
  }
  
  while(BTSerial.available()){
    messageComplet = BTSerial.readString();

    fonction = getMsgInfo(messageComplet, '_', 0);
    msg = getMsgInfo(messageComplet, '_', 1);
    
    switch (fonction.toInt()){
      case 1: //Afficher l'utilisateur connecté
        getionBluetooth(msg);
        Serial.println("L'utilisateur connecté est : ");
        Serial.println(msg);
        break;
        
      case 2: //changer l'heure de l'alarme
        hAlarme = msg.toInt();
        break;
        
      case 3: //changer les minutes de l'alargmes
        minutesAlarme = msg.toInt();
        break;
        
      case 4: //changer l'heure
        heures = msg.toInt();
        break;

      case 5: //changer les minutes
        minutes = msg.toInt();
        break;
        
      case 6:
        Serial.println("Actionnement du diffuseur");
        pshiiiit();
        break;
        
      default:
        Serial.println("Aucune commande connu");
        break;
    }
  
    Serial.println(messageComplet);
  }
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

void getionBluetooth(String nom){
  lcd.setCursor(0,1);
  lcd.print(nom);
}
