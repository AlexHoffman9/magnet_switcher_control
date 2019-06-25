#define STARTUP_DELAY 2000
#define LOOP_DELAY_U 100

#define PIN_P2 7
#define PIN_P1 8
#define PIN_N1_N 9
#define PIN_N2_N 10

#define PIN_CAP2 5
#define PIN_CAP1 4

// Variables to hold command input
String commandBuffer = "";
String commandIn = "";
volatile boolean commandReady = false;

// Variables for state machine and timing
long lastLoop = 0;
long delayValue = 500000;
int currentCircuit = 0;
boolean circuitOn = true;

void setup() {
  commandBuffer.reserve(200);
  
  noInterrupts();
  
  pinMode(PIN_N2_N, OUTPUT);
  pinMode(PIN_N1_N, OUTPUT);
  pinMode(PIN_P2, OUTPUT);
  pinMode(PIN_P1, OUTPUT);
  digitalWrite(PIN_N2_N, HIGH);
  digitalWrite(PIN_N1_N, LOW);
  digitalWrite(PIN_P2, LOW);
  digitalWrite(PIN_P1, LOW);
  
  
  Serial.begin(115200);
  delay(STARTUP_DELAY);
  //Serial.println("Initializing MagnaRotoTron 20XX MK I");
  //Serial.println("Daniel Luncasu (c) 2017");

  interrupts();

}

// Turns off all the MOSFETS
void circuitHalt() {
  noInterrupts();
  digitalWrite(PIN_N2_N, HIGH);
  digitalWrite(PIN_N1_N, HIGH);
  digitalWrite(PIN_P1, LOW);
  digitalWrite(PIN_P2, LOW);
  interrupts();
}

// Turns on pair 1
void circuit_A() {
  noInterrupts();
  // These should be off
  digitalWrite(PIN_N2_N, HIGH);
  digitalWrite(PIN_P1, LOW);
  // These should be on
  digitalWrite(PIN_P2, HIGH);
  //delayMicroseconds(delayValue*.01);
  digitalWrite(PIN_N1_N, LOW);
  interrupts();
}

// Turns on pair 2
void circuit_B() {
  noInterrupts();
  // These should be off
  digitalWrite(PIN_N1_N, HIGH);
  digitalWrite(PIN_P2, LOW);
  // These should be on
  digitalWrite(PIN_P1, HIGH);
  //delayMicroseconds(delayValue*.01);
  digitalWrite(PIN_N2_N, LOW);
  interrupts();
}



void loop() {
  noInterrupts();
  // Overvoltage check; default value is around 30 volts
  if (analogRead(PIN_CAP1) > 700 || analogRead(PIN_CAP2) > 700) {
    circuitHalt();
    circuitOn = false;
  }
  interrupts();
  
  // First: If micros has overflowed, make sure to reset lastLoop and halt the circuit
  // also if the circuit is stopped it halts constantly
  if (micros() < lastLoop || circuitOn == false) {
    noInterrupts();
    lastLoop = micros();
    circuitHalt();
    interrupts();
  }
  
  noInterrupts();
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
    interrupts();

    
    // Debug stuff, if you reenable the circuit will not work disconnected from MATLAB
    //char buf[20];
    //sprintf(buf, "switch to %d", currentCircuit);
    //Serial.println(buf);
    
  }

  while (Serial.available()) {
      commandBuffer += (char)Serial.read();
  }
  if (commandBuffer != "") {
        commandReady = true;
        commandIn = commandBuffer;
        commandBuffer = "";
  }
  
  if (commandReady) {
    if (handleCommand(commandIn) == false) {
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
  Serial.println(in);
  if (in == "STOP") {
    Serial.println("Halting circuit");
    circuitOn = false;
    circuitHalt();
    return true;
  }
  if (in == "START") {
    Serial.println("Starting circuit");
    circuitOn = true;
    return true;
  }
  if (in.substring(0, 5) == "FREQ " && in.length() >= 6) {
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

