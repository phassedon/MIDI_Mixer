/*
    Criteria:   Updates variables from external objects

    Version:    5
    Update:     Added option dissapear
    Date:       12:31 13/5/2023
    By:         Franek Talarek
*/

int BtnUpdate()   // Update multiplexer buttons
{
  for (int Btn = 0; Btn < NBtns; Btn++)   // Loop through each pin of the multiplexer.
  {
    if (mux.digitalRead(Btn) == 0 and btnState[Btn] == 0)  // Check if the button connected to the current pin is pressed.
    {
      btnState[Btn] = 1; 
      lastChangedBtn = Btn;
    }
    if (mux.digitalRead(Btn) == 1 and btnState[Btn] == 1)
    {
      btnState[Btn] = 0;
      lastChangedBtn = Btn;
    }
    if (btnState[Btn] != btnLastState[Btn])
    {
      lastChangedObj = "Btn"; 
      MIDIMath();
    }
    btnLastState[Btn] = btnState[Btn];
  }  
}


int OptBtnUpdate()    //  Update option buttons
{   
  unsigned long OptBtnPressedT; 
  for(int Btn = 0; Btn < NOptBtns; Btn++)
  {
    if (digitalRead(optBtnPin[Btn]) == 0)   // If the button is pressed:
    {  
      lastChangedOptBtn = Btn;
      OptBtnPressed = 1;
      OptBtnPressedT = millis();
        
      if(optBtnState[Btn] == 0)             // If stored state is Rel,
      {
        optBtnState[Btn] = 1;               // set it to Press.
        OptBtnPressTime[Btn] = millis();    // Store when the button was pressed.

        if (Btn == 0 and startNote >= 8)
        {
          startNote = startNote - 8;
        }
        else if (Btn == 1 and startNote < 120)
        {
          startNote = startNote + 8;
        }
        else if (Btn == 2 and startChannel > 1)
        {
          startChannel--;
        }
        else if (Btn == 3 and startChannel < 16)
        {
          startChannel++;
        }
      } 

      OptBtnHoldTime[Btn] = millis()-OptBtnPressTime[Btn];    // Store how long the button has been pressed.
    
      if (OptBtnHoldTime[Btn] >= 500 and optBtnState[Btn] == 1)    // If stored state is Press for more than 200ms,
      {
        optBtnState[Btn] = 2;                                                                     // set it to Hold.                                      
      } 
      
      if (OptBtnHoldTime[Btn] >= 1500 and optBtnState[Btn] == 2)    // If stored state is Press for more than 200ms,
      {
        optBtnState[Btn] = 3;                                                                     // set it to Hold.                                      
      }                 
    }
    
    if (digitalRead(optBtnPin[Btn]) == 1)    // If the button is not pressed:
    {
      optBtnState[Btn] = 0;                  // Store it.
    }
    
    optBtnLastState[Btn] = optBtnState[Btn];    // Save the Option Button state for next reading.   
  }
  
  if (OptBtnPressedT < millis()-millisView)
  {
    OptBtnPressed = 0;   
  }
}


int PotUpdate()   // Update potentiometers
{
  for (int i = 0; i < NPots; i++)   // Loop through all potentiometers 
  { 
      // --- Potentiometer average --- //
    potTotal[i] = potTotal[i] - potReadings[i][potReadIndex[i]];    // Remove the oldest reading from the total
    potReadings[i][potReadIndex[i]] = analogRead(potInputPin[i]);   // Read the current potentiometer value and store it
    potTotal[i] = potTotal[i] + potReadings[i][potReadIndex[i]];    // Add the new reading to the total
    potReadIndex[i] = potReadIndex[i] + 1;                          // Increment the index for the next reading
    potAverage[i] = potTotal[i] / NReadings;                        // Calculate the average potentiometer value
    if (potReadIndex[i] >= NReadings){ potReadIndex[i] = 0; }       // If the index is equal to or greater than the number of readings, reset it to 0    
      // --- Potentiometer average --- //

      // --- Value map --- //
    potMAverage[i] = map(potAverage[i], potInputPinMinRange[i], potInputPinMaxRange[i], 0, 127); // Map the average to an 8 bit value; MIDI

    if (potAverage[i] < potLastAverage[i] - 3 or potAverage[i] > potLastAverage[i] + 3) // Check if the pot value has changed by more than 3 since the last iteration
    {
      lastChangedPot = i;       // if so, update the last changed object to the current potentiometer index.
      lastChangedObj = "Pot";    // And set the last changed object to a potentiometer.
      MIDIMath();
    }
    potLastAverage[i] = potAverage[i];    // Set the last mapped average value to the current value for the next iteration
      // --- Value map --- //
  }
}
