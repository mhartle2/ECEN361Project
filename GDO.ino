#include <ESP8266WiFi.h>
#include <Servo.h>
 
//const char* ssid = "<3ley";
//const char* password = "LoveatHome4";
const char* ssid = "Funkybunch";
const char* password = "marc1234";

 
int ledPin = 2; // GPIO2
WiFiServer server(80);

Servo myServo;
 
void setup() {
  myServo.attach(5);
  Serial.begin(115200);
  delay(10);
 
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
   
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
    
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
   
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/GARAGE=CLOSED") != -1) {
    digitalWrite(ledPin, LOW);
    value = LOW;
    myServo.write(150);
    delay(500);
    myServo.write(92);
  } 
  if (request.indexOf("/GARAGE=OPEN") != -1){
    digitalWrite(ledPin, HIGH);
    value = HIGH;
    myServo.write(30);
    delay(500);
    myServo.write(90);
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
   
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html style=\"font-size: 200%;\">");
  //client.println("<div style=\"font-size: 200%;\">"
   
  client.print("The Garage Door is now: ");
   
  if(value == HIGH) {
    client.print("Open");  
  } else {
    client.print("Closed");
  }
  client.println("<br><br><br><br><br>");
  client.println("Click <a href=\"/GARAGE=OPEN\">here</a> to open the garage door<br>");
  client.println("<br><br>");
  client.println("Click <a href=\"/GARAGE=CLOSED\">here</a> to close the garage door<br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
