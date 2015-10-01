/*!
**	\file Fading.ino
**	\brief Module de gestion de fading sur une sortie (objet)
**
**	\issue Check pourquoi Fading Target peut faire le tour
**/

#define DEF_TPS_INC_MS	150		//!< Intervalle de mise à jour de la sortie de Fading

static const int		FadingPin = 3;		//!< LED pin

unsigned char			FadingVal = 0;		//!< Valeur courante de Fading
unsigned char			FadingTarget = 0;	//!< Valeur finale de Fading

static signed char		FadingInc = 0;		//!< Valeur de l'incrément (peut être négatif)
static boolean			BP0done, BP1done;	//!< Variables de traitement d'appui sur les touches déjà réalisé


/*!
**	\brief Initialise la sortie de fading
**	\return nothing
**/
void initFadingPin(void)
{
	pinMode(FadingPin, OUTPUT);
}


/*!
**	\brief Procédure de gestion du fading sur la sortie PWM
**	
**	\details	- Incrémente la valeur de 25 lors de l'appui de PB0
**				- Décrémente la valeur de 25 lors de l'appui de PB1
**				-> Gestion du fading sur la sortie (soumis à un timer d'incrément)
**	\return nothing
**/
void gestionFading(void)
{
	static unsigned long	TempoInc = 0;			//!< Variable de sauvegarde du temps de la dernière mise à jour de la sortie
	
	// Gestion PB0
	if (	(getPB0() == HIGH)
		&&	(BP0done == false))
	{
		BP0done = true;
		FadingInc = 1;
		FadingTarget = min(255, (unsigned char) (FadingTarget + 25U));
		TempoInc = millis();
	}
	else if (getPB0() == LOW) { BP0done = false; }

	// Gestion PB1
	if (	(getPB1() == HIGH)
		&&	(BP1done == false))
	{
		BP1done = true;
		FadingInc = -1;
		FadingTarget = max(0, (unsigned char) (FadingTarget - 25U));
		TempoInc = millis();
	}
	else if (getPB1() == LOW) { BP1done = false; }


	// Maj sortie
	if (	(millis() - TempoInc > DEF_TPS_INC_MS)
		&&	(FadingTarget != FadingVal))
	{
		FadingVal += FadingInc;
		analogWrite(FadingPin, FadingVal);
		TempoInc = millis();
	}
}
