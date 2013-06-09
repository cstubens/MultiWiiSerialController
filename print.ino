//
// An RC radio controller for MultiWii quadcopters.
// Carl Stubens, 2013
//
// v0.1
//

// Print the latest analog joystick data from nunchuks
void print_analog() {
  Serial.print(nunchuk0.analogX, DEC);
  Serial.print(' ');
  Serial.print(nunchuk0.analogY, DEC);
  Serial.print(' ');
  Serial.print(nunchuk1.analogX, DEC);
  Serial.print(' ');
  Serial.print(nunchuk1.analogY, DEC);
  Serial.print(' '); 
}

// Print the latest button data from nunchuks
void print_buttons() {
  Serial.print(nunchuk0.zButton, DEC);
  Serial.print(' ');
  Serial.print(nunchuk0.cButton, DEC);
  Serial.print(' ');
  Serial.print(nunchuk1.zButton, DEC);
  Serial.print(' ');
  Serial.print(nunchuk1.cButton, DEC);
  Serial.print(' ');  
}
