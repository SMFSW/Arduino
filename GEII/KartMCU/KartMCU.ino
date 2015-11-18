#include <SeqTimer.h>

SeqTimer  timI2C;
//SeqTimer  timRS;    // Si nécessaire
String SWVersion = "v0.0.1";  //!< Version en cours du soft


#define   pulseGENPin   8
static boolean pulseGENVal = HIGH;  //!< Variable statique de mémorisation d'état de TestLED (HIGH ou LOW)
static int pulseGENCpt = 0;


void setup()
{
  // Test LED (µc up & running)
  pinMode(pulseGENPin, OUTPUT);   // Onboard LED
  
  Serial.begin(9600);

  setup_SPI();          // Voir pour supprimer delay d'init
  setup_I2C_7Seg();
  setup_I2C_Bargraph();

  setup_DIGI_Speed();
  setup_Encoder();
  
  timI2C.init(256);
}

void loop()
{
  loop_DIGI_Speed();
  loop_SPI();

  if (timI2C.getTimer() == true)
  {
    loop_I2C_7Seg();
    loop_I2C_Bargraph();
  }

  int genPeriod = max(5, loop_Encoder());
  
  pulseGEN(genPeriod);
}


void pulseGEN(unsigned int cnt)
{
  if (++pulseGENCpt >= cnt)
  {
    pulseGENCpt = 0;
    digitalWrite(pulseGENPin, pulseGENVal);
    pulseGENVal = !pulseGENVal; // Invert LED state
  }
}

