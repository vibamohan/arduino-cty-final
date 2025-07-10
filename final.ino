// Include necessary libraries
#include <IRremote.h>  // For IR remote control functionality
#include <SR04.h>      // For ultrasonic distance sensor

// Pin definitions
const int IR_RECEIVER_PIN = 7;        // IR receiver connected to digital pin 7
const int ULTRASONIC_ECHO_PIN = 12;   // Ultrasonic sensor ECHO pin
const int ULTRASONIC_TRIG_PIN = 11;   // Ultrasonic sensor TRIG pin
const int LED_ALERT_PIN = 6;          // LED for alert indication
const int LED_STATUS_PIN = 5;         // LED for system status
const int LED_REMINDER_PIN = 4;       // LED to remind owner to activate system
const int BUZZER_PIN = 8;             // Buzzer for audible alert

// Constants
const long DISTANCE_THRESHOLD_CM = 5;     // Distance to trigger alert (in cm)
const int BUZZER_FREQUENCY = 440;         // Frequency of buzzer sound (in Hz)
const unsigned long ALERT_DURATION_MS = 2000;  // Duration of alert (in milliseconds)

// IR remote button codes
const long BUTTON_DEACTIVATE = 0xFF30CF;  // Code for button 1 (deactivate)
const long BUTTON_ACTIVATE = 0xFF18E7;    // Code for button 2 (activate)

// Create objects for IR receiver and ultrasonic sensor
IRrecv irReceiver(IR_RECEIVER_PIN);
decode_results irResults;
SR04 ultrasonicSensor = SR04(ULTRASONIC_ECHO_PIN, ULTRASONIC_TRIG_PIN);

// State variables
bool systemActive = true;  // Tracks whether the security system is active

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Start the IR receiver
  irReceiver.enableIRIn();
  
  // Set up pin modes
  pinMode(LED_ALERT_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
  pinMode(LED_REMINDER_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Set initial LED states
  updateLEDs();
  
  Serial.println("System initialized");
}

void loop() {
  // Main program loop
  handleRemoteControl();  // Check for remote control input
  handleProximitySensor();  // Check proximity sensor and handle alerts
}

void handleRemoteControl() {
  // Check if an IR signal is received
  if (irReceiver.decode(&irResults)) {
    switch(irResults.value) {
      case BUTTON_DEACTIVATE:
        systemActive = false;
        Serial.println("System deactivated");
        break;
      case BUTTON_ACTIVATE:
        systemActive = true;
        Serial.println("System activated");
        break;
      default:
        Serial.print("Unknown button pressed: ");
        Serial.println(irResults.value, HEX);
    }
    updateLEDs();  // Update LED states after system state change
    irReceiver.resume();  // Resume listening for IR signals
  }
}

void handleProximitySensor() {
  // Measure distance using ultrasonic sensor
  long distance = ultrasonicSensor.Distance();
  
  // If object is close and system is active, trigger alert
  if (distance <= DISTANCE_THRESHOLD_CM && systemActive) {
    triggerAlert();
  } else {
    stopAlert();
  }
}

void triggerAlert() {
  // Activate visual and audible alerts
  digitalWrite(LED_ALERT_PIN, HIGH);
  digitalWrite(LED_STATUS_PIN, HIGH);
  tone(BUZZER_PIN, BUZZER_FREQUENCY, ALERT_DURATION_MS);
}

void stopAlert() {
  // Deactivate visual and audible alerts
  digitalWrite(LED_ALERT_PIN, LOW);
  digitalWrite(LED_STATUS_PIN, LOW);
  noTone(BUZZER_PIN);
}

void updateLEDs() {
  // Update reminder LED based on system state
  digitalWrite(LED_REMINDER_PIN, systemActive ? HIGH : LOW);
}