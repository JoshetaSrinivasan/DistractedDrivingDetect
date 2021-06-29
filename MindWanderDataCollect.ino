// MindWander Data Collection 

// Description: Reads EEG data using the Brain Library (https://github.com/kitschpatrol/Brain), 
//              Captures when button is pressed appending a "*" at the end of EEG data, 
//              Buzz at random intervals for probing. 
                  
// More info: github  
// Author: Josheta Srinivasan, 2020

#include <Brain.h>

// Set up the brain parser, pass it the hardware serial object you want to listen on.
Brain brain(Serial);

// Innitialize Pins and Variables //
// BUTTON 
int buttonPin = 9;                                 // Connected Arduino Pin
int buttonVal = 1;                                 // 0 if button is pressed; 1 if button is not pressed
// BUZZER
int buzzerPin = 10;                                // Connected Arduino Pin
int intMin = 60, intMax = 120;                     // Max and Min of buzzing interval (seconds)
unsigned long interval = random(intMin,intMax);    // Buzzer Interval
int buzzFreq = 2500;                               // Buzz Frequency (Hz)
int buzzDur = 300;                                 // Buzz Duration (milliseconds)
// TRACKING TIME
unsigned long prevMillis = 0;                      // Time at prev point
unsigned long currMillis;                          // Time at current point
// currMillis and prevMillis are used to calcultae time passed from last buzz

// NOTE: buzzer resistor is 100 ohms 

void setup() {
    // Start the hardware serial.
    Serial.begin(9600);                          // Open Serial 
    pinMode(buttonPin, INPUT_PULLUP);            // Innitialize button
    pinMode(buzzerPin, OUTPUT);                  // Innitialize buzzer
}

void loop() {
    // Expect packets about once per second.
    // The .readCSV() function returns a string (well, char*) listing the most recent brain data, in the following format:
    // "signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma"
    // If button is pressed, a "*" is appended to the end of the brain data. 

    // BUZZER INTERVAL LOGIC //
    unsigned long currMillis = millis();
    
    if ((unsigned long)(currMillis-prevMillis) > interval*1000) // If buzz interval has passed
    {
       tone(buzzerPin, buzzFreq, buzzDur);                      // Buzz 
       prevMillis = currMillis;                                 // Update the 'prev' time point
       interval = random(intMin,intMax);                        // Assign new buzz interval
    }


    // EEG DATA READ //
    buttonVal = digitalRead(buttonPin);                        // Check if button is pressed or not
    if (brain.update()) {                       
        if (!buttonVal)                                        // Append "*" to brain data if button pressed
        {
          Serial.print(brain.readCSV());
          Serial.print(",");
          Serial.println("*");
        } 
        else                                                  // If not pressed, simply print brain data    
        {
          Serial.println(brain.readCSV());
        }
        
    }
}
