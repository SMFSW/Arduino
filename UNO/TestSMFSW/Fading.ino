/*!
**	\file Fading.ino
**	\brief Module de gestion de fading sur une sortie (objet)
**/

#define DEF_TPS_INC_MS	20					//!< Intervalle de mise à jour de la sortie de Fading (périodiquement toutes les 150ms)

static const int		FadingPin = 3;		//!< LED pin useb by fading function

unsigned int			FadingVal = 0;		//!< Valeur courante de Fading
unsigned int			FadingTarget = 0;	//!< Valeur finale de Fading

static signed char		FadingInc = 0;		//!< Valeur de l'incrément de Fading (peut Ãªtre négatif)
static signed char  StepFading = 0;
static boolean			BP0done, BP1done;	//!< Variables de traitement d'appui sur les touches déjà réalisé

static unsigned char StepValues[5] = { 0, 64, 128, 192, 255 }; //!< Steps de lumière

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
	static unsigned long	TempoInc = 0;			//!< Variable statique de sauvegarde du temps de la dernière mise à jour de la sortie

	int						IndexStep = size_of_obj(StepValues, unsigned char) - 1;

	//!< Gestion PB0 (- Fading)
	if (	(getPB0() == HIGH)
		&&	(BP0done == false))
	{
		BP0done = true;

		if (--StepFading < 0)	{ StepFading = 0; }
		FadingTarget = StepValues[StepFading];

		FadingInc = -1;

		TempoInc = millis();
	}
	else if (getPB0() == LOW)	{ BP0done = false; }

	//!< Gestion PB1 (+ Fading)
	if (	(getPB1() == HIGH)
		&&	(BP1done == false))
	{
		BP1done = true;

		if (++StepFading >= IndexStep)	{ StepFading = IndexStep; }
		FadingTarget = StepValues[StepFading];

		FadingInc = 1;

		TempoInc = millis();
	}
	else if (getPB1() == LOW)	{ BP1done = false; }

	//!< Maj sortie
	if (	(millis() - TempoInc > DEF_TPS_INC_MS)
		&&	(FadingTarget != FadingVal))
	{
		FadingVal += FadingInc;
		analogWrite(FadingPin, FadingVal);
		TempoInc = millis();
	}
}

