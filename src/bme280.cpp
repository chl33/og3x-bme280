#include "og3/bme280.h"

#include <og3/units.h>

namespace og3 {

Bme280::Bme280(const char* temp_name, const char* humidity_name, const char* pressure_name,
               ModuleSystem* module_system_, const char* description, VariableGroup* vg,
               bool publish, bool ha_discovery)
    : TempHumidity(temp_name, humidity_name, module_system_, description, vg, publish,
                   ha_discovery),
      m_pressure(pressure_name, 0.0f, units::kHPascals, pressure_name,
                 publish ? 0 : VariableBase::kNoPublish, 0, vg) {
  m_pressure.setFailed();
  add_init_fn([this]() {
    if (m_ha_discovery) {
      m_ha_discovery->addDiscoveryCallback([this](HADiscovery* had, JsonDocument* json) {
        return had->addMeas(json, m_pressure, ha::device_type::kSensor,
                            ha::device_class::sensor::kAtmosphericPressure);
      });
    }
  });
#ifndef NATIVE
  add_start_fn([this]() { m_ok = m_bme280.begin(0x76); });
#endif
}

void Bme280::read() {
#ifndef NATIVE
  m_temperature = m_bme280.readTemperature();
  m_humidity = m_bme280.readHumidity();
  m_pressure = m_bme280.readPressure() / 100.0f;
  if (m_temperature.value() < -9) {
    m_temperature.setFailed();
  }
  if (m_humidity.value() > 100) {
    m_humidity.setFailed();
  }
#endif
}

}  // namespace og3
