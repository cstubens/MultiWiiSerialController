//
// An RC radio controller for MultiWii quadcopters.
// Carl Stubens, 2013
//
// v0.1
//

// This file contains functions which deal with translating joystick 
// data into RC control signals wich are equivalent to the PWM RC 
// values expected by MultiWii.

// Apply a calibration tuple (min, max) to some input 
// value, and map the output onto (RC_MIN, RC_MAX).
int apply_calib(int x, int * calib) {
  x = map(x, calib[MIN], calib[MAX], RC_MIN, RC_MAX);
  x = constrain(x, RC_MIN, RC_MAX);
  return x;
}

// Set the RC signal array to the default values.
void zero_rc_signals(uint16_t * rc) {
  for(int i = 0; i < 8; i++) {
    rc[i] = RC_MID;
  }
  rc[THROTTLE] = RC_MIN;
}

// Compute RC signals from the latest nunchuk data.
void get_rc_signals(uint16_t * rc, int roll, int throttle, int yaw, int pitch) {
  zero_rc_signals(rc);
  rc[ROLL]     = apply_calib(roll,     calib[ROLL]);
  rc[THROTTLE] = apply_calib(throttle, calib[THROTTLE]);
  rc[YAW]      = apply_calib(yaw,      calib[YAW]);
  rc[PITCH]    = apply_calib(pitch,    calib[PITCH]);
}

// Set the RC bytes buffer to all zeros.
void zero_rc_bytes(uint8_t * buff) {
  for(int i = 0; i < 16; i++) {
    buff[i] = 0;  
  }  
}

// Compute the RC message data bytes based on the given RC signals.
void get_rc_bytes(uint16_t * rc, uint8_t * buff) {
  int j = 0;
  for(int i = 0; i < 8; i++) {
    buff[j++] = rc[i] & 0xFF;        // LSB first
    buff[j++] = (rc[i] >> 8) & 0xFF; // MSB second
  }  
}
