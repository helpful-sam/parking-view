/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

//---------------------------------------------------------------
// VENJUN 240514
// zip lib download and install 
//  - ESPAsyncWebServer-master
// LIRARY MANAGER Lib. install
//  - AsyncTCP by dvarrel 1.1.4
//---------------------------------------------------------------

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
//#include <Adafruit_Sensor.h>
//#include <DHT.h>

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "password";

#define DHTPIN 27     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

//DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  // float t = dht.readTemperature();

  float t = 100;
  
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).

  // 코드 중에서 처음 보는 isnan() 함수는 입력 인자 값이 숫자이면 false를 숫자가 아니면 true를 반환합니다.

  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // float h = dht.readHumidity();

  float h = 200;

  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}


const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>최소규모 주차정보시스템(PIS)</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            overflow: hidden;
        }

        .navbar {
            display: flex;
            justify-content: center;
            align-items: center;
            background-color: #ffffff;
            padding: 10px 20px;
            border-bottom: 1px solid #ddd;
        }

        .legend {
            display: flex;
            justify-content: center;
            margin: 10px 0;
            font-size: 18px;
        }

        .legend div {
            margin: 0 15px;
        }

        .legend .green, .legend .red {
            display: inline-block;
            width: 15px;
            height: 15px;
            margin-right: 5px;
        }

        .legend .green {
            background-color: green;
        }

        .legend .red {
            background-color: red;
        }

        .parking-area {
            margin: 0 auto;
            padding-top: 1%;
            text-align: center;
        }

        .parking-slot {
            display: inline-block;
            margin-right: 20px; /* Space between items */
            margin-bottom: 20px; /* Space between rows */
            width: 300px;
            height: 450px;
        }

        .parking-slot:last-child {
            margin-right: 0; /* Remove margin from the last item */
        }

        .parking-slot.occupied {
            background-color: red;
        }

        .parking-slot.available {
            background-color: green;
        }
    </style>
</head>
<body>
    <div class="navbar">
        <h1>최소규모 주차정보시스템(PIS)</h1>
    </div>
    <div class="legend">
        <div id="available-count"><span class="green"></span> Available: 0</div>
        <div id="occupied-count"><span class="red"></span> Occupied: 0</div>
    </div>
    <div class="parking-area" id="parking-area">
        <div class="parking-slot" id="slot-1"></div><div class="parking-slot" id="slot-2"></div>
    </div>

    <script>
        const parkingArea = document.querySelector('#parking-area');
        const availableCount = document.querySelector('#available-count');
        const occupiedCount = document.querySelector('#occupied-count');

        let parkingData = [0, 0]; // 0 for available (green), 1 for occupied (red)

        // Simulate data stream
        setInterval(() => {
            parkingData = parkingData.map(() => Math.round(Math.random())); // Randomly set 0 or 1
            updateParkingSlots();
        }, 2000);

        function updateParkingSlots() {
            let available = 0;
            let occupied = 0;

            parkingData.forEach((status, index) => {
                const slot = document.querySelector(`#slot-${index + 1}`);
                if (status === 0) {
                    slot.classList.remove('occupied');
                    slot.classList.add('available');
                    available++;
                } else {
                    slot.classList.remove('available');
                    slot.classList.add('occupied');
                    occupied++;
                }
            });

            availableCount.innerHTML = `<span class="green"></span> Available: ${available}`;
            occupiedCount.innerHTML = `<span class="red"></span> Occupied: ${occupied}`;
        }

        // Initial render
        updateParkingSlots();
    </script>
</body>
</html>

)rawliteral";


// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

//  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}