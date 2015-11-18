#include <MsTimer2.h>


#define   digiINPin   2

const unsigned int  tpsMes = 1000; // en ms, acquisition du nombre d'impulsions 

typedef struct StructSpeedSensor{
volatile unsigned int   cnt; // nb d'impulsions (1 impulsion = 1 tour)
volatile unsigned int   pulsesData; // pulses in the last acquired time window
volatile boolean        dataReady;  // vrai si on peut aller lire pulsesData
};

static volatile StructSpeedSensor cptSpeed = {0,0,false};

unsigned int  vitKmh = 0; // Variable partagée avec I2C_7Seg

void setup_DIGI_Speed()
{
  pinMode(digiINPin, INPUT_PULLUP);
  // It compteur tours
  attachInterrupt(digitalPinToInterrupt(digiINPin), isrIncPulses, FALLING);
  
  // Timer compteur de tours
  MsTimer2::set(tpsMes, isrGetPulses); // 1000ms period
  MsTimer2::start();
}

void loop_DIGI_Speed()
{
  unsigned int pulses;
  float vitesse;  // variable pour la vitesse en km/h
  
  if (getPulses(&pulses) == false)  { return; }
  
  Serial.print("impuls:\t");
  Serial.print(pulses);
  
  vitesse = pulses * 3.14f / 30 / 0.130f;   // calcul de la vitesse 
  vitesse = vitesse * 3.6f;
  
  unsigned int vitesseE = (unsigned int) vitesse; // vitesseE : partie entiere de la vitesse
  unsigned int vitesseD = (unsigned int) ((float) ((vitesse - vitesseE) * 10)); //vitesseD : la partie decimale de la vitesse

  vitKmh = vitesseE;
  
  Serial.print("\tVit:\t");
  Serial.print(vitesseE);
  Serial.print(".");
  Serial.print(vitesseD);
  Serial.println("km/h");
  
#if 0
  lcd.clear();          // effacer tous les choses avant d'afficher la vitesse
  if (vitesseE <= 9)
  {
    afficheNombre(vitesseE, 1);   // afficher le chiffre de 0 à 9 en position 1
  }
  else if (vitesseE <= 99)
  {
    afficheNombre(vitesseE, 0);   // afficher le chiffre de 10 à 99 en position 0 (à gauche)
  } 
  
  afficheChiffre(10, 2);        // afficher la virgule
  
  afficheChiffre(vitesseD, 3);  
#endif
}

boolean getPulses(unsigned int * res)
{
  boolean memo = cptSpeed.dataReady;
  *res = cptSpeed.pulsesData;
  cptSpeed.dataReady = false;
  return memo;
}

void isrIncPulses() 
{
  cptSpeed.cnt++;
}

void isrGetPulses() 
{
  cptSpeed.pulsesData = cptSpeed.cnt;
  cptSpeed.cnt = 0;
  cptSpeed.dataReady = true;
}

