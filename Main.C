#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include "bme280.h"

#define I2C_Dev  "/dev/i2c-1"

using namespace std;

int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
void user_delay_ms(uint32_t period);

int fd;

int main(int argc, char* argv[])
{
  struct bme280_dev dev;
  int8_t rslt = BME280_OK;

  int fd = open(I2C_Dev, O_RDWR);
  if(fd < 0)
    {
      cout << "Failed to open the i2c bus " << argv[1] << endl;
      exit(1);
    }

  if(ioctl(fd, I2C_SLAVE, 0x77) < 0)
    {
      cout << "Failed to acquire bus access and/or talk to slave" << endl;
      exit(1);
    }

  dev.dev_id = BME280_I2C_ADDR_SEC; //0x77
  dev.intf = BME280_I2C_INTF;
  dev.read = user_i2c_read;
  dev.write = user_i2c_write;
  dev.delay_ms = user_delay_ms;

  rslt = bme280_init(&dev);
  
  return 0;
}

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

void user_delay_ms(uint32_t period)
{
  usleep(period*1000);

  return;
}//void user_delay_ms(uint32_t period)

//////////
