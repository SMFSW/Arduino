
#define DEF_TAILLE_SCI_IN    64      //!< Taille réservée pour le buffer d'entrée SCI

static char        breakoutChar = ';';   //!< Declare ASCII value of tab as string breakout

static String      SCIIn = "";       //!< Declare a new string 
static boolean      SCIbreakout;      //!< Defines if an acquired SCI string breakout char is reached
static int        SCIInNbChar = 0;    //!< Inutilisé, sert juste à compter le nombre de char recus
static int       (*process)(String);

void setupSerial()
{
  process = msgSerial;
  
  Serial.begin(57600);
  
  Serial.println(ProjectName);
  Serial.print("Running software version: ");
  Serial.println(SWVersion);
  Serial.print("./>");
}

void taskSerial()
{
  if (SCIbreakout == true)
  {
    Serial.println(SCIIn);
    Serial.print("$/>");
    int rem = SCIIn.length();
    
    (void) msgSerial(SCIIn);
    
    // Clean treated part of serial internal buffer
    SCIIn.remove(0, rem);
    SCIbreakout = false;
    
    Serial.println("");
    Serial.print("./>");
  }
}

/*!
**  \brief Called whenever a new data has been received on RX
**  \remark This routine is run between each time loop() runs, no delay here set in here
**  \note Multiple bytes of data may be available
**/
void serialEvent(void)
{
  if (Serial.available())
  {
    char inChar = (char) Serial.read();

    SCIIn += inChar;

    if ( (++SCIInNbChar > DEF_TAILLE_SCI_IN-1) || (inChar == breakoutChar) )
    {
      SCIbreakout = true;
    }
  }
}

/*!
**  \brief write breakoutChar
**
**  \param[in] breakout - New ASCII digit as breakout
**  \return nothing
**/
inline void setBreakoutChar(char breakout)
{
  breakoutChar = breakout;
}

/*!
**  \brief reads breakoutChar
**
**  \return breakoutChar
**/
inline char getBreakoutChar()
{
  return (char) breakoutChar;
}

