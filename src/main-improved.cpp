/**
 * Author: Abel van Dijk
 * School: Fontys-ICT
 * Purpose: Line follower robot
*/

#include <Arduino.h>
#include <Servo.h>
#include <MD_TCS230.h>
#include <EEPROM.h>

// Constants should not exceed 0.5, works with both 0.1 
const double P_CONSTANT = 0.1; 
const double D_CONSTANT = 0.1;

// Used for calculating total error over a sample
const int MAX_SAMPLE_COUNT = 10;

// Standard speed if none is found in EEPROM (min 0, max 90, works best between 10-30)
int SPEED = 10;

// Declare pins
const int S0 = 4;
const int S1 = 5;
const int S2 = 6;
const int S3 = 7;
const int sensorOut = 8;
const int buttonPin = 11;

// Declare constants for line color, values are assigned while calibrating
int WHITE;
int BLACK;
int IDEAL;

// Declare servos
Servo MOTOR_L;
Servo MOTOR_R;

// Declare variables
int previousError;
int error;
int totalError[MAX_SAMPLE_COUNT];
int currentIndex = 0; // leave at 0
int minErrorValue;
int maxErrorValue;

// Declare functions
void calibrate();
void calculateTotalError(int);
void setupMotors();
void setEEPROMspeed();

// Setup before loop
void setup()
{
	// Setup serial monitor
	Serial.begin(9600); 
	
	// Attach motors and set speed to 0
	setupMotors();

	// Attach color sensor
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);
	pinMode(sensorOut, INPUT);

	// Set frequency 20%
	digitalWrite(S0, HIGH);
	digitalWrite(S1, LOW);

	// Set measuring to clear (white)
	digitalWrite(S2, HIGH);
	digitalWrite(S3, LOW);

	// Attach button
	pinMode(buttonPin, INPUT_PULLUP);

	// Log speed value in EEPROM if a value is send to serial monitor within 5 seconds
	setEEPROMspeed();

	// Calibrate light values
	calibrate();

	// Set some constants
	previousError = 0;
	minErrorValue = WHITE-IDEAL;
  	maxErrorValue = BLACK-IDEAL;

	// Delay to give user time to place the robot
	delay(3000);
}

void setEEPROMspeed(){
	if (Serial)
	{
		Serial.println("Enter robot speed (10-40), you have 5 seconds: ");
		// Wait for serial input and trim
		delay(5000); 
		String input = Serial.readStringUntil('\n');
		input.trim();
		// Writes input speed to EEPROM if valid input
		if (input != "" && input.toInt() > 0 && input.toInt() < 50){
			SPEED = input.toInt();
			EEPROM.write(0, SPEED);
		}
		Serial.println("speed = :" + input); 
		// Sets speed to EEPROM speed if no speed value is specified, and EEPROM value is valid
		if (EEPROM.read(0) == (int)EEPROM.read(0) && EEPROM.read(0) > 0 && EEPROM.read(0) < 5){
			SPEED = EEPROM.read(0);	
		}
		// Else, default speed value is used
	}
}

void setupMotors() {
	// Attaches motors to respective pins, and sets speed to 0
    MOTOR_L.attach(10);
    MOTOR_L.write(90);
    MOTOR_R.attach(9);
    MOTOR_R.write(90);
}

void loop()
{
	// Calculate error values
	int light = pulseIn(sensorOut, LOW);
	error = light-IDEAL;

  	// Limit the error range2
  	error = map(error, minErrorValue, maxErrorValue, -90, 90); // map the error to a range between -90 and 90 to not exceed the maximum motor speed

	calculateTotalError(error);
	double P = error;
	double D = error - previousError;

	int correction = ((P*P_CONSTANT)+(D*D_CONSTANT)); // calculate the change needed in motor

	MOTOR_L.write(90+SPEED + correction);
	MOTOR_R.write(90-SPEED + correction);

	previousError = error;
	delay(100);

}

void calibrate()
{
	Serial.println("Calibrating...");

	// Calibrate the white value
	while (digitalRead(buttonPin) == HIGH)
	{
		// Wait for the sensor to detect the white surface
	}
	WHITE = pulseIn(sensorOut, LOW);
	Serial.println("WHITE calibrated.");
	Serial.print(WHITE);

	delay(200);

	// Calibrate the black value
	while (digitalRead(buttonPin) == HIGH)
	{
		// Wait for the sensor to detect the black surface
	}
	BLACK = pulseIn(sensorOut, LOW);
	Serial.println("BLACK calibrated.");
	Serial.print(BLACK);

	// Calculate the IDEAL value
	IDEAL = (BLACK + WHITE) / 2;
	Serial.println("Calibration complete.");
	Serial.print(IDEAL);
	
}

void calculateTotalError(int error)
{
	// Check if the array is not full
	if (currentIndex < MAX_SAMPLE_COUNT)
	{
		totalError[currentIndex] = error;
		currentIndex++;
	}
	// Calculate total error and print
	else
	{
		// Initialize sum to zero
		int sum = 0;
		for (int i = 0; i < MAX_SAMPLE_COUNT; i++)
		{
			sum += totalError[i];
			totalError[i] = 0; // Reset elements to zero
		}
		// Print error
		Serial.print("Average error over samples: ");
		Serial.println(sum / MAX_SAMPLE_COUNT);
		currentIndex = 0; // Reset the index
	}
}
