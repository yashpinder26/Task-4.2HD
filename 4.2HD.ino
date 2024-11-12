#include <WiFiNINA.h> // Include library for WiFi functionality on Arduino boards with WiFiNINA module
#include <Firebase_Arduino_WiFiNINA.h> // Include Firebase library to interact with Firebase Realtime Database over WiFi

// Define GPIO pins for the red, green, and blue LEDs
const int redPin = 5;
const int greenPin = 6;
const int bluePin = 7;

// Create a Firebase data object to manage data transmission and response
FirebaseData firebaseData;

// Firebase connection details
String firebaseHost = "task42hd-default-rtdb.firebaseio.com"; // Firebase Realtime Database URL
String firebaseAuth = "3Olo1n9JRJ022oR2uiE7Ask3wGr6ChNW6azUVxvB"; // Firebase database secret token for authentication

// WiFi credentials
char ssid[] = "A16"; // WiFi network name (SSID)
char pass[] = "22222222"; // WiFi network password

// Setup function to initialize WiFi, Firebase, and LED pins
void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate for debugging

  // Attempt to connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) { // Wait until the WiFi connection is established
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase with host URL, auth token, and WiFi credentials
  Firebase.begin(firebaseHost, firebaseAuth, ssid, pass);
  Firebase.reconnectWiFi(true); // Enable automatic reconnection to WiFi if disconnected

  // Set LED pins as output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Ensure all LEDs are initially turned off
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

// Main loop to fetch data from Firebase and control LEDs based on the command
void loop() {
  // Fetch the "trafficLight" string value from Firebase
  if (Firebase.getString(firebaseData, "/trafficLight")) {
    // If the data is successfully received, store it in a string variable
    String command = firebaseData.stringData();
    Serial.println("Command received from Firebase: " + command);

    // Call function to control LEDs based on the received command
    controlTrafficLight(command);
  } else {
    // If there's an error, print the error reason
    Serial.println("Failed to get data from Firebase: " + firebaseData.errorReason());
  }
  delay(1000); // Wait 1 second before checking Firebase again
}

// Function to control traffic light LEDs based on the command received
void controlTrafficLight(String command) {
  // Convert the command to lowercase for easy comparison
  command.toLowerCase();

  // Check the first character of the command to determine LED action
  char firstChar = command.charAt(0);

  // Use a switch-case statement to determine which LED to turn on based on the first character
  switch (firstChar) {
    case 'r':  // 'r' command for Red light
      digitalWrite(redPin, HIGH); // Turn on red LED
      digitalWrite(greenPin, LOW); // Turn off green LED
      digitalWrite(bluePin, LOW); // Turn off blue LED
      Serial.println("Red light ON");
      break;

    case 'g':  // 'g' command for Green light
      digitalWrite(redPin, LOW); // Turn off red LED
      digitalWrite(greenPin, HIGH); // Turn on green LED
      digitalWrite(bluePin, LOW); // Turn off blue LED
      Serial.println("Green light ON");
      break;

    case 'b':  // 'b' command for Blue light
      digitalWrite(redPin, LOW); // Turn off red LED
      digitalWrite(greenPin, LOW); // Turn off green LED
      digitalWrite(bluePin, HIGH); // Turn on blue LED
      Serial.println("Blue light ON");
      break;

    default:  // Default case for unrecognized commands (all lights off)
      digitalWrite(redPin, LOW); // Turn off red LED
      digitalWrite(greenPin, LOW); // Turn off green LED
      digitalWrite(bluePin, LOW); // Turn off blue LED
      Serial.println("All lights OFF");
      break;
  }
}
