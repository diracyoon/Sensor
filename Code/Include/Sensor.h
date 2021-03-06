#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#include "bme280.h"

using namespace std;

#define IIC_Dev  "/dev/i2c-1"

class Sensor
{
 public:
  Sensor();
  ~Sensor();

  void Run();
  
  static void user_delay_ms(uint32_t period);
  static int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
  static int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
  
 protected:
  
  struct bme280_dev dev;
  int8_t rslt;

  static int fd;

  int delay;
};
