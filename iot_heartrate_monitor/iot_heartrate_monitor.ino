#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>
#include <WiFi.h>
#include <ThingSpeak.h>


const char ssid[] = "C1F3R";
const char password[] = "314159265";
WiFiClient client;

const long CHANNEL = 1222839;
const char *WRITE_API = "MOF12ZGK61DKYH08";

long prevMillisThingSpeak = 0;
int intervalThingSpeak = 20000; // Minimum ThingSpeak write interval is 15 seconds


const int OUTPUT_TYPE = SERIAL_PLOTTER;
const int PULSE_INPUT = 32;   
const int PULSE_BLINK = 21;    // 
const int PULSE_FADE = 5;
const int THRESHOLD = 550;   // Adjust this number to avoid noise when idle

byte samplesUntilReport;
const byte SAMPLES_PER_SERIAL_SAMPLE = 10;


PulseSensorPlayground pulseSensor;

void setup() {

  Serial.begin(115200);
  
   // Configure the PulseSensor manager.
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);

 
  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);

  // Skip the first SAMPLES_PER_SERIAL_SAMPLE in the loop().
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;
  
  // Now that everything is ready, start reading the PulseSensor signal.
  if (!pulseSensor.begin()) {
  
    for(;;) {
      // Flash the led to show things didn't work.
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  } 

}

void loop() {

  
  if (pulseSensor.sawNewSample()) {
  
    if (--samplesUntilReport == (byte) 0) {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

      pulseSensor.outputSample();

      if (pulseSensor.sawStartOfBeat()) {
        pulseSensor.outputBeat();
        
      }
    }
    int myBPM = pulseSensor.getBeatsPerMinute();

    if (myBPM < 100 && myBPM > 50){
      if (millis() - prevMillisThingSpeak > intervalThingSpeak) {

      // Set the fields with the values
      ThingSpeak.setField(1, myBPM);
    
      // Write to the ThingSpeak channel
      int x = ThingSpeak.writeFields(CHANNEL, WRITE_API);
      if (x == 200) {
        Serial.println("Channel update successful.");
      }
      else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }

        prevMillisThingSpeak = millis();
      }  
      
    }

    /*******
      Here is a good place to add code that could take up
      to a millisecond or so to run.
    *******/
  }

  /******
     Don't add code here, because it could slow the sampling
     from the PulseSensor.
  ******/
}
