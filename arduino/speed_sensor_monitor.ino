#include <SPI.h>
#include "mcp2515_can.h" //CAN_BUS_Shield, by seed studio

// Pin where the speed sensor is connected
const int sensorPin = 3;

// Variables for pulse counting and timing
volatile unsigned long pulseCount = 0;  // Pulse counter
unsigned long previousMillis = 0;       // Stores the last update time
const long interval = 250;             // Interval to calculate speed (in ms)

// Wheel and sensor parameters
const float wheelDiameter = 0.067;  // Diameter of the wheel (in meters)
const int pulsesPerRevolution = 20; // Pulses per wheel revolution

// CAN Bus setup
const int SPI_CS_PIN = 9;           // Chip select pin for CAN module
mcp2515_can CAN(SPI_CS_PIN);        // Create an instance of the MCP_CAN class

// Interrupt Service Routine to count pulses
void countPulse() {
  pulseCount++;
}

void setup() {
  // Initialize Serial communication for debugging
  Serial.begin(9600);

  // Initialize the CAN module
  if (CAN.begin(CAN_500KBPS) == CAN_OK) {
    Serial.println("CAN initialized successfully!");
  } else {
    Serial.println("CAN initialization failed!");
    while (1);  // Stop if initialization fails
  }

  // Set CAN module to normal mode
  CAN.setMode(MODE_NORMAL);
  Serial.println("CAN in NORMAL mode");

  // Set up the speed sensor
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, FALLING);  // Interrupt on falling edge
}

void loop() {
  unsigned long currentMillis = millis();

  // Update speed at the defined interval
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Copy the number of pulses counted in the last interval
    unsigned long pulsesThisSecond = pulseCount;
    pulseCount = 0;  // Reset the pulse counter for the next interval

    // Calculate the distance per pulse (in meters)
    float distancePerPulse = (PI * wheelDiameter) / pulsesPerRevolution;

    // Calculate the speed in meters per second (m/s)
    //float speed_mps = pulsesThisSecond * distancePerPulse;
    float speed_mps = (pulsesThisSecond * distancePerPulse) * (1000.0 / interval);

    // Print the speed to the Serial Monitor
    Serial.print("\nSpeed (m/s): ");
    Serial.println(speed_mps, 2);  // Print with 2 decimal places

    // Prepare the CAN message with speed in m/s (scaled by 100 for precision)
    int speed_scaled = (int)(speed_mps * 100);  // Multiply by 100 to avoid decimals
    byte canMessage[2];  // Speed encoded in 2 bytes
    canMessage[0] = (speed_scaled >> 8) & 0xFF;  // High byte
    canMessage[1] = speed_scaled & 0xFF;         // Low byte

    // Send the CAN message with ID 0x100
    if (CAN.sendMsgBuf(0x100, 0, 2, canMessage) == CAN_OK) {
      Serial.println("CAN MESSAGE SENT!");
    } else {
      Serial.println("CAN MESSAGE ERROR!");
    }
  }
}