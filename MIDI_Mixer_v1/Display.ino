/*
    Criteria:   Display control

    Version:    12
    Update:     Option dissapears
       Fix:     Option not appearing; display() on end line fixes it
    Date:       13:14 13/5/2023
    By:         Franek Talarek
*/

int logoDisplay()   // Display logo when booting
{
  display.clearDisplay();
  display.setTextColor(WHITE);
    
  for (int i = 0; i < 64; i++)
  {
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(20, 10);
    display.print("MELK");
    display.fillRect(i * 2, 0, display.width(), display.height(), BLACK);
    display.display();
    
    PotUpdate();    //Run PotUpdate to replace the empty data with the actual values
  }
}

int Display()
{
  if (OptBtnPressed == 1)
  {   
    if (lastChangedOptBtn <= 1)
    {
      display.setTextSize(1);  
      display.setCursor(0, 0);              
      display.print("Note");               
    
      display.setTextSize(2);
      if (lastChangedOptBtn == 0)
      {
        display.setCursor(0, 12);                     
        display.print("-");
      }
      else
      {
        display.setCursor(0, 12);                     
        display.print("+");
      }
      display.setCursor(16, 12); 
      display.print("8");
      display.setTextSize(4);
      display.setCursor(45, 0); 
      display.print(startNote);
    } 
      
    if (lastChangedOptBtn >= 2)
    {
      display.setTextSize(1);  
      display.setCursor(0, 0);              
      display.print("Chnl");  
      
      display.setTextSize(2);
      if (lastChangedOptBtn == 2)
      {
        display.setCursor(0, 12);                     
        display.print("-");
      }
      else
      {
        display.setCursor(0, 12);                     
        display.print("+");
      }
      display.setCursor(16, 12); 
      display.print("1");
      display.setTextSize(4);
      display.setCursor(45, 0); 
      display.print(startChannel);
    }
    
    display.display();   // Display the content on the OLED screen.
  }

  else
  {
    // Display button values
    if (lastChangedObj == "Btn")
    {
      display.setTextSize(1); 
      display.setCursor(0, 0);                    
      display.print("NO");    
      display.setCursor(15, 0);                    
      display.print(startNote+lastChangedBtn);    
    
      display.setCursor(0, 15);                    
      display.print("CHN");    
      display.setCursor(20, 15);                   
      display.print(startChannel); 
    
      display.fillRect(0, 25, btnState[lastChangedBtn]*128, display.height(), WHITE);
      
      display.setTextSize(3);                       
      display.setCursor(40, 0);                     
      display.print(BtnStates[btnState[lastChangedBtn]]);    
    }
  
    // Display potentiometer values
    if (lastChangedObj == "Pot")
    {
    
      display.fillRect(0, 25, potMAverage[lastChangedPot], display.height(), WHITE);   
      
      display.setTextSize(1); 
      display.setCursor(0, 0);                    
      display.print("CC");    
      display.setCursor(15, 0);                    
      display.print(startNote+lastChangedPot);    
    
      display.setCursor(0, 14);                    
      display.print("CHN");    
      display.setCursor(20, 14);                   
      display.print(startChannel);    
      
      
      display.setTextSize(3);                       
      display.setCursor(40, 0);                     
      display.print(potMAverage[lastChangedPot]);     
    } 
  }
  display.display();   // Display the content on the OLED screen.
  display.clearDisplay();
}
