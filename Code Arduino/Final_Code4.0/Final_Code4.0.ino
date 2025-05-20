
//Encodeur~~~~
#define encoder0PinA  2  //CLK 
#define encoder0PinB  3  //DT 
#define Switch 4 // Switch 

int Pos = 1; //correspond au menu surligné de 1 à 5
int Validation = 0; //passe à 1 lorsque le bouton de l'encodeur est appuyé ou lorsqu'on touche au flex sensor
bool SwitchState = HIGH; //récupère l'état du bouton de l'encodeur (ou du flex sensor) par défaut il est HIGH

//Menu~~~~
String menu1 = "Capteur"; //menu pour les mesures classiques de résistance avec le capteur graphite
String menu2 = "Flex"; // menu pour les mesures du flex sensor
String menu3 = "Mesure continue"; //menu pour réaliser la calibration du capteur graphite
String menu4 = "Informations"; //menu nous aiguillant vers l'appli puisqu'il est difficile d'écrire beaucoup d'informations sur l'écran OLED
String menu5 = "Creatrices"; //crédit

unsigned long previousMillis = 0;

//OLED~~~~
#include <Adafruit_SSD1306.h>

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

//Potentiometre~~~~
#include <SPI.h>

const byte csPin = 10; //MCP42100 chip select pin
const int maxPositions = 256; //wiper can move from 0 to 255 = 256 positions
const long rAB = 50000; // pot resistance between terminals A and B
const byte rWiper = 125; //125 ohms pot wiper resistance
const byte pot0 = 0x11; //pot0 addr // B0001 0001

#define MCP_WRITE 0b00010001; // command byte format xxCCxxPP, CC command, PP pot number (01 if selected)

//Valeurs du circuit~~~~
int R3 = 100000;
int Vcc = 5;
int R1 = 100000;
int R5 = 10000;
double Vadc;
float Resistance;
long R2;

//Module bluetooth~~~~
#include <SoftwareSerial.h>
#define rxPin 7 //Broche 11 en tant que RX, � raccorder sur TX du HC-05
#define txPin 8 //Broche 10 en tant que RX, � raccorder sur TX du HC-05
#define baudrate 9600
SoftwareSerial mySerial(rxPin ,txPin); //D�finition du software serial

//Flex sensor~~~~
const int flexPin = A1;      // Pin connected to voltage divider output

const float VCC = 5;
const float R_DIV = 47000.0;  // resistor used to create a voltage divider
const float flatResistance = 25000.0; // resistance when flat
const float bendResistance = 100000.0;  // resistance at 90 deg
int ADCflex;
float Vflex;
float Rflex;
float angle;


void setPot(int addr, int pospot){
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  pospot = constrain (pospot, 0, 255); //limit wiper setting to range of 0 to 255
  digitalWrite(csPin, LOW); //select chip
  SPI.transfer(addr); // configure target pot with wiper position
  SPI.transfer(pospot);
  digitalWrite(csPin, HIGH); //de-select chip
  //print pot resistance between wiper and B terminal
  R2 = ((rAB * pospot) / maxPositions) + rWiper;
  Serial.print("Wiper pos : ");
  Serial.print(pospot);
  Serial.print("  R2 : ");
  Serial.print(R2);
  Serial.println("ohms");
}

void setup() {
  Serial.begin(9600);
  // Initialisation potentiometre
  digitalWrite(csPin, HIGH); // chip select default to de-selected
  pinMode(csPin, OUTPUT); // configure chip select as output
  SPI.begin();
  // Initialisation Bluetooth
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  mySerial.begin(9600);
  //Initialisation flex sensor
  pinMode(flexPin, INPUT);
  // Initialisation encoder
  pinMode(encoder0PinA, INPUT);        
  pinMode(encoder0PinB, INPUT);  
  pinMode(Switch, INPUT);   

  attachInterrupt(0, doEncoder, RISING); 
  // Initialisation ecran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)){
    while(1);
  }  
}

void Flex_Sensor(){
  // Read the ADC and calculate voltage and resistance from it
  ADCflex = analogRead(flexPin);
  Vflex = ADCflex * VCC / 1024.0;
  Rflex = R_DIV * (VCC /Vflex - 1.0);
  Serial.print("Resistance : ");
  Serial.print(Rflex);
  Serial.println("ohms");
  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(1);
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  ecranOLED.print(F("Resistance flex"));
  ecranOLED.setCursor(0,15);
  ecranOLED.print(Rflex);
  ecranOLED.setCursor(40,15);
  ecranOLED.print(F("ohms"));
  ecranOLED.display();
}

void Calcul(){
  //Calcule la résistance au niveau du capteur graphite
  Vadc = analogRead(A0) * 5 /1023;
  Serial.print("Vadc = ");
  Serial.print(Vadc * 1024 / 5);
  Serial.print("Volts");
  Resistance = (1 + R3 / R2) * R1 * Vcc / Vadc - R1 -R5;
  double ResistanceM;
  ResistanceM = Resistance/1000000; //pour passer en Mega ohms
  mySerial.print(ResistanceM);
  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(1);
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  ecranOLED.print(F("Resistance capteur"));
  ecranOLED.setCursor(0,15);
  ecranOLED.print(Resistance);
  ecranOLED.setCursor(40,15);
  ecranOLED.print(F("ohms"));
  ecranOLED.display();
}
void loop() {
  //Affichage des différents menus
  Menu ();
}

void doEncoder() {
  //Permet de déplacer le "curseur" sur le menu en fonction de l'encodeur rotatoire
  if (Pos < 5 && digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==HIGH) {
    Pos++;
  } else if (Pos > 1 && digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==LOW) {
    Pos--;
  }
  else if (Pos == 5 && digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==HIGH){
    Pos = 1;
  }
  else if (Pos == 1 && digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==LOW){
    Pos = 5;
  } 
  
}

void Menu(){
  unsigned long currentMillis = millis ();
  
  SwitchState = digitalRead(Switch); //récupère la valeur du bouton de l'encodeur
  if (SwitchState == LOW){
    Validation = 1;
    SwitchState = HIGH;
  }

  
  if (currentMillis - previousMillis >= (500)){
    previousMillis = currentMillis;
    ecranOLED.clearDisplay();
    ecranOLED.setTextSize(1);
    ecranOLED.setCursor(0, 0);
    ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK); //on réinitialise l'affichage de l'écran OLED
    
    
    if (Pos==1){
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(menu1);
      ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      ecranOLED.println(menu2);
      ecranOLED.println(menu3);
      ecranOLED.println(menu4);
      ecranOLED.println(menu5);
      ecranOLED.display();
      if (Validation == 1){
        setPot(pot0, 1); //potentiometre à un niveau bas car le capteur graphite ne fonctionne pas bien 
        Calcul();
        delay(1000);
        Validation = 0;
      }
      
    }
    else if (Pos==2){
      ecranOLED.println(menu1);
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(menu2);
      ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      ecranOLED.println(menu3);
      ecranOLED.println(menu4);
      ecranOLED.println(menu5);
      ecranOLED.display();
      if (Validation == 1){
        Flex_Sensor();
        delay(1000);
        Validation = 0;
      }
    }
    else if (Pos==3){
      ecranOLED.println(menu1);
      ecranOLED.println(menu2);
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(menu3);
      ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      ecranOLED.println(menu4);
      ecranOLED.println(menu5);
      ecranOLED.display();
      if (Validation == 1){
        ecranOLED.clearDisplay();
        ecranOLED.setTextSize(1);
        ecranOLED.setCursor(0, 0);
        ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);      
        ecranOLED.println(F("Selectionner le menu Capteur"));     
        ecranOLED.setCursor(0, 20);
        ecranOLED.println(F("Appuyez sur le flex sensor"));
        ecranOLED.setCursor(0, 40);  
        ecranOLED.println(F("Le temps de la mesure"));   
        ecranOLED.display();
        delay(3000);
        Validation = 0;
      }
    }
    else if (Pos == 4){
      ecranOLED.println(menu1);
      ecranOLED.println(menu2);
      ecranOLED.println(menu3);
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(menu4);
      ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      ecranOLED.println(menu5);
      ecranOLED.display();
      if (Validation == 1){
        ecranOLED.clearDisplay();
        ecranOLED.setTextSize(1);
        ecranOLED.setCursor(0, 0);
        ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);      
        ecranOLED.println(F("Une application"));
        ecranOLED.setCursor(0, 20);
        ecranOLED.print(F("bluetooth"));
        ecranOLED.setCursor(0,40);
        ecranOLED.print(F("est disponible")); //la fonction F() permet d'utiliser moins de RAM    
        ecranOLED.display();
        delay(2000);
        Validation = 0;
      }
    }
    else if (Pos == 5){
      ecranOLED.println(menu1);
      ecranOLED.println(menu2);
      ecranOLED.println(menu3);
      ecranOLED.println(menu4);
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(menu5);
      ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      ecranOLED.display();
      if (Validation == 1){
        ecranOLED.clearDisplay();
        ecranOLED.setTextSize(1);
        ecranOLED.setCursor(0, 0);
        ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);      
        ecranOLED.println(F("Projet realise par"));
        ecranOLED.setCursor(0, 20);
        ecranOLED.print(F("Sofia et Melissande")); //la fonction F() permet d'utiliser moins de RAM      
        ecranOLED.display();
        delay(2000);
        Validation = 0;
      }        
    }
    
  }

}

