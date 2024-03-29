#define STARTUP_DELAY 2000
#define COMMAND_LENGTH 12
#define PIN_P2 11
#define PIN_P1 9
#define PIN_N1_N 10
#define PIN_N2_N 12

#define PMOS_ON HIGH // pulls H bridge gate low to turn on PMOS
#define PMOS_OFF LOW
#define NMOS_ON LOW // pulls H bridge gate high to turn on NMOS
#define NMOS_OFF HIGH

// Variables to hold command input
String commandBuffer = "";
String commandIn = "";
volatile boolean commandReady = false;

// Variables for state machine and timing
long lastLoop = 0;
long delayValue = 500000;
int currentCircuit = 0;
//boolean circuitOn = true;
boolean circuitOn = false; // leaving off while testing

void setup() {
  commandBuffer.reserve(200);
  
  pinMode(PIN_N2_N, OUTPUT);
  pinMode(PIN_N1_N, OUTPUT);
  pinMode(PIN_P2, OUTPUT);
  pinMode(PIN_P1, OUTPUT);
  digitalWrite(PIN_N2_N, NMOS_OFF);
  digitalWrite(PIN_N1_N, NMOS_OFF);
  digitalWrite(PIN_P2, PMOS_OFF);
  digitalWrite(PIN_P1, PMOS_OFF);

  Serial.begin(115200);
  delay(STARTUP_DELAY);
}

// shuts off current to coil, do not leave an nmos on...this can cause shoot through
void circuitHalt() {
  noInterrupts();
  digitalWrite(PIN_P1, PMOS_OFF);
  digitalWrite(PIN_P2, PMOS_OFF);
  digitalWrite(PIN_N2_N, NMOS_OFF);
  digitalWrite(PIN_N1_N, NMOS_OFF);
  interrupts();
}

// Turns on pair 1
void circuit_A() {
  noInterrupts();
  // These should be off
  digitalWrite(PIN_N2_N, NMOS_OFF);
  digitalWrite(PIN_P1, PMOS_OFF);
  // These should be on
  digitalWrite(PIN_P2, PMOS_ON);
  digitalWrite(PIN_N1_N, NMOS_ON);
  interrupts();
}

// Turns on pair 2
void circuit_B() {
  noInterrupts();
  // These should be off
  digitalWrite(PIN_N1_N, NMOS_OFF);
  digitalWrite(PIN_P2, PMOS_OFF);
  // These should be on
  digitalWrite(PIN_P1, PMOS_ON);
  digitalWrite(PIN_N2_N, NMOS_ON);
  interrupts();
}



void loop() {
  // First: If micros has overflowed, make sure to reset lastLoop and halt the circuit
  // also if the circuit is stopped it halts constantly
  if (micros() < lastLoop || circuitOn == false) {
    noInterrupts();
    lastLoop = micros();
    circuitHalt();
    interrupts();
  }

  //noInterrupts(); caused loop to fail
  // Check to see if time elapsed > delayValue
  if (micros()-lastLoop > delayValue) {
    
    lastLoop = micros();
    circuitHalt(); // For safety
    switch (currentCircuit) {
      case 0: currentCircuit = 1;
              circuit_A();
              break;
      case 1: currentCircuit = 2;
              circuitHalt();
              break;
      case 2: currentCircuit = 3;
              circuit_B();
              break;
      case 3: currentCircuit = 0;
              circuitHalt();
              break;
      
    }
    //interrupts();
  }
  
  while (Serial.available()) {
      commandBuffer += (char)Serial.read();
  }
  if (commandBuffer.length() >= COMMAND_LENGTH) { // check if full command received
        commandReady = true;
        commandIn = commandBuffer.substring(0,COMMAND_LENGTH); // this works for exactly 8 chars. improve by just grabbing 1st 8 chars
        commandBuffer = "";
  }
  
  if (commandReady) {
    if (handleCommand(commandIn) == false) {
      Serial.println(commandIn);
      Serial.println("Error, halting circuit");
      circuitHalt();
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
    Serial.println("Halting circuit");
    circuitOn = false;
    circuitHalt();
    return true;
  }
  if (in.substring(0,5) == "START") {
    Serial.println("Starting circuit");
    circuitOn = true;
    return true;
  }
  if (in.substring(0, 5) == "FREQ ") {
    String freq = in.substring(5);
    float convertedValue = freq.toFloat();
    if (convertedValue > 0) {
      float newDelay = 250000.0/convertedValue;
      Serial.println(newDelay);
      circuitOn = true;
      delayValue = newDelay;
      return true;
    }
  }


  return false;
}
