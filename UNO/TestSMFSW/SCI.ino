/*!
**	\file SCI.ino
**	\brief code de gestion de la communication série
**/

#define DEF_TAILLE_SCI_IN  50

static const int * VarRegs[10] =
{
  &PhotoRes_Val,
  &VarRes_Val,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
};

static char breakoutChar = '*';   // Declare ASCII value of tab as string breakout

static String SCIIn = "";   // Declare a new string 
static boolean SCIbreakout; // Defines if an acquired SCI string breakout char is reached
static int SCIInNbChar = 0; // Inutilisé, sert juste à compter le nombre de char reçus

/*!
**  \brief Initialise le port série 
**  
**  \details Init pour les x PB definis aux pins dans le tableau PB_Pins[]:
**        - Initialise pin en input
**        - Active la résistance de pull-up interne au µ
**/
void initSCI(void)
{
  Serial.begin(DEF_COMM_BAUD_RATE);
  //while (!Serial) { };  // wait for serial port to connect. Needed for Leonardo only
  Serial.println("Welcome to the debug session!");
  
  SCIIn.reserve(DEF_TAILLE_SCI_IN); // Réserve 50 characteres pour le buffer SCI
}

/*!
**	\brief Sends debug infos on SCI
**	
**	\details Protocole: (TBD)
**/
void comm(void)
{
	unsigned long LED_COUNT = 0;
	String ToSend = "";
 
	noInterrupts();
	LED_COUNT = TIME_COUNT;
	interrupts();
	
	Serial.print("LED_COUNT: ");
	Serial.print(LED_COUNT);
	
	Serial.print("\tVarRes_Val: ");
	Serial.print(*VarRegs[1]);
  
  Serial.print("\tFadingVal: ");
  Serial.print(FadingVal);
  
  Serial.print("\tPush Buttons: 0x");
  Serial.print(getPBs(), HEX);
  Serial.print(" b");
  Serial.print(getPBs(), BIN);

  if (SCIbreakout == true)
  {
    ToSend += "\tQuery: ";
    ToSend += SCIIn;
    int NbChar = ToSend.length();
    
    // Faire cette opération tout le temps supprime un char qui peut être autre que le breakout char
    // Réception d'une trame trop longue par exemple
    // if contenu de ToSend at ToSend.length() est breakout char
    {
      ToSend.remove(NbChar-1);
    }
    
    ToSend += "\tValue: ";
    ToSend += "Queries not available yet!!!";
    Serial.print(ToSend);
    
    // Clean serial internal variables
    //!< Voir pour supprimer uniquement la partie qui a été traitée
    //SCIIn.remove(0, NbChar);
    //SCIInNbChar -= NbChar;
    SCIIn.remove(0);
    SCIbreakout = false;
  }
  
  Serial.println("");
}

/*!
**  \brief Called whenever a new data has been received on RX
**  \remark This routine is run between each time loop() runs, no delay here set in here
**  \note Multiple bytes of data may be available
**/
void serialEvent(void)
{
  while (Serial.available() /*&& (SCIInNbChar < DEF_TAILLE_SCI_IN-1)*/)
  {
    char inChar = (char) Serial.read();

    SCIIn += inChar;

    if (   (++SCIInNbChar > DEF_TAILLE_SCI_IN-1)
        || (inChar == breakoutChar))
    {
      //SCIIn += '\0';
      SCIbreakout = true;
    }
  }
}
