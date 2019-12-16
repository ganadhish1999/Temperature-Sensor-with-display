#include <DallasTemperature.h>
#include <OneWire.h>
#include <LiquidCrystal_I2C.h>


const int temp_data_pin = 4;
OneWire oneWire(temp_data_pin);
DallasTemperature sensor(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif

// Replace with your network credentials
const char* ssid = "Akshat";
const char* password = "password";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

float celsius, fahrenheit;

String returnCTemp() {
  sensor.requestTemperatures();
  celsius = sensor.getTempCByIndex(0);
  return String(celsius);
}

String returnFTemp() {
  sensor.requestTemperatures();
  fahrenheit = sensor.getTempFByIndex(0);
  return String(fahrenheit);
}


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    body {
      background-color:#424242;
    }
    html {
     font-family: Century Gothic MS, Verdana, sans-serif;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
     color:#fff;
    }
    .container {
      padding-top:10rem;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .ds-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
      color:#fff;
    }
    .label-name {
      padding-top:0.3rem;
    }
  </style>
</head>
<body>
  <div class="container">
  <h1>Temperature Readings</h1>
  <p>
    <i class="fas fa-thermometer-three-quarters" style="color:#f44336"></i>
    <span class="ds-labels label-name">Celsius</span> 
    <span id="temperaturec">%TEMPERATUREC%</span>
    <sup class="units">&deg;</sup>C 
  </p>
  <p>
    <i class="fas fa-thermometer-three-quarters" style="color:#f44336"></i>
    <span class="ds-labels label-name">Fahrenheit</span>
    <span id="temperaturef">%TEMPERATUREF%</span>
    <sup class="units">&deg;</sup>F
  </p>
  </div>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturef").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturef", true);
  xhttp.send();
}, 10000) ;
</script>
</html>)rawliteral";




// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATUREC"){
    return returnCTemp();
  }
  else if(var == "TEMPERATUREF"){
    return returnFTemp();
  }
  return String();
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  sensor.begin();
  
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("C:");
  lcd.setCursor(0,1);
  lcd.print("F:");

    // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", returnCTemp().c_str());
  });
  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", returnFTemp().c_str());
  });
  // Start server
  server.begin();

  
}

void loop() {
  // put your main code here, to run repeatedly:
   Serial.print("Celsius:");
   Serial.print(returnCTemp());
   Serial.print("\tFahrenheit:");
   Serial.println(returnFTemp());
   lcd.setCursor(5, 0);
   lcd.print(celsius);
   lcd.setCursor(5, 1);
   lcd.print(fahrenheit);
   delay(1000);
}
