//
// An RC radio controller for MultiWii quadcopters.
// Carl Stubens, 2013
//
// v0.1
//

#include <Wire.h>
#include <ArduinoNunchuk.h> // https://github.com/GabrielBianconi/ArduinoNunchuk

// Configuration:
#define READ_NUNCHUKS       1
#define SEND_MSP            1
#define PRINT_ANALOG        0
#define PRINT_BUTTONS       0
#define PRINT_RC_SIGNALS    1
#define REQUIRE_Z_BUTTONS   0 

// Communications
#define BAUDRATE      9600
#define BAUDRATE_XBEE 9600
#define POLL_PERIOD 20 // 20ms --> 50Hz
#define PIN_PPM 4

// Pins connected to transistors, which connect and
// disconnect the two nunchucks from the I2C bus.
#define PIN_TRANSISTOR_0 2
#define PIN_TRANSISTOR_1 3

// The opcode (message type ID) and message length for 
// the MultiWii serial protocol's "SET_RAW_RC" message type.
#define MSP_SET_RAW_RC        200
#define MSP_SET_RAW_RC_LENGTH 16

// RC signal range to send to quad
#define RC_MIN 1000
#define RC_MID 1500
#define RC_MAX 2000

// The indices of the 8 signals in the following arrays
#define ROLL     0
#define PITCH    1
#define YAW      2
#define THROTTLE 3
#define AUX1     4
#define AUX2     5
#define AUX3     6
#define AUX4     7

// Calibration settings for each axis.
// Each axis has a minimum and maximum value.
int calib[4][2] = { 
                    {21,  220}, // roll 
                    {28,  222}, // pitch
                    {27,  222}, // yaw
                    {127, 227}  // throttle
                  };
                    
// The indices of the minimum and maximum values in 
// the above calibration arrays.
#define MIN 0
#define MAX 1

// RC signals to send to the quad
// format: { roll, throttle, yaw, pitch, 0, 0, 0, 0 }
uint16_t rc_signals[8] = { 1234 }; 

// Buffer for storing the serializes byte form of the RC signals
uint8_t rc_bytes[16] = { 0 };

ArduinoNunchuk nunchuk0 = ArduinoNunchuk();
ArduinoNunchuk nunchuk1 = ArduinoNunchuk();

void setup() {
  Serial.println();
  
  // Initialize serial connections
  Serial.begin(BAUDRATE);       // USB connection
  Serial3.begin(BAUDRATE_XBEE); // Xbee connection
  
  // Initialize digital pins which control transistors
  pinMode(PIN_TRANSISTOR_0, OUTPUT);
  pinMode(PIN_TRANSISTOR_1, OUTPUT);
  
  // Initialize the two nunchucks
  Serial.print("Initializing nunchucks... ");
  select0();
  nunchuk0.init();
  select1();
  nunchuk1.init();
  
  Serial.println("Done.");
  delay(500);
}

void loop() {
  // Read data from nunchucks and compute RC signals
  if(READ_NUNCHUKS) {
    update();
        
    get_rc_signals(rc_signals,
      nunchuk1.analogX, // roll
      nunchuk0.analogY, // thottle
      nunchuk0.analogX, // yaw
      nunchuk1.analogY  // pitch
      );
      
    if(nunchuk0.cButton == 1) {
      rc_signals[AUX1] = RC_MAX;
    } else {
      rc_signals[AUX1] = RC_MID;  
    }
    
    if(REQUIRE_Z_BUTTONS) {
      // Unless both z buttons are held down, always set the
      // throttle to RC_MIN for safety.
      if(nunchuk0.zButton != 1 || nunchuk1.zButton != 1) {
        rc_signals[THROTTLE] = RC_MIN;
      }
    }
  }
 
  // Send MultiWii Serial Protocol message
  if(SEND_MSP) {
    get_rc_bytes(rc_signals, rc_bytes);
    send_msp(MSP_SET_RAW_RC, rc_bytes, MSP_SET_RAW_RC_LENGTH); 
  }

  // Print debugging information, if configured to do so:

  if(PRINT_ANALOG) {
    print_analog();
  }

  if(PRINT_BUTTONS) {
    print_buttons();
  }

  if(PRINT_RC_SIGNALS) {
    for(int i = 0; i < 8; i++) {
      Serial.print(rc_signals[i], DEC);
      Serial.print(' ');
    }
  }

  if(PRINT_ANALOG | PRINT_BUTTONS | PRINT_RC_SIGNALS) {
    Serial.println();
    Serial.flush();
  }
  
  delay(POLL_PERIOD);
  
} // End main loop
