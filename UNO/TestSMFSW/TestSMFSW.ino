/*!
**	\file TestSMFSW.ino
**	\brief Test de la carte Arduino UNO avec:
**			- 1x RGB LED
**			- 3x LEDs
**			- 2x Push Buttons
**			- 2x Sensors (Photo resistor & Variable resistor)
**/

//#define USE_MSTIMER2		// Si défini, utilisera le timer de la lib MsTimer2

/* ******** */
/* Includes */
/* ******** */
#if defined(USE_MSTIMER2) // Si le symbole USE_MSTIMER2 est défini
	#include <MsTimer2.h>
	//! \warning Pins 5 and 6: controlled by timer0
	//! \warning Pins 9 and 10: controlled by timer1
	//! \warning Pins 11 and 3: controlled by timer2
#endif

/* ******* */
/* Defines */
/* ******* */
#define DEF_COMM_PERIOD_RATE	1000U		//!< 1s

#define DEF_MSTIMER2_PERIOD		250U		//!< 250ms timer

#define RES_ADC					1023U		//!< Resolution du DAC de 10bits

// Macros de comparaison de temps �coul� depuis la valeur de timer sauvegard�e
#define TIME_COMP_SUP(v, t)  ((unsigned int) ((unsigned int) TIME_COUNT - (unsigned int) v) > (unsigned int) ((unsigned long) t / DEF_MSTIMER2_PERIOD))
//!< Tests if \b v (a Timer save variable) has reached time lapse stated in \b t (ms) 
#define TIME_COMP_INF(v, t) ((unsigned int) ((unsigned int) TIME_COUNT - (unsigned int) v) < (unsigned int) ((unsigned long) t / DEF_MSTIMER2_PERIOD))
//!< Tests if \b v (a Timer save variable) has not reached time lapse stated in \b t (ms) 

#define size_of_obj(obj, type)   (sizeof(obj)/sizeof(type)) //!< Connaitre le nombre d'élements d'un objet \b obj d'un \b type donné
//! \warning ne fonctionne que pour des tableaux (même taille


/* **************************** */
/* Global Variables & Constants */
/* **************************** */
static const int	PWM_Pins[3] = { 9, 10, 6 };		//!< Pins used for PWM
static const int	LED_Pins[3] = { 11, 5, 13 };	//!< Pins used for LEDs
static const int	AN_Pins[2] = { A0, A1 };		//!< Pins used for Analogic Inputs

int			PhotoRes_Val = 0, VarRes_Val = 0;

volatile unsigned long	TIME_COUNT = 0;  //!< Par tranches de (DEF_MSTIMER2_PERIOD)ms


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
	
	// Test LED (µc up & running)
	pinMode(LED_Pins[2], OUTPUT);   // Onboard LED

	// Init Fading Pin
	initFadingPin();

	#if defined(USE_MSTIMER2) // Si le symbole USE_MSTIMER2 est défini
		// Init & Launch Timer
		MsTimer2::set(DEF_MSTIMER2_PERIOD, isrLedflash);
		MsTimer2::start();
	#endif
  
	// PWM Pins
	pinMode(PWM_Pins[0], OUTPUT);   // Red
	pinMode(PWM_Pins[1], OUTPUT);   // Green
	pinMode(PWM_Pins[2], OUTPUT);   // Blue
	
	// PB Pins
	initPBs();
	
	// Analog Pins
	pinMode(AN_Pins[0], INPUT);     // Photores
	pinMode(AN_Pins[1], INPUT);     // Varres
	
	// Serial Port
	initSCI();
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
	// Ne mettre que des variables à réinitialiser à chaque cycle ici
	//static unsigned int PeriodComm = 0; // Pourquoi ne fonctionne pas déclaré en static ici?
	int Temp;

	updateTimers();

	// Lecture des BPs
	acquirePBs();
	gestionFading();

	// Lecture valeurs ADC
	PhotoRes_Val = analogRead(AN_Pins[0]);
	VarRes_Val = analogRead(AN_Pins[1]);
	
	// Conversion résultat Resistance variable
	Conv_ADC_to_BYTE(VarRes_Val, &Temp);
	
	// Ajustements de LEDs
	for (int i; i < size_of_obj(PWM_Pins, int); i++)
	{
		analogWrite(PWM_Pins[i], Temp);
	}
	
	// Conversion résultat Resistance variable
	Conv_ADC_to_BYTE(PhotoRes_Val, &Temp);
	analogWrite(LED_Pins[1], Temp);
}


/*!
**	\brief Conversion d'une valeur ADC 10bits vers un BYTE (8bits)
**
**	\param [in]		val - Valeur à convertir
**	\param [in,out]	res - Pointeur de résultat de la valeur convertie
**	\return nothing
**/
void Conv_ADC_to_BYTE(int val, int *res)
{
	// Limitation de la valeur à convertir
	unsigned int temp = min(RES_ADC, max(0, val));
	// Résultat du passage de 10 à 8bits (avec complémentation)
	*res = (int) ((RES_ADC - temp) >> 2);
}


/*!
**	\brief Fonction permettant de générer l'appel d'évènements sur comparaison d'intervalle de temps
**	\return nothing
**/
void updateTimers(void)
{
	// Eléments statiques à la fonction, déclarés tout le temps du déroulement du soft, initialisés uniquement au démarrage
	static unsigned int CommInterval = 0;
	static unsigned int LedFlashInterval = 0;
	
	unsigned int TempMillis = (unsigned int) millis();
	
	if (TempMillis - CommInterval > DEF_COMM_PERIOD_RATE)
	{
		CommInterval = TempMillis;	// Sauvegarde de la valeur de temps courante
		comm(); // Envoi vers SCI
	}
  
	if (TempMillis - LedFlashInterval > DEF_MSTIMER2_PERIOD)
	{
		LedFlashInterval = TempMillis;	// Sauvegarde de la valeur de temps courante
		#if (!defined(USE_MSTIMER2)) // Si le symbole USE_MSTIMER2 n'est pas défini, on simule l'interruption en appelant la fonction lorsque la condition est valide
			isrLedflash();
		#endif
	}
}

