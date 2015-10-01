/*!
**  \file Fading.ino
**  \brief Module de gestion de fading sur une sortie (objet)
**
**  \todo Voir pb lib MsTimer1 (uncomment MsTimer1::start)
**  \todo Check pourquoi Fading Target peut faire le tour
**/

#define DEF_TPS_INC_MS  150

static const char    FadingPin = 3;  //!< LED pin

unsigned char FadingVal = 0;  //!< Valeur courante de Fading
unsigned char FadingTarget = 0; //!< Valeur finale de Fading

static signed char   FadingInc = 0;  //!< Valeur de l'incrément (peut être négatif)
static unsigned long TempoInc;
static boolean       BP0done, BP1done;

void initFadingPin(void)
{
  pinMode(FadingPin, OUTPUT);
  
  // Init Default Timer
  //MsTimer1::set(100, isrTick);
}

void gestionFading(void)
{
  if (   (getPB0() == HIGH)
      && (BP0done == false))
  {
      BP0done = true;
      FadingInc = 1;
      FadingTarget = min(255, (FadingTarget + 25U));
      TempoInc = millis();
  }
  else if (getPB0() == LOW) { BP0done = false; }
  
  if (   (getPB1() == HIGH)
      && (BP1done == false))
  {
      BP1done = true;
      FadingInc = -1;
      FadingTarget = max(0, (FadingTarget - 25U));
      TempoInc = millis();
  }
  else if (getPB1() == LOW) { BP1done = false; }

  
  if (    (millis() - TempoInc > DEF_TPS_INC_MS)
      &&  (FadingTarget != FadingVal))
  {
    FadingVal += FadingInc;
    analogWrite(FadingPin, FadingVal);
    TempoInc = millis();
  }
}

/*void isrTick(void)
{
  if (FadingTarget != FadingVal)
  {
    FadingVal += FadingInc;
    analogWrite(FadingPin, FadingVal);
  }
  else
  {
    MsTimer1::stop();
  }
}*/
