//
// An RC radio controller for MultiWii quadcopters.
// Carl Stubens, 2013
//
// v0.1
//

// Enable nunchuk 0 and disable nunchuk 1
void select0() {
  digitalWrite(PIN_TRANSISTOR_0, HIGH);
  digitalWrite(PIN_TRANSISTOR_1, LOW);
}

// Enable nunchuk 1 and disable nunchuk 0
void select1() {
  digitalWrite(PIN_TRANSISTOR_1, HIGH);
  digitalWrite(PIN_TRANSISTOR_0, LOW);
}

// Disable both nunchuks
void select_none() {
  digitalWrite(PIN_TRANSISTOR_0, LOW);
  digitalWrite(PIN_TRANSISTOR_1, LOW);
}

// Read data from both nunchuks
void update() {
  select0();
  nunchuk0.update();
  
  select1();
  nunchuk1.update();
  
  select_none();
}

