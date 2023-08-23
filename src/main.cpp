#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include <stdio.h>  
#include <Adafruit_TCS34725.h>
#include <Wire.h>
#include <SparkFun_SCD30_Arduino_Library.h> 

// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_CS    5
#define TFT_RST   4 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC    2
#define TFT_BAR_HEIGHT 4 // Height for the loading bar

#define TFT_BAR_Y 156 // Y position for the loading bar
#define TFT_BAR_X 0 // X position for the loading bar
#define TFT_WIDTH 128 // Width for the loading bar
#define DISPLAY_INTERVAL 2000 // Time in ms for the display interval

#define I2C_SDA 11 // Defines I2C SDA for communication 
#define I2C_SCL 14 // Defines I2C SCL for communication 
#define DEBUG_OUTPUT
#define TFT_OUTPUT

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
SCD30 airSensor;

void setup() {
    
    Serial.begin(115200); // Transfer rate
    Serial.println("initializing start"); // Initializing starts

    Wire.begin();
    if (airSensor.begin()) {
        Serial.println("Sensor found"); // SCD30 sensor found
    } else {
        Serial.println("Sensor not found"); // SCD30 sensor not found
    }
    if (tcs.begin()) {
        Serial.println("Found sensor TCS34725"); // TCS34725 found
    } else {
        Serial.println("No TCS34725 found ... check your connections"); // TCS34725 not found
        while (1); // stop!
    }


    tft.initR(); // Initializes the ST7735 chip
    Serial.println("initializing finished"); // Initializing finishes
}

void tft_loading_bar(int percent) {
    static int last_percent = 0; // Last percentage set to 0

    if (percent == last_percent) {
        // skip procedure, if value has not changed
        return;
    }

    tft.fillRect(TFT_BAR_X, TFT_BAR_Y, TFT_WIDTH, TFT_BAR_HEIGHT, ST7735_WHITE);
    if (percent > 0) {
        Serial.print("TFT X WIDTH "); // Percentage, from 0 to 128 (pixels). 64 = 50%; at 128 will change the screen.
        Serial.println((TFT_WIDTH * percent) / 100); // Prints percentage
        tft.fillRect(TFT_BAR_X, TFT_BAR_Y, (TFT_WIDTH * percent) / 100, TFT_BAR_HEIGHT, ST7735_RED); // Loading bar
        last_percent = percent; // New value
    }
}

void tft_screen_climate(){
    int co2 = airSensor.getCO2(); // Get the value for co2
    Serial.println(co2); // Prints co2
    float humidity = airSensor.getHumidity(); // Get the value for humidity
    Serial.println(humidity); // Prints humidtity
    float temperature = airSensor.getTemperature(); // Get the value for temperature
    Serial.println(temperature); // Prints temperature

    tft.setTextColor(ST7735_BLACK); // Set the text color to black
    tft.fillRect(0, 0, 128, 26, ST7735_GREEN); // Green rectangle
    tft.setCursor(45, 8); // Sets the cursor at X: 8 Y: 24
    tft.setTextSize(2); // Set the text size to 2
    tft.println("CO2 "); // First word of the green rectangle

    tft.setTextColor(ST7735_BLACK); // Set the text color to black
    tft.fillRect(0, 26, 128, 27, ST7735_GREEN); // Green rectangle
    tft.setCursor(25, 24); // Sets the cursor at X: 34 Y: 24
    tft.println(co2); // Prints the co2 value
    tft.setCursor(60, 24); // Sets the cursor at X: 55 Y: 24
    tft.println(" ppm"); // Co2  symbol

    tft.fillRect(0, 53, 128, 53, ST7735_CYAN); // Cyan rectangle
    tft.setCursor(6, 59);   // Sets the cursor at X: 8 Y: 75
    tft.println("Temperatur"); // First word of the cyan rectangle

    tft.setTextColor(ST7735_BLACK); // Set the text colour to black
    tft.setCursor(23, 80); // Sets the cursor at X: 75 Y: 75

    tft.println(temperature); // Prints the temperature value
    tft.setCursor(78, 80); // Sets the cursor at X: 100 Y: 75
    tft.println(" C"); // Temperature symbol

    tft.fillRect(0, 106.65, 128, 54, ST7735_YELLOW); // yellow rectangle
    tft.setCursor(15, 114); // sets the cursor at x:8 y:129 
    tft.println("Humidity"); // First word of the yellow rectangle
    tft.setCursor(29, 132); // sets the cursor at x:90 y:129 
    tft.println(humidity); // Prints the humidity value
    tft.setCursor(89, 132); // sets the cursor at x:122 y:129
    tft.println("%"); // Percentage symbol
    tft.setTextSize(1); // Set the text size to 1
}

void tft_colors(){
    uint16_t r, g, b, c; // Defines r, g, b, and c.
    tcs.getRawData(&r, &g, &b, &c); // Get the values for r, g, b and c.

    tft.setTextSize(2); // Set the text size to 2
    tft.fillRect(0, 0, 128, 40, ST7735_RED); // Red rectangle
    tft.setTextColor(ST7735_BLACK); // Sets the text color to black
    tft.setCursor(8, 20); // Sets the cursor at X: 8 Y: 20
    tft.println("R: "); // First word of the red rectangle
    tft.setCursor(30, 20); // Sets the cursor at X: 33 Y: 20
    tft.println(r); // Red color value

    tft.fillRect(0, 40, 128, 40, ST7735_GREEN); // Green rectangle
    tft.setCursor(8, 60); // Sets the cursor at X: 8 Y: 60 
    tft.println("G: "); // First word of the green rectangle
    tft.setCursor(30, 60); // Sets the cursor at X: 45 Y: 60
    tft.println(g); // Green color value
    
    tft.fillRect(0, 80, 128, 40, ST7735_BLUE); // Blue Rectangle
    tft.setCursor(8, 100); // Sets the cursor at X: 8 Y: 100
    tft.println("B: "); // First word of the blue rectangle
    tft.setCursor(30, 100); // Sets the cursor at X: 38 Y: 100
    tft.println(b); // Blue color value

    tft.fillRect(0, 120, 128, 40, ST7735_WHITE); // White rectangle
    tft.setCursor(8, 140); // Sets the cursor at X: 8 Y: 140
    tft.println("C: "); // First word of the white rectangle
    tft.setCursor(30, 140); // Sets the cursor at X: 45 Y: 140
    tft.println(c); // White color value
}

void tft_screen_reset(int reset){
    tft.fillScreen(ST7735_BLACK); // Fill the screen black, reset the rectangles 
    delay(reset);                  
}

void changeScreen(){
	static unsigned long last_display_change = 0;   // Start value set to 0
	static uint8_t display_page = 0;    // Start value set to 0
    int ms_since_last_change = 0;   // Start value set to 0   
    int status_bar_percent = 0; // Start value set to 0

	if (last_display_change == 0 || (millis() - last_display_change) > DISPLAY_INTERVAL) {
		Serial.println(F("Changing Display...")); // Prints Changing Display...
		last_display_change = millis(); // Miliseconds since last change
		display_page = (display_page + 1 ) % 2; // If the remainder when dividing is:
		if (display_page == 0) { // If 0
			tft_screen_climate(); // Climate
		} else if (display_page == 1)  { // If 1
			tft_colors(); // Colors
		}
    }

    ms_since_last_change = millis() - last_display_change; // MS since last change 
    status_bar_percent = (ms_since_last_change * 100) / DISPLAY_INTERVAL; // Bar being drawn

    tft_loading_bar(status_bar_percent); // Changes the screen

}

void loop() {
   changeScreen(); // Code
}

