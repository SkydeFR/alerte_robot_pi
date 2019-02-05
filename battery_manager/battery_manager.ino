#include <Wire.h>
#include <SparkFunBQ27441.h>

void setup()
{
  Serial.begin(115200);
  if (!lipo.begin())
  {
    Serial.println("Error: Unable to communicate with BQ27441.");
    Serial.println("Check wiring and try again.");
    Serial.println("(Battery must be plugged into Battery Babysitter!)");
    while (1) ;
  }
  Serial.println("Connected to BQ27441!");
  Serial.println("-----");
  lipo.setCapacity(5100); // Configure BQ27441 to assume a 5100 mAh battery
}

void loop() {
  int soc = lipo.soc(); // Read state-of-charge (in %)
  int volts = lipo.voltage(); // Read voltage (in mV)
  int current = lipo.current(AVG); // Read average current (in mA)
  int capacity = lipo.capacity(REMAIN); // Read remaining capacity (in mAh)
  int power = lipo.power(); // Read power consumption (in mW)
  int health = lipo.soh(); // Read state-of-health (in %)
  
  Serial.print("State-of-charge : ");
  Serial.print(soc);
  Serial.println("%");
  
  Serial.print("Average current : ");
  Serial.print(volts);
  Serial.println("mV");
  
  Serial.print("Average current : ");
  Serial.print(current);
  Serial.println("mA");
  
  Serial.print("Remaining capacity : ");
  Serial.print(capacity);
  Serial.println("mAh");
  
  Serial.print("Power consumption : ");
  Serial.print(power);
  Serial.println("mW");
  
  Serial.print("State-of-health : ");
  Serial.print(health);
  Serial.println("%");
  
  Serial.println("-----");
  delay(10000);
  
}
