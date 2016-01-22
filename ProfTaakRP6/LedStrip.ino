void showFuelLevel(int fuel) {
  double lower = 0;
  double upper = 0;

  for (int i = 0; i < 8; i++) {
    lower = i * 12.5;
    upper = (i + 1) * 12.5;
    int y = 0;
    if(fuel > upper){
      y = 200;
    } else if (fuel >= lower && fuel <= upper) {
      y = map((fuel - lower), 0, 12.5, 0, 200);
    }
    else {
      y = 0;
    }
    if (fuelType == "Petrol") {
      red_led[i] = y;
    } else if (fuelType == "LPG") {
      green_led[i] = y;
    } else if (fuelType == "Diesel") {
      blue_led[i] = y;
    }
  }
  WriteBuffer();
}

void WriteBuffer() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, red_led[i], green_led[i], blue_led[i]);
  }
  strip.show();
}

void resetStrip() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}

void fillCar() {
  fill_level ++;

  showFuelLevel(fill_level);

  if (fill_level < 100) {
    timer.after(100, fillCar);
  }
  else{
    red_led[9] = 0;
    green_led[9] = 0;
    blue_led[9] = 0;
    WriteBuffer();
    Serial.print("%pay:"); Serial.print(kenteken); Serial.print(",amountOfFuel:"); Serial.print((fuel_capacity - fuel_start)); Serial.println('#');
  }
}
