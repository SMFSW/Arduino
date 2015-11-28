#include <MsTimer2.h>

#define   digiINPin   2

#define    pi  3.14159f       // Aprrox pi constant

// For 11/20/R5 (mm/%/inches) tires ( 1inch = 2.54 cm)
// Total is (2* 11*20%) + (6 * 25,4)
#define   diamRim      ((float) 5 * 25.4f)  // diametre de jante (inches to mm)
#define   lenTire   ((float) ((uint16_t) 11 * (uint16_t) 2) * ((float) 20 / 100.0f)) // Largeur de pneu (complète -> x2)

//#define   diamTire      ((float) (diamRim + lenTire) / 1000.0f) // diametre pneu (en m)
#define   diamTire    0.26f // en m

#define   circTire   ((float) (pi * diamTire)) // circonférence pneu

#define   lenReference (float) ((circTire / 2.0f))  // La référence est une demie circonférence de pneu

typedef struct StructSpeedSensor{
const unsigned int      tpsMes; // en ms, acquisition du nombre d'impulsions 
volatile unsigned int   cnt;        // nb d'impulsions (1 TOUR = 2 IMPULSIONS)
volatile unsigned int   pulsesData; // pulses in the last acquired time window
volatile boolean        dataReady;  // vrai si on peut aller lire pulsesData
};

static volatile StructSpeedSensor cptSpeed = {1000, 0,0,false};   // Allonger le temps d'acquisition pour plus de précision (au détriment de la vitesse)

unsigned int  vitKmh = 0; // Variable partagée avec I2C_7Seg

//static float circTire;
//static float lenReference;

float calcTire(uint8_t len, uint8_t ratio, uint8_t radius)  // To test before implementing in setup
{
// For 11/20/R5 (mm/%/inches) tires ( 1inch = 2.54 cm)
// Total is (2* 11*20%) + (6 * 25,4)
  float dRim = ((float) radius * 25.4f); // diametre de jante (inches to mm)
  float lTire = ((float) ((uint16_t) len * 2) * ((float) ratio / 100.0f)); // Largeur de pneu (complète -> x2)
  float dTire = ((float) (dRim + lTire) / 1000.0f); // diametre pneu (en m);

  return dTire;
  //return 0.26f;
}

void setup_DIGI_Speed()
{
  //circTire = calcTire(11, 20, 6);
  pinMode(digiINPin, INPUT_PULLUP);
  // It compteur tours
  attachInterrupt(digitalPinToInterrupt(digiINPin), isrIncPulses, FALLING);
  
  // Timer compteur de tours
  MsTimer2::set(cptSpeed.tpsMes, isrGetPulses); // 1000ms period
  MsTimer2::start();
}

void loop_DIGI_Speed()
{
  unsigned int pulses;
  float vitesse;  // variable pour la vitesse en km/h
  
  if (getPulses(&pulses) == false)  { return; }
  
  Serial.print("impuls:\t");
  Serial.print(pulses);
  
  vitesse = (float) pulses * (float) lenReference;   // en m/s
  //vitesse *= 3600.0f; // en m/h
  //vitesse /= 1000.0f; // en km/h
  float coef = 3.6f / (float) (cptSpeed.tpsMes / 1000); // Afin d'ajuster suivant le temps d'acquisition
  vitesse = vitesse * coef; // en km/h
  
  unsigned int vitesseE = (unsigned int) vitesse; // vitesseE : partie entiere de la vitesse
  unsigned int vitesseD = (unsigned int) ((float) ((vitesse - vitesseE) * 100)); //vitesseD : la partie decimale de la vitesse

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

