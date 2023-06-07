#include <SoftwareSerial.h>

SoftwareSerial gsmSerial(10, 11); // RX, TX pins for SIM800L GSM module

void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);
  delay(1000);

  // Connect to the cellular network
  gsmSerial.println("AT+CPIN=\"1234\""); // Replace "1234" with your SIM card PIN
  delay(1000);
  gsmSerial.println("AT+CREG=1"); // Enable network registration
  delay(1000);
  gsmSerial.println("AT+CGATT=1"); // Enable GPRS
  delay(1000);

  // Configure the HTTP parameters
  gsmSerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\""); // Set the context type
  delay(1000);
  gsmSerial.println("AT+SAPBR=3,1,\"APN\",\"your_apn\""); // Replace "your_apn" with your APN
  delay(1000);
  gsmSerial.println("AT+SAPBR=1,1"); // Open the GPRS context
  delay(3000);
  gsmSerial.println("AT+HTTPINIT"); // Initialize the HTTP service
  delay(1000);
  gsmSerial.println("AT+HTTPPARA=\"CID\",1"); // Set the context ID
  delay(1000);
}

void loop() {
  // Check if a 10 rupees UPI payment is made
  gsmSerial.println("AT+HTTPACTION=0"); // Send the HTTP GET request
  delay(3000);
  while (gsmSerial.available()) {
    String response = gsmSerial.readString();
    if (response.indexOf("+HTTPACTION: 0,200") >= 0) { // HTTP response code 200 indicates success
      if (response.indexOf("SUCCESS") >= 0) { // Check if the UPI payment is successful
        if (response.indexOf("AMOUNT=10") >= 0) { // Check if the payment amount is 10 rupees
          Serial.println("10 rupees UPI payment successful");
          // Do something here (e.g., play a sound, turn on a light, etc.)
        }
        else {
          Serial.println("UPI payment successful but not 10 rupees");
        }
      }
      else {
        Serial.println("UPI payment unsuccessful");
      }
    }
  }
  delay(5000); // Wait for 5 seconds before checking again
}
