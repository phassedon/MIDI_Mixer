/*
    Program:    Program for turning an Arduino into a MIDI Device (Not Class-Compliant)
    TODOs:      1.Arrays for easy pin management            [x]
                2.Potentiometer calibration variables.      [x]
                3.Stable potentiometer value.               [x]
                4.Functions for each process.               [x]   
                4.Files for function grouping.              [x] hi     
                
    Version:    1
    Update:     Added option dissapear
    Date:       12:31 13/5/2023
    By:         Franek Talarek
*/

////////////////////////////////////////
//Library initialization -->
#include <Wire.h>               // I2C library         by Arduino
#include <Adafruit_GFX.h>       // Graphics manager    by Adafruit
#include <Adafruit_SSD1306.h>   // I2C OLED Display    by Adafruit
#include <Adafruit_PCF8574.h>   // I2C Multiplexer     by Adafruit

#include <MIDI.h>               // MIDI Library        by Francois Best
MIDI_CREATE_DEFAULT_INSTANCE();   // Start MIDI device with default settigns

#define SCREEN_WIDTH 128                                                    // OLED display width, in pixels
#define SCREEN_HEIGHT 32                                                    // OLED display height, in pixels
#define OLED_RESET    -1                                                    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C                                                 // OLED display address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);   // Define display library varibles

Adafruit_PCF8574 mux;   // Start multiplexer library
////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
//Button and potentiometer variables -->

//Option Buttons -->  
const int NOptBtns = 4;                                   // Define number of buttons for different options
const int optBtnPin[NOptBtns] = {2, 3, 4, 5};             // Define button pins for the different options
int optBtnState[NOptBtns];                                // Store the option buttons state         [Rel, Press, Hold, Hold Long] [0, 1, 2, 3] (Hold not used)
int optBtnLastState[NOptBtns];                            // Store the option buttons last state    [Rel, Press, Hold, Hold Long] [0, 1, 2, 3] (Hold not used)
unsigned long OptBtnPressTime[NOptBtns];                  // Store when it has been pressed
unsigned long OptBtnHoldTime[NOptBtns];                   // Store how long it has been pressed    
bool OptBtnPressed;                                       // Store if any option button was pressed
int millisView = 800;                                     // How long the option is showed on display

//Buttons --> 
const int NBtns = 8;
bool btnState[NBtns];   
bool btnLastState[NBtns];
unsigned long lastBtnPress[NBtns]; 
String BtnStates[2] = {"OFF", "ON"};

//Potentiometers -->  
const int NPots = 4;
const int potInputPin[NPots] = {A0, A1, A2, A3};                        // Define Analog pins
const String potInputPinS[NPots] = {"A0", "A1", "A2", "A3"};            // Define Analog pins
const long potInputPinMaxRange[NPots] = {1022, 1022, 1022, 1022};       // Potentiometers maximum read, 0-1024
const long potInputPinMinRange[NPots] = {};                             // Potentiometers minimum read, 0-1024

//Potentiometer value stabilization -->
const int NReadings = 12;                 // How many values will be stored;  Lower the number, worse average. Higher the number, slower average.
int potReadings[NPots][NReadings];        // The readings from the pot input.
int potReadIndex[NPots];                  // Index for total pot value.
int potTotal[NPots];                      // Store Pot read.
int potAverage[NPots];                    // Store Pot average.
int potLastAverage[NPots];                // Store the last Pot average
int potMAverage[NPots];                   // Average converted to a 0-127 value.
 
// Variables to store last change -->
int lastChangedBtn;           // Store which Button was used last.
int lastChangedOptBtn;        // Store which Option Button was used last.
int lastChangedPot;           // Store which Potentiometer was used last.
String lastChangedObj;        // Store which Object has changed last.
////////////////////////////////////////////////////////////////////////////////////////////////////  


////////////////////////////////////////
//MIDI Start variables -->
int startNote = 0;
int startChannel = 1;
////////////////////////////////////////



void setup()
{
  Init();           // Initialize inputs and outputs; including communication protocols
  logoDisplay();    // Initialize the display and display the logo
}

void loop()
{
  BtnUpdate();          // Update buttons
  OptBtnUpdate();       // Update option buttons
  PotUpdate();          // Update potentiometers
  Display();            // Update screen
}


int Init()        // Initialize board communication and pin setups
{
  Serial.begin(115200);                             // Start serial communication for Serial-to-MIDI Standart.
  mux.begin(0x27, &Wire);                           // Initialize multiplexer
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);        // Initialize display
  
  for (int p = 0; p < NBtns; p++)       // Loop through each pin of the multiplexer.
  {
    mux.pinMode(p, INPUT_PULLUP);       // Set the mode of the current pin to input_pullup.
  }

  for (int p = 0; p < NOptBtns; p++)            // Loop through each option button pin.
  {
    pinMode(optBtnPin[p], INPUT_PULLUP);        // Set the mode of the current pin to input_pullup.
  }
}
