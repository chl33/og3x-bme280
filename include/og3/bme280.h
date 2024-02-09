#pragma once

#ifndef NATIVE
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>
#endif

#include <og3/temp_humidity.h>

namespace og3 {

class Bme280 : public TempHumidity {
 public:
  Bme280(const char* temp_name, const char* humidity_name, const char* pressure_name,
         ModuleSystem* module_system_, const char* description, VariableGroup* vg,
         bool publish = true, bool ha_discovery = true);

  float pressure() const { return m_pressure.value(); }
  const FloatVariable& pressureVar() const { return m_pressure; }

  void read();

 private:
#ifndef NATIVE
  Adafruit_BME280 m_bme280;
#endif
  FloatVariable m_pressure;
};

}  // namespace og3
