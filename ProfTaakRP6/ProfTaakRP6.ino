//COM10
#include <Timer.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#define PIN 6
#define NLEDS 9

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NLEDS, PIN, NEO_GRB + NEO_KHZ800);
Timer timer;

String kenteken = "";

int red_led[NLEDS];
int green_led[NLEDS];
int blue_led[NLEDS];

String command = "";
String fuelType = "";

int fill_level = -1;
int fuel_start = 0; // % of total capacity the car starts with.

double fuel_capacity = 70; // Liters

void setup()
{
  //kenteken = "GL-07-KL"; fuelType = "LPG";
  kenteken = "RD-08-MN"; fuelType = "Petrol";
  //kenteken = "BL-09-PQ"; fuelType = "Diesel";

  Serial.begin(9600);
  
  randomSeed(analogRead(0));
  fill_level = random(5, 25);
  fuel_start = fill_level * 0.7;

  showFuelLevel(fill_level);
  
  strip.begin();
  strip.show();
}

void loop()
{
  timer.update();

  command = getCommand();

  if (command == "reset") {
    fill_level = random(5, 25);
    fuel_start = fill_level * 0.7;
    showFuelLevel(fill_level);

    resetStrip();
  } else if (command == "start") {
    if (fill_level >= 0 && fill_level < 100) {
      fillCar();
    }
  }
}
