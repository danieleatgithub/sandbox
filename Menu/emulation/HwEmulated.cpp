/*
 * HwEmulated.cpp
 *
 *  Created on: 22/ago/2016
 *      Author: daniele
 */


#include <HwEmulated.hpp>
#include <Winstar.h>
#include <linux/i2c-dev.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

namespace homerio {



int I2cBusEmulated::open(const char *file, int flag) {
	return(100);
}
int I2cBusEmulated::close(int fd) {
	return(0);
}
int I2cBusEmulated::read(int fd, void *buf, size_t nbyte) {
	return(0);
}
int I2cBusEmulated::write(int fd, const void *buffer, size_t size) {
	return(size);
}
int I2cBusEmulated::ioctl(int fd, unsigned long int request, ...) {
//	va_list argptr;
//	unsigned int address;
//	cerr << "I2cBusEmulated ioctl" << "req=" << hex << request;
//
//	switch(request) {
//	case I2C_SLAVE:
//		address = va_arg(argptr,unsigned int);
//		cerr << "add=" << hex << address << endl;
//		break;
//	default:
//		return(-1);
//	}
	return(0);
}
__off_t I2cBusEmulated::lseek (int fd, __off_t __offset, int __whence) {
	return(0);
}



int GpioPortEmulated::open(const char *file, int flag) {
	return(100);
}
int GpioPortEmulated::close(int fd) {
	return(0);
}
int GpioPortEmulated::read(int fd, void *buf, size_t nbyte) {
	return(0);
}
int GpioPortEmulated::write(int fd, const void *buffer, size_t size) {
	write_obs(fd, buffer, size);
	return(size);
}
int GpioPortEmulated::ioctl(int fd, unsigned long int request, ...) {
//	int ret = 0;
//	cerr << "GpioPort ioctl" << "req=" << hex << request;
//
//	switch(request) {
//	default:
//		cerr << "(UNKNOW)";
//		ret = -1;
//		break;
//	}
//	cerr << endl;
	return(0);
}
__off_t GpioPortEmulated::lseek (int fd, __off_t __offset, int __whence) {
	return(0);
}

}
