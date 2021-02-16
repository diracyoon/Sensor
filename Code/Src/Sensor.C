#include "Sensor.h"

//////////

int Sensor::fd = -1;

//////////

Sensor::Sensor()
{
  //sampling periond in second
  delay = 1;
  
  if((fd = open(IIC_Dev, O_RDWR)) < 0) throw "Failed to open the i2c bus";
  if(ioctl(fd, I2C_SLAVE, 0x76) < 0) throw "Failed to acquire bus access and/or talk to slave.";

  dev.dev_id = BME280_I2C_ADDR_PRIM;
  dev.intf = BME280_I2C_INTF;
  dev.read = user_i2c_read;
  dev.write = user_i2c_write;
  dev.delay_ms = user_delay_ms;

  rslt = bme280_init(&dev);
  //cout << "BME280 Init Result is: " << (int)rslt << endl;

  /* Recommended mode of operation*/
  dev.settings.osr_h = BME280_OVERSAMPLING_1X;
  dev.settings.osr_p = BME280_OVERSAMPLING_16X;
  dev.settings.osr_t = BME280_OVERSAMPLING_2X;
  dev.settings.filter = BME280_FILTER_COEFF_16;
  dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

  uint8_t settings_sel;
  settings_sel = BME280_OSR_PRESS_SEL;
  settings_sel |= BME280_OSR_TEMP_SEL;
  settings_sel |= BME280_OSR_HUM_SEL;
  settings_sel |= BME280_STANDBY_SEL;
  settings_sel |= BME280_FILTER_SEL;

  rslt = bme280_set_sensor_settings(settings_sel, &dev);
  rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev);


  /*getting sensor data*/
  dev.delay_ms(delay*1000);

  struct bme280_data comp_data;
  rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);

  cout << "Temp: " << comp_data.temperature << endl;
  cout << "Pressure: " << comp_data.pressure/100 << endl;
  cout << "RH: " << comp_data.humidity << endl;
}//Sensor::Sensor()

//////////

Sensor::~Sensor()
{
}//Sensor::~Sensor()

//////////

void Sensor::user_delay_ms(uint32_t period)
{
  usleep(period*1000);

  return;
}//void user_delay_ms(uint32_t period)

//////////

int8_t Sensor::user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
  write(fd, &reg_addr,1);
  read(fd, data, len);

  return 0;
}//int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)

//////////

int8_t Sensor::user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
  int8_t*buf = new int8_t[len+1];
  buf[0] = reg_addr;
  memcpy(buf+1, data, len);
  write(fd, buf, len +1);

  delete[] buf;

  return 0;
}//int8_t Sensor::user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)

//////////
