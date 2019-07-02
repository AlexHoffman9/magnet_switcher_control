#define STARTUP_DELAY 2000
#define COMMAND_LENGTH 12
#define PIN_P2_A 11
#define PIN_P1_A 9
#define PIN_N1_A 10
#define PIN_N2_A 12
#define PIN_P2_B 7
#define PIN_P1_B 2
#define PIN_N1_B 3
#define PIN_N2_B 8
#define PIN_P2_C 24
#define PIN_P1_C 28
#define PIN_N1_C 26
#define PIN_N2_C 22

#define PMOS_ON HIGH // pulls H bridge gate low to turn on PMOS
#define PMOS_OFF LOW
#define NMOS_ON LOW // pulls H bridge gate high to turn on NMOS
#define NMOS_OFF HIGH

// shuts off current to coil
void circuitHalt(char coil) {
    if (coil=='A') {
      noInterrupts();
      digitalWrite(PIN_P1_A, PMOS_OFF);
      digitalWrite(PIN_P2_A, PMOS_OFF);
      digitalWrite(PIN_N2_A, NMOS_OFF);
      digitalWrite(PIN_N1_A, NMOS_OFF);
      interrupts();
    } else if (coil=='B') {
      noInterrupts();
      digitalWrite(PIN_P1_B, PMOS_OFF);
      digitalWrite(PIN_P2_B, PMOS_OFF);
      digitalWrite(PIN_N2_B, NMOS_OFF);
      digitalWrite(PIN_N1_B, NMOS_OFF);
      interrupts();
    } else {
      noInterrupts();
      digitalWrite(PIN_P1_C, PMOS_OFF);
      digitalWrite(PIN_P2_C, PMOS_OFF);
      digitalWrite(PIN_N2_C, NMOS_OFF);
      digitalWrite(PIN_N1_C, NMOS_OFF);
      interrupts();
  }
}

// Turns on pair 1 of h bridge
void circuitActivateForward(char coil) {
    if (coil=='A') {
      noInterrupts();
      // These should be off
      digitalWrite(PIN_N2_A, NMOS_OFF);
      digitalWrite(PIN_P1_A, PMOS_OFF);
      // These should be on
      digitalWrite(PIN_P2_A, PMOS_ON);
      digitalWrite(PIN_N1_A, NMOS_ON);
      interrupts();
    } else if (coil=='B') {
      noInterrupts();
      digitalWrite(PIN_N2_B, NMOS_OFF);
      digitalWrite(PIN_P1_B, PMOS_OFF);
      digitalWrite(PIN_P2_B, PMOS_ON);
      digitalWrite(PIN_N1_B, NMOS_ON);
      interrupts();
    } else {
      noInterrupts();
      digitalWrite(PIN_N2_C, NMOS_OFF);
      digitalWrite(PIN_P1_C, PMOS_OFF);
      digitalWrite(PIN_P2_C, PMOS_ON);
      digitalWrite(PIN_N1_C, NMOS_ON);
      interrupts();
  }
}

// Turns on pair 2 of h bridge
void circuitActivateReverse(char coil) {
    if (coil=='A') {
      noInterrupts();
      // These should be off
      digitalWrite(PIN_N1_A, NMOS_OFF);
      digitalWrite(PIN_P2_A, PMOS_OFF);
      // These should be on
      digitalWrite(PIN_P1_A, PMOS_ON);
      digitalWrite(PIN_N2_A, NMOS_ON);
      interrupts();
    } else if (coil=='B') {
    //case 'B': // THIS CODE WAS GETTING EXECUTING WHEN COIL == 'A' USING SWITCH STATEMENT
      noInterrupts();
      digitalWrite(PIN_N1_B, NMOS_OFF);
      digitalWrite(PIN_P2_B, PMOS_OFF);
      digitalWrite(PIN_P1_B, PMOS_ON);
      digitalWrite(PIN_N2_B, NMOS_ON);
      interrupts();
    } else {
      noInterrupts();
      digitalWrite(PIN_N1_C, NMOS_OFF);
      digitalWrite(PIN_P2_C, PMOS_OFF);
      digitalWrite(PIN_P1_C, PMOS_ON);
      digitalWrite(PIN_N2_C, NMOS_ON);
      interrupts();
    }
}
