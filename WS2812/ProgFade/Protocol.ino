/*!
**  \brief Called whenever a new data has been received on RX
**  \param[in] In - String in
**  \param[in] pRem - pointer to where
**  \return nothing
**/
int msgSerial(String In)
{
	int NbChar = In.length();

	// if ToSend at ToSend.length() is breakout char, remove it
	if (In.charAt(NbChar - 1) == getBreakoutChar())  { In.remove(NbChar-1); }
	//Serial.println(In);

	//if DimTab
	for (int i = 0 ; i < NbLEDsStrip1 ; i++)
	{
	Serial.print(Strip.DimPix[i]);
	Serial.print("\t");
	}

	return NbChar;
}


