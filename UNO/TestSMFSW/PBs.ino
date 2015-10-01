/*!
**  \file PBs.ino
**  \brief Module de gestion de PBs (objet)
**/


/* ******* */
/* Defines */
/* ******* */
#define GESTION_REBONDS_SUPPLEMENTAIRE		0	//!< Pre-comp to do own more filtering


/* **************************** */
/* Global Variables & Constants */
/* **************************** */
static const int	PB_Pins[2] = { 2, 4 };		//!< Pins used for Push Buttons

static union {
	unsigned char BPbyte;		//!< Valeur de PushButtons sur 8bits
	struct {
		// LSB Less Significant Byte
		boolean BP0 :1;			//!< Valeur du bit BP0 (LSB)
		boolean BP1 :1;			//!< Valeur du bit BP1
		boolean     :6;			//!< bits non utilisés
		// MSB Less Significant Byte
	}BPbits;
}PushButtons;


/*!
**	\brief Initialise les input push buttons 
**	
**	\details Init pour les x PB definis aux pins dans le tableau PB_Pins[]:
**				- Initialise pin en input
**				- Active la résistance de pull-up interne au µ
**/
void initPBs(void)
{
	for (int i = 0; i < size_of_obj(PB_Pins, int); i++)
	{
    pinMode(PB_Pins[i], INPUT_PULLUP);     // PBx avec pull interne
		//pinMode(PB_Pins[i], INPUT);			// PBx
		//digitalWrite(PB_Pins[i], HIGH);		// Enable internal pull-up resistor
	} 
}

/*!
**	\brief Donne l'état du bit BP0
**	
**	\return Etat de BP0
**	\returnval	HIGH - BP appuyé
**	\returnval	HIGH - BP relaché
**/
boolean getPB0(void)
{
	return PushButtons.BPbits.BP0;
} 

/*!
**	\brief Donne l'état du bit BP1
**	
**	\return Etat de BP1
**	\returnval	HIGH - BP appuyé
**	\returnval	HIGH - BP relaché
**/
boolean getPB1(void)
{
	return PushButtons.BPbits.BP1;
}

/*!
**	\brief Donne l'état des différents BPs sur 8bits
**	
**	\return Etat de BP0
**	\returnval	HIGH - BP appuyé
**	\returnval	HIGH - BP relaché
**/
unsigned char getPBs(void)
{
	return PushButtons.BPbyte;
}

/*!
**	\brief Procédure d'acquisition des différents BPs
**	\remark Doit être appelée régulièrement pour réaliser des acquisitions correctes
**	
**	\details Actions (tous les 50 cycles de µ) sur chaque PB à récupérer:
**				- acquisiton INPUT & xor (afin d'inverser l'état de l'entrée du fait du pull-up)
**				- GETSION DES REBONDS (option de précompilation) ?
**					- non -> Mise à jour de l'union PushButtons
**					- oui -> Test et maj compteur, puis Mise à jour de l'union PushButtons
**	
**/
void acquirePBs(void)
{
	// Variables statiques à la fonction (mais toujours définies à leur adresse en RAM)
	//	Initialisées comme des variables globales -> à chaque init du soft
	static int IntervalAcq = 0;								// Intervalle d'acquisition (en cycles de loop)
	static boolean MemoBP[sizeof(PB_Pins)] = { LOW, LOW };
	static int TimerBP[sizeof(PB_Pins)] = { 0, 0 };
	
	// Variables temporaires détruites lors de retour de la fonction
	//	Initialisées à chaque passage dans la fonction
	boolean TempBP;
	
	// Test interval d'acquisition
	if (++IntervalAcq >= 50)
	{
		IntervalAcq = 0;	// Raz compteur interval d'acquisition (pour le lancer périodiquement)
		
		// Boucle sur les différents PBs
		for (int i = 0; i < size_of_obj(PB_Pins, int); i++)
		{
			TempBP = digitalRead(PB_Pins[i]);
			
			#if !GESTION_REBONDS_SUPPLEMENTAIRE
        bitClear(PushButtons.BPbyte, i);
				//PushButtons.BPbyte &= ~(1U << i);	// RAZ bit à mettre à jour
				PushButtons.BPbyte |= (TempBP ? LOW : HIGH) << i;	// MAJ bit avec valeur lue
			#else
				if (TempBP == MemoBP[i])			// Si valeur mémorisée identique à la valeur lue
				{
					if (++TimerBP[i] > 10)			// Incrémentation et test du compteur pour mise à jour
					{
						PushButtons.BPbyte &= ~(1U << i);	// RAZ bit à mettre à jour
						PushButtons.BPbyte |= (TempBP ? LOW : HIGH) << i;	// MAJ bit avec valeur lue
						TimerBP[i] = 0;						// RAZ du compteur (permet de réécrire la valeur périodiquement même si inchangée)
					}
				}
				else
				{
					if (TimerBP[i] > 0)		TimerBP[i]--;		// Décrément du TimerBP tant que le timer n'est pas à 0
					else					MemoBP[i] = TempBP;	// Recopie de la valeur lue vers la valeur mémorisée
				}
			#endif
		}
	} 
}
