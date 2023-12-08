// #include <Arduino.h>
// #include <Servo.h>
// #include <MD_TCS230.h>


// const int S0 = 4;
// const int S1 = 5;
// const int S2 = 6;
// const int S3 = 7;
// const int sensorOut = 8;
// const int buttonPin = 11;

// int WHITE;
// int BLACK;
// int IDEAL;

// int P_CONSTANT = 1;
// int D_CONSTANT = 1;
// const int MAX_SAMPLE_COUNT = 10;

// int lightMeter;
// int lightMeterMapped;
// Servo MOTOR_L;
// Servo MOTOR_R;


// int proportional(int);
// int derivative(int, int);

// int previous_light;
// int error;
// int buttonstate = 0;

// int totalError[MAX_SAMPLE_COUNT];
// int currentIndex = 0;

// void calibrate();
// void calculateTotalError(int);
// void setupMotors();

// void setup()
// {
// 	Serial.begin(9600); // Initialize serial port.
	
// 	setupMotors();

// 	// Attach color sensor
// 	pinMode(S0, OUTPUT);
// 	pinMode(S1, OUTPUT);
// 	pinMode(S2, OUTPUT);
// 	pinMode(S3, OUTPUT);
// 	pinMode(sensorOut, INPUT);
// 	// Set frequency 20%
// 	digitalWrite(S0, HIGH);
// 	digitalWrite(S1, LOW);
// 	// Set measuring to clear (white)
// 	digitalWrite(S2, HIGH);
// 	digitalWrite(S3, LOW);

// 	// Attach button
// 	pinMode(buttonPin, INPUT_PULLUP);

// 	previous_light = IDEAL;
// 	calibrate();
// 	delay(5000);
// }

// void setupMotors() {
//     MOTOR_L.attach(10);
//     MOTOR_L.write(90);
//     MOTOR_R.attach(9);
//     MOTOR_R.write(90);
// }

// void loop()
// {
// 	// Calculate error value
// 	int light = pulseIn(sensorOut, LOW);
// 	error = (proportional(light) * P_CONSTANT + derivative(light, previous_light) * D_CONSTANT) / 15;

// 	// Limit the error range
// 	int max_error = 80;
// 	int min_error = -80;

// 	if (error > max_error)
// 	{
// 		error = max_error;
// 	}
// 	else if (error < min_error)
// 	{
// 		error = min_error;
// 	}

// 	calculateTotalError(error);

// 	MOTOR_L.write(100 + error);
// 	MOTOR_R.write(80 + error);

// 	previous_light = light;
// 	delay(100);
// }


// int proportional(int light)
// {
// 	return light - IDEAL;
// }
// int derivative(int light, int previous_light)
// {
// 	return light - previous_light;
// }

// void calibrate()
// {
// 	Serial.println("Calibrating...");

// 	// Calibrate the white value
// 	while (digitalRead(buttonPin) == HIGH)
// 	{
// 		// Wait for the sensor to detect the white surface
// 	}
// 	WHITE = pulseIn(sensorOut, LOW);
// 	Serial.println("WHITE calibrated.");
// 	Serial.print(WHITE);

// 	delay(200);

// 	// Calibrate the black value
// 	while (digitalRead(buttonPin) == HIGH)
// 	{
// 		// Wait for the sensor to detect the black surface
// 	}
// 	BLACK = pulseIn(sensorOut, LOW);
// 	Serial.println("BLACK calibrated.");
// 	Serial.print(BLACK);

// 	// Calculate the IDEAL value
// 	IDEAL = (BLACK + WHITE) / 2;
// 	Serial.println("Calibration complete.");
// 	Serial.print(IDEAL);
// }

// void calculateTotalError(int error)
// {
// 	// Check if the array is not full
// 	if (currentIndex < MAX_SAMPLE_COUNT)
// 	{
// 		totalError[currentIndex] = abs(error);
// 		currentIndex++;
// 	}
// 	// Calculate total error and print
// 	else
// 	{
// 		// Initialize sum to zero
// 		int sum = 0;
// 		for (int i = 0; i < MAX_SAMPLE_COUNT; i++)
// 		{
// 			sum += totalError[i];
// 			totalError[i] = 0; // Reset elements to zero
// 		}
// 		// Print error
// 		Serial.print("Average error over samples: ");
// 		Serial.println(sum / MAX_SAMPLE_COUNT);
// 		currentIndex = 0; // Reset the index
// 	}
// }
