#include <WiFiNINA.h> 
#include <Firebase_Arduino_WiFiNINA.h>  

const int redPin = 5;
const int greenPin = 6;
const int bluePin = 7;

FirebaseData firebaseData;
String firebaseHost = "task42hd-default-rtdb.firebaseio.com"; 
String firebaseAuth = "3Olo1n9JRJ022oR2uiE7Ask3wGr6ChNW6azUVxvB";  


char ssid[] = "A16";
char pass[] = "22222222";  

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Firebase.begin(firebaseHost, firebaseAuth, ssid, pass);
  Firebase.reconnectWiFi(true);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

void loop() {
  if (Firebase.getString(firebaseData, "/trafficLight")) 
  {
    String command = firebaseData.stringData();
    Serial.println("Command received from Firebase: " + command);
    controlTrafficLight(command);  
  } else {
    Serial.println("Failed to get data from Firebase: " + firebaseData.errorReason());
  }
  delay(1000);  
}

void controlTrafficLight(String command) {
  
  command.toLowerCase();  
  char firstChar = command.charAt(0);  

  switch (firstChar) {
    case 'r':  
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      Serial.println("Red light ON");
      break;

    case 'g':  
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
      Serial.println("Green light ON");
      break;

    case 'b':  
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH);
      Serial.println("Blue light ON");
      break;

    default:  
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      Serial.println("All lights OFF");
      break;
  }
}

