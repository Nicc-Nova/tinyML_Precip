#include "model.h"
#include <EloquentTinyML.h>
#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>

#define INPUTS 3
#define OUTPUTS 5
#define Tensor_ARENA 8 * 1024

Eloquent::TinyML::TfLite<INPUTS, OUTPUTS, Tensor_ARENA> precip_model;


void setup() {
  //setup code here, to run once:
  delay(5000);
  Serial.begin(9600);
  Serial.println("Setup!");
  precip_model.begin(model);
  
  //instanciate the pressure sensor
  if (!BARO.begin())
  {
    Serial.println("Failed to initialize pressure sensor!");
    while (1)
      Serial.println("Failed to initialize pressure sensor!");
      delay(5000);
  } else{
    Serial.println("SUCCESS: initialized pressure sensor!");
  }

  //instanciate the temperature and humidity sensor
  if (!HTS.begin())
  {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1)
      Serial.println("Failed to initialize humidity temperature sensor!");
      delay(5000);
  } else{
    Serial.println("SUCCESS: initialized humidity temperature sensor!");
  }
}

void loop() {
  //main code here, to run repeatedly:
  float temp = HTS.readTemperature();
  float hum = HTS.readHumidity();
  float pres = BARO.readPressure();

  float inputs[INPUTS] = {temp, hum, pres};
  float output[OUTPUTS] = {0};
  precip_model.predict(inputs, output);

  Serial.print("Temperature : ");
  Serial.print(temp);
  Serial.print(" Pressure : ");
  Serial.print(pres);
  Serial.print(" Humidity : ");
  Serial.println(hum);
  //print result
  Serial.print("Today's precipitation: ");
  Serial.print(output[1]);
  Serial.println("cm.");
  delay(5000);
}