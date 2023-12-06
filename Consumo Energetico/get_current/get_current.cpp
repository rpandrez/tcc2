#include <Wire.h>
#include <Adafruit_INA219.h>

//Adafruit_INA219 ina219; //DEFAULT 0x040
Adafruit_INA219 ina219_B(0x040);

void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) {
      delay(1);
  }

  uint32_t currentFrequency;
    
  // Inicializa o INA219.
  if (! ina219_B.begin()) {
    Serial.println("FALHA AO TENTAR LOCALIZAR O CHIP INA219");
    while (1) { delay(10); }
  }
  }

void loop(void) 
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219_B.getShuntVoltage_mV();
  busvoltage = ina219_B.getBusVoltage_V();
  current_mA = ina219_B.getCurrent_mA();
  power_mW = ina219_B.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  // Plotar os valores de corrente e tensão
  //Serial.print(busvoltage); Serial.print(","); Serial.println(current_mA);

  delay(100);
}
