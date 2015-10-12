/*!
**	\file ADC.ino
**	\brief Gestion du module ADC (Analog to Digital channels)
**/


/*!
**  \brief Conversion d'une valeur ADC 10bits vers un BYTE (8bits)
**
**  \param [in]   val - Valeur à convertir
**  \param [in,out] res - Pointeur de résultat de la valeur convertie
**  \return nothing
**/
void Conv_ADC_to_BYTE(int val, int *res)
{
  // Limitation de la valeur à convertir
  unsigned int temp = min(RES_ADC, max(0, val));
  // Résultat du passage de 10 à 8bits (avec complémentation)
  *res = (int) ((RES_ADC - temp) >> 2);
}


