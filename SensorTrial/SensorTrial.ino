// Timers
#include <SeqTimer.h>
// I2C
#include <Wire.h>
// Sensors
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_Simple_AHRS.h>


// Create sensor instances.
Adafruit_LSM303_Accel_Unified accel(30301);
Adafruit_LSM303_Mag_Unified   mag(30302);
Adafruit_BMP085_Unified       bmp(18001);

// Create simple AHRS algorithm using the above sensors.
Adafruit_Simple_AHRS          ahrs(&accel, &mag);

// Update this with the correct SLP for accurate altitude measurements
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

SeqTimer timSensor, timDisplay;

void setup()
{
  // SERIAL
  Serial.begin(115200);

  SetupOLED();
  
  // SENSORS
  // Initialize the sensors.
  accel.begin();
  mag.begin();
  bmp.begin();

  // initialize timers period
  timSensor.init(50, millis());
  timDisplay.init(150, millis()+17);
}


sensors_vec_t     orientation;
sensors_event_t   acceleration;
sensors_event_t   bmp_event;
float temperature;
float pressure;

void loop(void)
{
  if (timDisplay.getTimer())  { taskOLED(); }
  
  if (timSensor.getTimer())
  {
    // Use the simple AHRS function to get the current orientation.
    if (ahrs.getOrientation(&orientation))
    {
      /* 'orientation' should have valid .roll and .pitch fields */
      Serial.print("Orientation: ");
      Serial.print(getStrOrientation());
    }
    
    Serial.print("\t\tAcceleration: ");
    accel.getEvent(&acceleration);
    Serial.print(getStrAcceleration());
    
    // Calculate the altitude using the barometric pressure sensor
    bmp.getEvent(&bmp_event);
    
    //if (bmp_event.pressure)
    {
      /* Get ambient temperature in C */
      bmp.getTemperature(&temperature);
      bmp.getPressure(&pressure);
      
      Serial.print("\t\t");
      Serial.println(getStrBMP());
    }
  }
}

String getStrOrientation()
{
  String str;
  
  //str = "Orientation: X";
  str = F("X ");
  str += orientation.roll;
  str += F("  Y ");
  str += orientation.pitch;
  str += F("  Z ");
  str += orientation.heading;
  str += F("");
  
  return str;
}

String getStrAcceleration()
{
  String str;
    
  //str = "Acceleration: X";
  str = F("X ");
  str += acceleration.acceleration.x;
  str += F("  Y ");
  str += acceleration.acceleration.y;
  str += F("  Z ");
  str += acceleration.acceleration.z;
  str += F("");
   
  return str;
}

String getStrBMP()
{
  
  String str;
    
  /* Convert atmospheric pressure, SLP and temp to altitude */
  str = F("A: ");
  str += bmp.pressureToAltitude(seaLevelPressure, bmp_event.pressure, temperature);
  str += F("  T: ");
  str += temperature;
  str += F("  P: ");
  str += (long) pressure;
  str += F("");
   
  return str;
}


