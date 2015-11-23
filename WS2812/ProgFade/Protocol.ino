/*!
**  \brief Called whenever a new data has been received on RX
**  \param[in] In - String in
**  \param[in] pRem - pointer to where
**  \return nothing
**/
int msgSerial(String In)
{
	int NbChar = In.length();
    String ToSend;
    
	// if ToSend at ToSend.length() is breakout char, remove it
	if (In.charAt(NbChar - 1) == getBreakoutChar())  { In.remove(NbChar-1); }
	//Serial.println(In);

	//if DimTab
	for (int i = 0 ; i < NbLEDsStrip1 ; i++)
	{
		ToSend += Strip.DimPix[i];
		ToSend += "\t";
	}

	ToSend += "\n";

	ToSend += "Mode Color: ";
	ToSend += Strip.ControlCol.Mode;
    ToSend += "\tStatus: ";
    ToSend += Strip.ControlCol.Status;
	ToSend += "\tSteps: ";
	ToSend += Strip.ControlCol.steps;
	ToSend += "\tidx: ";
	ToSend += Strip.ControlCol.idx;
	ToSend += "\tDir: ";
	ToSend += Strip.ControlCol.Dir;

	ToSend += "\n";

	ToSend += "Mode Dim: ";
	ToSend += Strip.ControlDim.Mode;
    ToSend += "\tStatus: ";
    ToSend += Strip.ControlDim.Status;
	ToSend += "\tSteps: ";
	ToSend += Strip.ControlDim.steps;
	ToSend += "\tidx: ";
	ToSend += Strip.ControlDim.idx;
	ToSend += "\tDir: ";
	ToSend += Strip.ControlDim.Dir;

	Serial.print(ToSend);

	return NbChar;
}


