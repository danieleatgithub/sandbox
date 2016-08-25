/*
 * HwEmulated.hpp
 *
 *  Created on: 22/ago/2016
 *      Author: daniele
 */

#ifndef HWEMULATED_HPP_
#define HWEMULATED_HPP_

#include <HwLayer.hpp>
#include <linux/i2c-dev.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include "Observer.h"
#include <homerd.h>


using namespace obs;

namespace homerio {

class I2cBusEmulated : public I2cBus {
private:
    // Observers
    Subject<void (int filedes, const void *buffer, size_t size)> write_obs; // Triggered on key released
    // Observer Registration
    vector<Registration> reg_write_obs;

public:

	I2cBusEmulated(const char *bus) : I2cBus(bus) {};
	 ~I2cBusEmulated() {};

	int open(const char *file, int flag);
	int read(int fd, void *buf, size_t nbyte);
	int write(int filedes, const void *buffer, size_t size);
	int ioctl(int fd, unsigned long int __request, ...);
	__off_t lseek (int fd, __off_t __offset, int __whence);
	int close(int fd);

	void reg_write(std::function<void (int filedes, const void *buffer, size_t size)> f) {
    	Registration reg = write_obs.registerObserver(f);
    	reg_write_obs.push_back(reg);
	}

};

class GpioPortEmulated : public GpioPort  {
private:
    // Observers
    Subject<void (int filedes, const void *buffer, size_t size)> write_obs; // Triggered on key released
    // Observer Registration
    vector<Registration> reg_write_obs;

public:
	 GpioPortEmulated(const char *name) : GpioPort(name) {};
	 ~GpioPortEmulated() {};

		int open(const char *file, int flag);
		int read(int fd, void *buf, size_t nbyte);
		int write(int filedes, const void *buffer, size_t size);
		int ioctl(int fd, unsigned long int __request, ...);
		__off_t lseek (int fd, __off_t __offset, int __whence);
		int close(int fd);

		void reg_write(std::function<void (int filedes, const void *buffer, size_t size)> f) {
	    	Registration reg = write_obs.registerObserver(f);
	    	reg_write_obs.push_back(reg);
		}

};



class BoardEmulated {
	I2cBusEmulated    i2c_0;
	GpioPortEmulated  lcd_backlight;
	GpioPortEmulated  lcd_reset;

public:
	BoardEmulated() :
		i2c_0(I2C_BUS), lcd_backlight(LCD_BACKLIGHT_PIN), lcd_reset(LCD_RESET_PIN)  {

	};
	~BoardEmulated() {
	}
	I2cBusEmulated& getI2c0() {
		return i2c_0;
	}

	GpioPortEmulated& getLcdBacklight() {
		return lcd_backlight;
	}

	GpioPortEmulated& getLcdReset() {
		return lcd_reset;
	}

};

}
#endif /* HWEMULATED_HPP_ */
