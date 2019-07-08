// MagnetSwitcher_2019_3coil.ino
// Alex Hoffman, University of Washingoton
// This code runs on an Arduino Mega to drive the MagnetSwitcher PCB I designed for the CIMS lab
// To interact with this code, use the python script (maybe GUI in future) located in the github repo
// Always use caution when operating high voltage electronics

#include "MagnetSwitcher_2019_3coil.h" //defines pins, functions to turn on/off H bridge

// Variables to hold command input
String commandBuffer = "";
String commandIn = "";
volatile boolean commandReady = false;

// Variables for state machine and timing
long lastLoop = 0;
long delayValue = 500000;
int currentCircuit = 0;
int circuitDirection = 1; //use to reverse rotation direction
boolean circuitOn = false; // leaving off while testing

void setup() {
  commandBuffer.reserve(200);
  
  pinMode(PIN_N2_A, OUTPUT);
  pinMode(PIN_N1_A, OUTPUT);
  pinMode(PIN_P2_A, OUTPUT);
  pinMode(PIN_P1_A, OUTPUT);
  pinMode(PIN_N2_B, OUTPUT);
  pinMode(PIN_N1_B, OUTPUT);
  pinMode(PIN_P2_B, OUTPUT);
  pinMode(PIN_P1_B, OUTPUT);
  pinMode(PIN_N2_C, OUTPUT);
  pinMode(PIN_N1_C, OUTPUT);
  pinMode(PIN_P2_C, OUTPUT);
  pinMode(PIN_P1_C, OUTPUT);
  circuitHalt('A');
  circuitHalt('B');
  circuitHalt('C');
  Serial.begin(115200);
  delay(STARTUP_DELAY);
}

// Continuously loops
void loop() {
  // First: If micros has overflowed, make sure to reset lastLoop and halt the circuit
  // also if the circuit is stopped it halts constantly
  if (micros() < lastLoop || circuitOn == false) {
    noInterrupts();
    lastLoop = micros();
    circuitHalt('A');
    circuitHalt('B');
    interrupts();
  }

  // Check to see if time elapsed > delayValue
  if (micros()-lastLoop > delayValue) {
    // time to rotate magnetic field
    lastLoop = micros();
    circuitHalt('A'); // For safety
    circuitHalt('B');
    switch (currentCircuit) {
      case 0: currentCircuit = (currentCircuit + circuitDirection+4)%4; // moves to case 1 if forward, case 3 if reverse
              circuitActivateForward('A');
              circuitHalt('B');
              break;
      case 1: currentCircuit = (currentCircuit + circuitDirection)%4;
              circuitHalt('A');
              circuitActivateForward('B');
              break;
      case 2: currentCircuit = (currentCircuit + circuitDirection)%4;
              circuitActivateReverse('A');
              circuitHalt('B');
              break;
      case 3: currentCircuit = (currentCircuit + circuitDirection)%4;
              circuitHalt('A');
              circuitActivateReverse('B');
              break;
    }
  }
  
  while (Serial.available()) {
      commandBuffer += (char)Serial.read();
  }
  if (commandBuffer.length() >= COMMAND_LENGTH) { // check if full command received
        commandReady = true;
        commandIn = commandBuffer.substring(0,COMMAND_LENGTH); // this works for exactly 8 chars. improve by just grabbing 1st 8 chars
        commandBuffer = ""; // ignore possibility of pending command after this one
  }
  
  if (commandReady) {
    if (handleCommand(commandIn) == false) {
      //Serial.println(commandIn);
      Serial.println("Error, halting circuit");
      circuitHalt('A');
      circuitHalt('B');
      circuitOn = false;
    }
    commandReady = false;
  }
}


// Command table:
// STOP -- stops looping
// START -- starts looping
// FREQ XX -- sets the frequency in Hz, where XX is the desired frequency

boolean handleCommand(String in) {
  if (in.substring(0,4) == "STOP") {
    Serial.print("Halting circuit");
    circuitOn = false;
    circuitHalt('A');
    circuitHalt('B');
    return true;
  }
  if (in.substring(0,5) == "START") {
    Serial.print("Starting circuit");
    circuitOn = true;
    return true;
  }
  if (in.substring(0, 5) == "FREQ ") {
    String freq = in.substring(5);
    float convertedValue = freq.toFloat();
    if (convertedValue > 0) {
      float newDelay = 250000.0/convertedValue;
      Serial.print(newDelay);
      circuitOn = true;
      delayValue = newDelay;
      return true;
    }
  }
  if (in.substring(0,4) == "DIR ") {
    String dir = in.substring(4,5);
    if (dir == "+") {
      Serial.print("+ DIR");
      circuitDirection = 1;
      return true;
    } if (dir == "-") {
      Serial.print("- DIR");
      circuitDirection = -1;
      return true;
    }
  }
  return false;
}
