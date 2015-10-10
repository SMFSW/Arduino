/*!
**	\file SCI.ino
**	\brief code de gestion de la communication série
**	\details	 Afin d'implémenter le module de debug via SCI dans un programme, il convient de:
**					- Appeler la fonction initSCI (dans setup) avec comme paramètre l'adresse du tableau de constantes d'adresses des valeurs que l'on veut pouvoir récupérer via la Rs
**					- Appeler la fonction commDebug (dans loop) afin d'éxécuter le code de gestion des transmissions Rs de debug
**
**	\todo Implémenter les queries suivant le tableau VarRegs
**	\todo Implémenter la lecture de variables en donnant une adresse via la comm
**/

#define DEF_COMM_PERIOD_RATE	1000U		//!< 1s

#define DEF_COMM_BAUD_RATE		9600U		//!< Vitesse de communication Rs (9600 baud)

#define DEF_TAILLE_SCI_IN		50			//!< Taille réservée pour le buffer d'entrée SCI


static char				breakoutChar = '*';		//!< Declare ASCII value of tab as string breakout

static const int **		pTabRegistres;			//!< Pointeur sur le tableau de registres pour le debug

static String			SCIIn = "";				//!< Declare a new string 
static boolean			SCIbreakout;			//!< Defines if an acquired SCI string breakout char is reached
static int				SCIInNbChar = 0;		//!< Inutilisé, sert juste à compter le nombre de char reÃ§us


/*!
**	\brief Redéfinit le char de traitement de chaÃ®ne
**
**	\return nothing
**/
void setBreakoutChar(char breakout)
{
	breakoutChar = breakout;
}


/*!
**	\brief Initialise le port série 
**
**	\details Init des paramètres de l'interface SCI de debug:
**				- Initialise la vitesse de communication
**				- Réserve de la place pour le buffer de message reçu
**				- Initialise le pointeur de tableau d'adresses de variables à monitorer
**				- Envoie un message sur la liaison série
**/
void initSCI(const int ** pTab)
{
	Serial.begin(DEF_COMM_BAUD_RATE);
	//while (!Serial) { };  // wait for serial port to connect. Needed for Leonardo only

	SCIIn.reserve(DEF_TAILLE_SCI_IN); // Réserve 50 characteres pour le buffer SCI

	pTabRegistres = pTab;

	Serial.println("Welcome to the debug session!");
	Serial.print("Running software version: ");
	Serial.println(SWVersion);
}


/*!
**	\brief Cadence l'appel à la fonction de comm pour le debug
**
**	\return Nothing
**/
void commDebug()
{
	static unsigned int CommInterval = 0;	//!< Variable statique d'intervalle entre transmissions sur la Rs
	//static boolean CallComm = false;		//!< Variable statique flag de déclenchement transmission

	unsigned int TempMillis = (unsigned int) millis();

	if (TempMillis - CommInterval > DEF_COMM_PERIOD_RATE)
	{
		CommInterval = TempMillis;	// Sauvegarde de la valeur de temps courante
		//CallComm = true;
		comm();		// Envoi vers SCI
	}

	/*if (CallComm == true)
	{
		comm();		// Envoi vers SCI
		CallComm = false;
	}*/
}

/*!
**	\brief Sends debug infos on SCI
**
**	\details Protocole: (TBD)
**/
void comm(void)
{
	String ToSend = "";				//!< Déclaration d'une chaÃ®ne de caractères temporaire à la fonction
 
	Serial.print("ADC VAL: ");
	Serial.print(*pTabRegistres[0]);

	if (SCIbreakout == true)
	{
		ToSend += "\tQuery: ";
		ToSend += SCIIn;
		int NbChar = ToSend.length();

		// Faire cette opération tout le temps supprime un char qui peut être autre que le breakout char
		// Réception d'une trame trop longue par exemple
		// if contenu de ToSend at ToSend.length() est breakout char
		if (ToSend.charAt(NbChar - 1) == breakoutChar)
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
**	\brief Called whenever a new data has been received on RX
**	\remark This routine is run between each time loop() runs, no delay here set in here
**	\note Multiple bytes of data may be available
**/
void serialEvent(void)
{
	while (Serial.available())
	{
		char inChar = (char) Serial.read();

		SCIIn += inChar;

		if (	(++SCIInNbChar > DEF_TAILLE_SCI_IN-1)
			||	(inChar == breakoutChar))
		{
			SCIbreakout = true;
		}
	}
}

