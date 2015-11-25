/*!
**  \brief Called whenever a new data has been received on RX
**  \param[in] In - String in
**  \param[in] pRem - pointer to where
**  \return nothing
**/
static String ToSend;

int msgSerial(String In)
{
	int NbChar = In.length();
    
	// if ToSend at ToSend.length() is breakout char, remove it
	if (In.charAt(NbChar - 1) == getBreakoutChar())  { In.remove(NbChar-1); }
	//Serial.println(In);

  ToSend += "DEBUG INFOS:\n";
  for (int i = 0 ; i < 3 ; i++)
  {
    ToSend += "Strip";
    ToSend += i+1;
    ToSend += " :\n";
    dbgStrip(i);
  }
  Serial.print(ToSend);

	return NbChar;
}

inline void dbgStrip(uint8_t strip)
{
	WS2812Strip * pStrip;

	switch (strip)
	{
		case 0: { pStrip = &Strip; } break;
		case 1: { pStrip = &Strip2; } break;
		case 2: { pStrip = &Strip3; } break;
		default: break;
	}
  
  for (int i = 0 ; i < 3 ; i++)
  {
    uint32_t * pColor;
    
    switch (i)
    {
      case 0:
      {
        ToSend += "Color Back: ";
        pColor = &pStrip->colorBack;
      }
      break;
      
      case 1:
      {
        ToSend += "Color Front: ";
        pColor = &pStrip->colorFront;
      }
      break;
      
      case 2:
      {
        ToSend += "Color Pix: ";
        pColor = &pStrip->colorPix;
      }
      break;
      
      default:  break;
    }
    
    ToSend += pStrip->Red(*pColor);
    ToSend += "\t";
    ToSend += pStrip->Green(*pColor);
    ToSend += "\t";
    ToSend += pStrip->Blue(*pColor);
    ToSend += "\t\t";
  }
  
  ToSend += "\n";
  
  //if DimTab
 ToSend += "Dim Tab: ";
  for (int i = 0 ; i < 12/*getSize(pStrip->DimPix)*/ ; i++)
  {
    ToSend += pStrip->DimPix[i];
    ToSend += "\t";
  }

  ToSend += "\n";

  ToSend += "Mode Color: ";
  ToSend += pStrip->ControlCol.Mode;
    ToSend += "\tStatus: ";
    ToSend += pStrip->ControlCol.Status;
  ToSend += "\tSteps: ";
  ToSend += pStrip->ControlCol.steps;
  ToSend += "\tidx: ";
  ToSend += pStrip->ControlCol.idx;
  ToSend += "\tDir: ";
  ToSend += pStrip->ControlCol.Dir;

  ToSend += "\n";

  ToSend += "Mode Dim: ";
  ToSend += pStrip->ControlDim.Mode;
    ToSend += "\tStatus: ";
    ToSend += pStrip->ControlDim.Status;
  ToSend += "\tSteps: ";
  ToSend += pStrip->ControlDim.steps;
  ToSend += "\tidx: ";
  ToSend += pStrip->ControlDim.idx;
  ToSend += "\tDir: ";
  ToSend += pStrip->ControlDim.Dir;
  ToSend += "\n\n";
}

