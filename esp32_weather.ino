
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "OLEDDisplayUi.h"
#include "images.h"
#include <WiFi.h>

const char* ssid     = "Your_SSID";
const char* password = "Your_Password_SSID";

const char* host = "api.openweathermap.org";
const char* privateKey = "Your Key";

String city;
String rise, set;
String temperature, temperature_min, temperature_max;
String humidity;
String pressure;
String wind_speed, wind_name, wind_code, wind_value;
String clouds;
String weather_value, weather_icon;
String lastupdate;

SSD1306  display(0x3c, 5, 4);
OLEDDisplayUi ui     ( &display );

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  //display->setTextAlignment(TEXT_ALIGN_RIGHT);
  //display->setFont(ArialMT_Plain_10);
  //display->drawString(128, 0, String(millis()));
}

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawXbm(32 + x, 0 + y, icon_width, icon_height, icon_bits);
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  if (weather_icon == "01d")
  {
    display->drawXbm(0 + x, 0 + y, Logo_01d_width, Logo_01d_height, Logo_01d_bits);
  }
  else if (weather_icon == "01n")
  {
    display->drawXbm(0 + x, 0 + y, Logo_01n_width, Logo_01n_height, Logo_01n_bits);
  }
  else if (weather_icon == "02d")
  {
    display->drawXbm(0 + x, 0 + y, Logo_02d_width, Logo_02d_height, Logo_02d_bits);
  }
  else if (weather_icon == "02n")
  {
    display->drawXbm(0 + x, 0 + y, Logo_02n_width, Logo_02n_height, Logo_02n_bits);
  }
  else if (weather_icon == "03d" || weather_icon == "03n")
  {
    display->drawXbm(0 + x, 0 + y, Logo_03dn_width, Logo_03dn_height, Logo_03dn_bits);
  }
  else if (weather_icon == "04d" || weather_icon == "04n")
  {
    display->drawXbm(0 + x, 0 + y, Logo_04dn_width, Logo_04dn_height, Logo_04dn_bits);
  }
  else if (weather_icon == "09d" || weather_icon == "09n")
  {
    display->drawXbm(0 + x, 0 + y, Logo_09dn_width, Logo_09dn_height, Logo_09dn_bits);
  }
  else if (weather_icon == "10d" || weather_icon == "10n")
  {
    display->drawXbm(0 + x, 0 + y, Logo_10dn_width, Logo_10dn_height, Logo_10dn_bits);
  }
  else if (weather_icon == "11d" || weather_icon == "11n")
  {
    display->drawXbm(0 + x, 0 + y, Logo_11dn_width, Logo_11dn_height, Logo_11dn_bits);
  }
  else if (weather_icon == "13d" || weather_icon == "13n")
  {
    display->drawXbm(0 + x, 0 + y, Logo_13dn_width, Logo_13dn_height, Logo_13dn_bits);
  }
  else if (weather_icon == "50d" || weather_icon == "50n")
  {
    display->drawXbm(0 + x, 0 + y, Logo_50dn_width, Logo_50dn_height, Logo_50dn_bits);
  }

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(60 + x, 10 + y, city);
  display->setFont(ArialMT_Plain_10);
  display->drawString(65 + x, 30 + y, clouds);
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(0 + x, 11 + y, "Humidity: " + humidity + " %");
  display->drawString(0 + x, 22 + y, "Pressure: " + pressure + " hPa");
  display->drawString(0 + x, 33 + y, "Wind speed: " + wind_speed + " m/s");
  //display->drawString(0 + x, 30 + y, String(set));


}

void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(0 + x, 11 + y, "Sunrise: " + rise);
  display->drawString(0 + x, 22 + y, "Sunset: " + set);
  display->drawString(0 + x, 33 + y, "update " + lastupdate);
}

void drawFrame5(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 11 + y, "Temperature " + temperature + " C");

  // The coordinates define the center of the text
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(54 + x, 22 + y, "Temperature min " + temperature_min + " C");

  // The coordinates define the right end of the text
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(118 + x, 33 + y, "Temperature max " + temperature_max + " C");
}

FrameCallback frames[] = { drawFrame1, drawFrame2, drawFrame3, drawFrame4, drawFrame5 };
int frameCount = 5;
OverlayCallback overlays[] = { msOverlay };
int overlaysCount = 1;

unsigned long previousMillis = 0;
const long interval = 10000;


void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();

  ui.setTargetFPS(60);
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);
  ui.setIndicatorPosition(BOTTOM);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setFrames(frames, frameCount);
  ui.setOverlays(overlays, overlaysCount);
  ui.init();
  display.flipScreenVertically();
  connectwifi();
}

void connectwifi()
{
  int i;
  display.init();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Connecting to ");
  Serial.print("Connecting to ");
  //display.display();
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.drawString(0 + i, 10, ".");
    i += 5;
    display.display();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println();

  display.drawString(0, 30, "WiFi connected");
  display.display();
  delay(2000);
}

void getdata()
{
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/data/2.5/weather?q=Bangkok&mode=xml&units=metric";
  url += "&appid=";
  url += privateKey;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
    cut_string(line, "city", "name");
    cut_string(line, "sun", "rise");
    cut_string(line, "sun", "set");
    cut_string(line, "temperature", "value");
    cut_string(line, "temperature", "min");
    cut_string(line, "temperature", "max");
    cut_string(line, "humidity", "humidity value");
    cut_string(line, "pressure", "pressure value");
    cut_string(line, "speed", "speed value");
    cut_string(line, "direction", "direction value");
    cut_string(line, "direction", "NE");
    cut_string(line, "direction", "name");
    cut_string(line, "clouds", "clouds value");
    cut_string(line, "weather", "value");
    cut_string(line, "weather", "icon");
    cut_string(line, "lastupdate", "lastupdate value");
  }
  Serial.println();
  Serial.println("closing");
  Serial.println();
}

void cut_string(String input, String header, String get_string)
{
  if (input.indexOf(header) != -1)    //ตรวจสอบว่าใน input มีข้อความเหมือนใน header หรือไม่
  {
    //int header = input.indexOf(header);

    int num_get = input.indexOf(get_string);  //หาตำแหน่งของข้อความ get_string ใน input
    if (num_get != -1)      //ตรวจสอบว่าตำแหน่งที่ได้ไม่ใช่ -1 (ไม่มีข้อความ get_string ใน input)
    {
      int start_val = input.indexOf("\"", num_get) + 1; // หาตำแหน่งแรกของ “
      int stop_val = input.indexOf("\"", start_val);  // หาตำแหน่งสุดท้ายของ “
      //return (input.substring(start_val, stop_val));  //ตัดเอาข้อความระหว่า “แรก และ ”สุดท้าย

      if (header == "city" && get_string == "name")
      {
        city = input.substring(start_val, stop_val);
      }
      else if (get_string == "rise")
      {
        rise = input.substring(start_val, stop_val);
        cuttime(rise, "rise");
      }
      else if (get_string == "set")
      {
        set = input.substring(start_val, stop_val);
        cuttime(set, "set");
      }
      else if (header == "temperature" && get_string == "value")
      {
        temperature = input.substring(start_val, stop_val);
      }
      else if (get_string == "min")
      {
        temperature_min = input.substring(start_val, stop_val);
      }
      else if (get_string == "max")
      {
        temperature_max = input.substring(start_val, stop_val);
      }
      else if ((header == "humidity") && (get_string == "humidity value"))
      {
        humidity = input.substring(start_val, stop_val);
      }
      else if (header == "pressure" && get_string == "pressure value")
      {
        pressure = input.substring(start_val, stop_val);
      }
      else if (header == "speed" && get_string == "speed value")
      {
        wind_speed = input.substring(start_val, stop_val);
      }
      else if (header == "direction" && get_string == "direction value")
      {
        wind_value = input.substring(start_val, stop_val);
      }
      else if (header == "direction" && get_string == "code")
      {
        wind_code = input.substring(start_val, stop_val);
      }
      else if (header == "direction" && get_string == "name")
      {
        wind_name = input.substring(start_val, stop_val);
      }
      else if (header == "clouds" && get_string == "clouds value")
      {
        int num_get = input.indexOf(get_string);
        int start_val = input.indexOf("name=", num_get) + 6;
        int stop_val = input.indexOf("\"", start_val + 5);
        clouds = input.substring(start_val, stop_val);
      }
      else if (header == "weather" && get_string == "value")
      {
        weather_value = input.substring(start_val, stop_val);
      }
      else if (header == "weather" && get_string == "icon")
      {
        weather_icon = input.substring(start_val, stop_val);
      }
      else if (get_string == "lastupdate value")
      {
        lastupdate = input.substring(start_val, stop_val);
        cuttime(lastupdate, "lastupdate");
      }

    }
  }
}

void cuttime(String input, String get_string)
{
  int num_get = input.indexOf(":");
  if (num_get != -1)
  {
    int start_val = input.indexOf(":") - 2; // หาตำแหน่งแรกของ “
    int stop_hr = input.indexOf(":");
    int stop_val = input.indexOf("\"");  // หาตำแหน่งสุดท้ายของ “
	
	int int_hr = input.substring(start_val, stop_hr).toInt() + 7;
	if(input.substring(start_val, stop_hr).toInt() + 7 > 24)
	{
		int_hr = int_hr -24;
	}
	
    if (get_string == "lastupdate")
    {
		
      lastupdate = int_hr + input.substring(stop_hr, stop_val);
    }
    else if (get_string == "rise")
    {
      rise = int_hr + input.substring(stop_hr, stop_val);
    }
    else if (get_string == "set")
    {
      set = int_hr + input.substring(stop_hr, stop_val);
    }
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    getdata();
  }
  int remainingTimeBudget = ui.update();
  if (remainingTimeBudget > 0) {
    delay(remainingTimeBudget);
  }
}
