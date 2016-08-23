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
using namespace obs;

namespace homerio {

class I2cBusEmulated : public I2cBus {
private:
    // Observers
    Subject<void (int filedes, const void *buffer, size_t size)> write_obs; // Triggered on key released
    // Observer Registration
    Registration reg_write_obs;

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
		reg_write_obs = write_obs.registerObserver(f);
	}

};

class GpioPortEmulated : public GpioPort  {
private:
    // Observers
    Subject<void (int filedes, const void *buffer, size_t size)> write_obs; // Triggered on key released
    // Observer Registration
    Registration reg_write_obs;

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
			reg_write_obs = write_obs.registerObserver(f);
		}

};

}

#endif /* HWEMULATED_HPP_ */
