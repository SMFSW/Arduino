/*!
**	\file StringManipulations.ino
**	\brief Fonctions de manipulation de chaînes de caractères (de type String)
**/


/*!
**	\brief Convertit un String de type ASCII codé en décimal vers un integer de type long
**	\remark La cohérence des données de String \b StrIn est testée à l'intérieur de la fonction
**
**	\param [in] StrIn - objet de type String à convertir
**	\param [in, out] LongOut - pointeur sur la variable qui contiendra la valeur convertie
**
**	\return Etat de la conversion (toujours valide pour le moment, LongOut à 0 si conversion échouée)
**	\retval true - Conversion réalisée et accessible dans \b LongOut
**	\retval false - Conversion échouée (données d'entrée incohérentes (présence de char autres que correspondants à du décimal))
**/
boolean DecimalASCIItoLong(String StrIn, unsigned long * LongOut)
{
	*LongOut = StrIn.toInt();
	return true;
}


/*!
**	\brief Convertit un String de type ASCII codé en Hexadécimal vers un integer de type long
**	\remark La cohérence des données de String \b StrIn est testée à l'intérieur de la fonction
**
**	\param [in] StrIn - objet de type String à convertir
**	\param [in, out] LongOut - pointeur sur la variable qui contiendra la valeur convertie
**
**	\return Etat de la conversion
**	\retval true - Conversion réalisée et accessible dans \b LongOut
**	\retval false - Conversion échouée (données d'entrée incohérentes (présence de char autres que correspondants à de l'hexadécimal))
**/
boolean HexASCIItoLong(String StrIn, unsigned long * LongOut)
{
	int				i;
	int				NbChar = StrIn.length();
	unsigned long	TempOut = 0;
	unsigned char	TempChar;
	
	for (i = NbChar ; i > 0 ; i--)
	{
		TempChar = StrIn.charAt(NbChar-1);
		
		if (TempChar >= '0' && TempChar <= '9')			{ TempChar -= '0'; }	// Si entre 0x30 et 0x39, on retranche 0x30 (0 en ASCII)
		else if (TempChar >= 'A' && TempChar <= 'F')	{ TempChar -= 'A'; }	// Si entre 0x41 et 0x46, on retranche 0x41 (A en ASCII)
		else											{ return false; }		// Si une autre valeur, on ne peut pas convertir, donc on sort
		
		TempOut += TempChar;	// On ajoute la valeur calculée à la valeur temporaire de type integer
	}
	
	*LongOut = TempOut;		// Recopie de la valeur de type integer calculée dans la variable de sortie
	return true;
}


/*!
**	\brief Convertit un integer de type long vers un String codé en Hexadécimal
**
**	\param [in] LongIn - objet de type long à convertir
**	\param [in, out] StrOut - pointeur sur le String qui contiendra la chaîne convertie
**
**	\return Etat de la conversion (conversion toujours valide)
**	\retval true - Conversion réalisée et accessible dans \b LongOut
**	\retval false - Conversion échouée
**/
boolean LongtoHexASCII(unsigned long LongIn, String StrOut)
{
	StrOut = String(LongIn, HEX);
	return true;
}


/*!
**	\brief Convertit un integer de type long vers un String codé en Décimal
**
**	\param [in] LongIn - objet de type long à convertir
**	\param [in, out] StrOut - pointeur sur le String qui contiendra la chaîne convertie
**
**	\return Etat de la conversion (conversion toujours valide)
**	\retval true - Conversion réalisée et accessible dans \b LongOut
**	\retval false - Conversion échouée
**/
boolean LongtoDecimalASCII(unsigned long LongIn, String StrOut)
{
	StrOut = String(LongIn, DEC);
	return true;
}

