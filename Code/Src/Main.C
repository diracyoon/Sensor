#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#include "bme280.h"

using namespace std;

#define IIC_Dev  "/dev/i2c-1"

void user_delay_ms(uint32_t period);
int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);

int fd;

int main(int argc, char* argv[])
{
  struct bme280_dev dev;
  int8_t rslt = BME280_OK;
  
  if((fd = open(IIC_Dev, O_RDWR)) < 0)
    {
      cerr << "Failed to open the i2c bus %s" << argv[1] << endl;
      exit(1);
    }
  if(ioctl(fd, I2C_SLAVE, 0x76) < 0)
    {
      cerr << "Failed to acquire bus access and/or talk to slave." << endl;
      exit(1);
    }
  
  dev.dev_id = BME280_I2C_ADDR_PRIM;
  dev.intf = BME280_I2C_INTF;
  dev.read = user_i2c_read;
  dev.write = user_i2c_write;
  dev.delay_ms = user_delay_ms;

  rslt = bme280_init(&dev);
  cout << "BME280 Init Result is: " << (int)rslt << endl;
  
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

  ofstream fout;
  int date_yesterday = -1;
  
  while(1)
    {
      /*getting time info.*/
      time_t current_time = time(NULL);
      struct tm* time_tm = localtime(&current_time);

      int date_today = time_tm->tm_mday;
      
      if(date_today!=date_yesterday)
	{
	  int year = time_tm->tm_year + 1900;
	  int month = time_tm->tm_mon + 1;
	  int date = time_tm->tm_mday;

	  string name = to_string(year);
	  name += "_";
	  name += string(2 - to_string(month).length(), '0') + to_string(month);
	  name += "_";
	  name += string(2 - to_string(date).length(), '0') + to_string(date);
	  name += ".dat";

	  //close file for yesterday
	  if(fout.is_open()) fout.close();

	  //open file for today
	  fout.open(name, std::ofstream::app);

	  date_yesterday = date_today;
	}

      /*getting sensor data*/
      dev.delay_ms(10000);

      struct bme280_data comp_data;
      rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);

      fout << time_tm->tm_hour << ":" << time_tm->tm_min << ":" << time_tm->tm_sec << "," << comp_data.temperature << "," << comp_data.pressure/100 << "," << comp_data.humidity << endl;
    }
  
  return 0;
}//int main(int argc, char* argv[])

//////////

void user_delay_ms(uint32_t period)
{
  usleep(period*1000);

  return;
}//void user_delay_ms(uint32_t period)

//////////

int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
  write(fd, &reg_addr,1);
  read(fd, data, len);

  return 0;
}//int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)

//////////

int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
  int8_t *buf;
  buf = (int8_t*)malloc(len +1);
  buf[0] = reg_addr;
  memcpy(buf +1, data, len);
  write(fd, buf, len +1);
  free(buf);
  
  return 0;
}//int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)

//////////
