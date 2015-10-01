/*!
**	\file TestSMFSW.ino
**	\brief Test de la carte Arduino UNO avec:
**			- 1x RGB LED
**			- 3x LEDs
**			- 2x Push Buttons
**			- 2x Sensors (Photo resistor & Variable resistor)
**/

String SWVersion = "v0.4";	//!< Version en cours du soft


//#include <Wire.h>		//!< Inclusion de Wire.h en vue de l'utilisation de l'I2C
//#include <EEPROM.h>	//!< Inclusion de EEPROM.h en vue de l'utilisation de la eeprom


//#define USE_MSTIMER2			//!< Si défini, utilisera le timer de la lib MsTimer2

/* ******** */
/* Includes */
/* ******** */
#if defined(USE_MSTIMER2) // Si le symbole USE_MSTIMER2 est défini
//	#include <MsTimer2.h>	// Semble inclure la lib même si le parser devrait ne pas l'inclure suivant l'option de pré-compilation
	//! \warning Pins 5 and 6: controlled by timer0
	//! \warning Pins 9 and 10: controlled by timer1
	//! \warning Pins 11 and 3: controlled by timer2
#endif

/* ******* */
/* Defines */
/* ******* */
#define DEF_MSTIMER2_PERIOD		250U		//!< 250ms timer

#define RES_ADC					1023U		//!< Resolution du DAC de 10bits
#define RES_PWM					255U		//!< Resolution des sorties PWM sur 8bits

// Macros de comparaison de temps écoulé depuis la valeur de timer sauvegardée
#define TIME_COMP_SUP(v, t)		((unsigned int) ((unsigned int) TIME_COUNT - (unsigned int) v) > (unsigned int) ((unsigned long) t / DEF_MSTIMER2_PERIOD))
//!< Tests if \b v (a Timer save variable) has reached time lapse stated in \b t (ms) 
#define TIME_COMP_INF(v, t)		((unsigned int) ((unsigned int) TIME_COUNT - (unsigned int) v) < (unsigned int) ((unsigned long) t / DEF_MSTIMER2_PERIOD))
//!< Tests if \b v (a Timer save variable) has not reached time lapse stated in \b t (ms) 

#define size_of_obj(obj, type)	((int) (sizeof(obj) / sizeof(type)))	//!< Connaitre le nombre d'élements d'un objet \b obj d'un \b type donné
//! \warning ne fonctionne que pour des tableaux (même taille entre les différents éléments)

#define INC_ROLLBACK_IDX(Idx, End, Start)   if (++Idx >= End) { Idx = Start; }	//!< macro permettant d'incrémenter un compteur tournant


/* **************************** */
/* Global Variables & Constants */
/* **************************** */
static const int		PWM_Pins[3] = { 9, 10, 6 };		//!< Pins used for PWM
static const int		LED_Pins[3] = { 11, 5, 13 };	//!< Pins used for LEDs
static const int		AN_Pins[2] = { A0, A1 };		//!< Pins used for Analogic Inputs

int						PhotoRes_Val = 0, VarRes_Val = 0;

volatile unsigned long	TIME_COUNT = 0;		//!< Par tranches de (DEF_MSTIMER2_PERIOD)ms


/*!\struct StructActionFlags
** \brief Structure de flags d'actions
**/
struct StructActionFlags{
	// LSB Less Significant Byte
	boolean ActLed		:1;			//!< Action Leds à réaliser
	boolean ActMajAna	:1;			//!< Action de mise à jour des entrées analogiques
	boolean	Act3		:1;			//!< bit non utilisé pour le moment
	boolean	Act4		:1;			//!< bit non utilisé pour le moment
	boolean	Act5		:1;			//!< bit non utilisé pour le moment
	boolean	Act6		:1;			//!< bit non utilisé pour le moment
	boolean	Act7		:1;			//!< bit non utilisé pour le moment
	boolean	Act8		:1;			//!< bit non utilisé pour le moment
	// MSB Less Significant Byte
}ActionFlags;

extern unsigned int		FadingVal;		//!< Valeur courante de Fading (déclarée dans un autre fichier)
extern unsigned int		FadingTarget;	//!< Valeur finale de Fading (déclarée dans un autre fichier)

static const int * VarRegs[10] =
{
	&PhotoRes_Val,					//!< Adresse de la variable 0 à monitorer
	&VarRes_Val,					//!< Adresse de la variable 1 à monitorer
	(const int *) &FadingTarget,	//!< Adresse de la variable 2 à monitorer
	(const int *) &FadingVal,		//!< Adresse de la variable 3 à monitorer
	NULL,							//!< Adresse de la variable 4 à monitorer
	NULL,							//!< Adresse de la variable 5 à monitorer
	NULL,							//!< Adresse de la variable 6 à monitorer
	NULL,							//!< Adresse de la variable 7 à monitorer
	NULL,							//!< Adresse de la variable 8 à monitorer
	NULL,							//!< Adresse de la variable 9 à monitorer
};	//!< Déclaration du tableau d'adresses de variables à monitorer (qui sera utilisé par le module SCI)


/*!
**	\brief The setup function is called once at startup of the sketch
**
**	\details Inits:
**				- Test LED pin
**				- RGB LED pins
**				- LED Fading
**				- PWM pins
**				- PB pins
**				- SCI port (Rs232)
**				- Timer (lib) si USE_MSTIMER2 defined 
**/
void setup()
{
	// LED Pins
	pinMode(LED_Pins[0], OUTPUT);   // LED1 Red
	pinMode(LED_Pins[1], OUTPUT);   // LED2 Blue

	// Test LED (Âµc up & running)
	pinMode(LED_Pins[2], OUTPUT);   // Onboard LED

	// Init Fading Pin
	initFadingPin();

	#if defined(USE_MSTIMER2) // Si le symbole USE_MSTIMER2 est défini
		// Init & Launch Timer
		MsTimer2::set(DEF_MSTIMER2_PERIOD, isrLedflash);
		MsTimer2::start();
	#endif

	// PWM Pins
	pinMode(PWM_Pins[0], OUTPUT);	// Red
	pinMode(PWM_Pins[1], OUTPUT);	// Green
	pinMode(PWM_Pins[2], OUTPUT);	// Blue

	// PB Pins
	initPBs();

	// Analog Pins
	pinMode(AN_Pins[0], INPUT);		// PhotoRes
	pinMode(AN_Pins[1], INPUT);		// VarRes

	// Serial Port
	initSCI(&VarRegs[0]);
}


/*!
**	\brief The loop function is called in an endless loop
**
**	\details Actions:
**				- ADC acquire
**				- Convert ADC
**				- Update PWM pins
**				- Time check to SCI communication (Rs232)
**/
void loop()
{
	int Temp;

	updateTimers();		// Mise à jour des flags d'actions

	acquirePBs();		// Lecture des BPs

	gestionFading();	// Gestion du fading sur la pin en question

	// Lecture valeurs ADC
	if (ActionFlags.ActMajAna == true)			// Si flag d'action pour la mise à jour des entrées analogiques
	{
		ActionFlags.ActMajAna = false;

		VarRes_Val = analogRead(AN_Pins[1]);
		Conv_ADC_to_BYTE(VarRes_Val, &Temp);	// Conversion résultat Resistance variable
		
		// Ajustements de la LED RGB (3 composantes à l'identique, génère du blanc)
		for (int i = 0; i < size_of_obj(PWM_Pins, int); i++)
		{
			analogWrite(PWM_Pins[i], Temp);
		}

		PhotoRes_Val = analogRead(AN_Pins[0]);
		Temp = map(PhotoRes_Val, 0, RES_ADC, RES_PWM, 0);	// Lit la valeur, la cale entre 0 et 1023 et la convertit de 255 à 0
		//Conv_ADC_to_BYTE(PhotoRes_Val, &Temp);	// Conversion résultat Photo Resistance
		analogWrite(LED_Pins[1], Temp);			// Ecriture vers la sortie PWM 
	}
	
	if (ActionFlags.ActLed == true)			// Si flag d'action pour les LEDs précedemment gérées en it
	{
		#if (!defined(USE_MSTIMER2)) // Si le symbole USE_MSTIMER2 n'est pas défini, on simule l'interruption en appelant la fonction lorsque la condition est valide
			isrLedflash();	// Maj blinking LED
		#endif
		
		ActionFlags.ActLed = false;
		//bitClear(ActionFlags.ActLed, 1);	// Reset flag action
	}
	
	commDebug();	// Appel du module de debug (par la comm série)
}


/*!
**	\brief Fonction permettant de générer l'appel d'évènements sur comparaison d'intervalle de temps
**	\return nothing
**/
void updateTimers(void)
{
	static unsigned int MajAnaInterval = 0;
	static unsigned int LedFlashInterval = 0;			//!< Variable statique à la fonction, déclaré tout le temps du déroulement du soft et initialisé uniquement au démarrage
	
	unsigned int TempMillis = (unsigned int) millis();	//!< Variable temporaire à la fonction, pris sur la pile au moment de l'éxécution de la fonction, puis libéré lors du retour de cette fonction (intialisé à chaque passage dans la fonction)
	
	if (TempMillis - MajAnaInterval > 32) // Mise à jour toutes les 32ms
	{
		MajAnaInterval = TempMillis;	// Sauvegarde de la valeur de temps courante
		ActionFlags.ActMajAna = true;
	}
	
	if (TempMillis - LedFlashInterval > DEF_MSTIMER2_PERIOD)
	{
		LedFlashInterval = TempMillis;	// Sauvegarde de la valeur de temps courante
		ActionFlags.ActLed = true;
	}
}



