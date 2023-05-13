/*
    Criteria:   Sends MIDI messages

    Version:    3
    Update:     Simplified
       Fix:     Potentiometer ControlChange stuck on first
    Date:       23:12 12/5/2023
    By:         Franek Talarek
*/

int MIDIMath()
{ 
  if (lastChangedObj == "Btn")
  { 
      MIDI.sendNoteOn(startNote+lastChangedBtn, btnState[lastChangedBtn]*127, startChannel);
  }
  if (lastChangedObj == "Pot")
  {        
      MIDI.sendControlChange(startNote+lastChangedPot, potMAverage[lastChangedPot], startChannel); // cc number, cc value, midi channel
  }
}
