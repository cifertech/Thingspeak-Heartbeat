# Thingspeak-Heartbeat
In the heart rate monitoring project, we set up the heart rate sensor using a nodemcu board that uses the ESP32 chip. We then transfer the information received from the sensor to the IoT platform.

## What it DO!
In the heart rate monitoring project, we used an IoT platform called ThingSpeak, in fact, this platform allows us to build a dashboard with the ability to monitor input values in different ways. And in this project, using this feature, we will measure the heart rate of the person in question with the relevant sensor and display it as a graph with this dashboard. The sensor used in this project is skin type. Just place the sensor on the surface of the skin of the hand.

### Items needed
-ESP32
-Heart beat sensor

### Required libraries
-PulseSensorPlayground
-WiFi

## Conclusion
In the heart rate monitoring project, we obtained heart rate values using a pulse sensor or heart rate sensor and analyzed these values using the ESP32 board and displayed them in the monitor series. Next, by calling the ThingSpeak library, we will send the obtained values to ThinkSpeak servers, and by introducing the built-in dashboard to the program code, the values will be displayed in our dashboard.

 Full Tutorial at http://cifertech.net/
