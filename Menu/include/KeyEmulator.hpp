/*
 * KeyEmulator.hpp
 *
 * References
 * http://thiemonge.org/getting-started-with-uinput
 * https://web.archive.org/web/20120710121647/http://www.einfochips.com/download/dash_jan_tip.pdf
 * http://www.linux.it/~rubini/docs/input26/input26.html
 * http://git.nosuchfile.org/?p=uinput-sample.git;a=blob_plain;f=uinput-sample.c;hb=HEAD
 *
 *  Created on: 07/ago/2016
 *      Author: daniele
 */

#ifndef KEYEMULATOR_HPP_
#define KEYEMULATOR_HPP_
#include <thread>
#include <iostream>
#include <random>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <dirent.h>

#define SYS_PATH "/sys/class/input/"
#define EVT_PATH "/dev/input/"

#define KEY_EMULATOR_DIE(str, args...) do { \
        perror(str); \
        fprintf(stderr,"%s:%d\n",__FILE__,__LINE__); \
        exit(EXIT_FAILURE); \
    } while(0)





/*
 * Real Example
 *
 *
 *
[root@homer input]# ls -l
total 0
crw------- 1 root root 13, 64 Jan  1  1970 event0
[root@homer input]# evtest event0
Input driver version is 1.0.1
Input device ID: bus 0x19 vendor 0x1 product 0x1 version 0x100
Input device name: "ahb:apb:gpio_keys"
Supported events:
  Event type 0 (EV_SYN)
  Event type 1 (EV_KEY)
    Event code 28 (KEY_ENTER)
    Event code 103 (KEY_UP)
    Event code 105 (KEY_LEFT)
    Event code 106 (KEY_RIGHT)
    Event code 108 (KEY_DOWN)
Properties:
Testing ... (interrupt to exit)
Event: time 1470768867.077489, type 1 (EV_KEY), code 103 (KEY_UP), value 1
Event: time 1470768867.077489, -------------- SYN_REPORT ------------
Event: time 1470768867.350997, type 1 (EV_KEY), code 103 (KEY_UP), value 0
Event: time 1470768867.350997, -------------- SYN_REPORT ------------

Event: time 1470768876.142763, type 1 (EV_KEY), code 108 (KEY_DOWN), value 1
Event: time 1470768876.142763, -------------- SYN_REPORT ------------
Event: time 1470768876.484660, type 1 (EV_KEY), code 108 (KEY_DOWN), value 0
Event: time 1470768876.484660, -------------- SYN_REPORT ------------

Long click
Pressed
Event: time 1470769541.133427, type 1 (EV_KEY), code 103 (KEY_UP), value 1
Event: time 1470769541.133427, -------------- SYN_REPORT ------------
Released
Event: time 1470769545.002499, type 1 (EV_KEY), code 103 (KEY_UP), value 0
Event: time 1470769545.002499, -------------- SYN_REPORT ------------


*
*
*
*/

class KeyEmulator {

private:

    std::thread key_thread;
    bool running;
    int                    fd;
    struct uinput_user_dev uidev;
    struct input_event     ev;
    struct termios t;
	char sysfs_device_name[16];
	string input;
	string event;
	bool key_pressed;
	__u16 current_key;
	__s32 current_val;


    void stdin_unbuffered(void)
    {
    	tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
    	t.c_lflag &= ~ICANON; //Manipulate the flag bits to do what you want it to do
    	t.c_lflag &= ~ECHO; //Manipulate the flag bits to do what you want it to do
   	tcsetattr(STDIN_FILENO, 0, &t); //Apply the new settings
    }


    void stdin_buffered(void)
    {
    	tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
    	t.c_lflag |= ICANON; //Manipulate the flag bits to do what you want it to do
       	t.c_lflag |= ECHO; //Manipulate the flag bits to do what you want it to do
    	tcsetattr(STDIN_FILENO, 0, &t); //Apply the new settings
    }

    void init(const char *name,__u16 type,__u16 vendor,__u16 product,__u16 version) {
    	DIR *pdir;
    	struct dirent *pent;

    	this->running = false;
    	this->key_pressed = false;
    	this->current_key = KEY_ESC;

        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if(fd < 0)      							KEY_EMULATOR_DIE("error: open");
        if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)		KEY_EMULATOR_DIE("error: ioctl");
        if(ioctl(fd, UI_SET_EVBIT, EV_SYN) < 0)  	KEY_EMULATOR_DIE("error: ioctl");
        if(ioctl(fd, UI_SET_KEYBIT, KEY_ENTER) < 0) KEY_EMULATOR_DIE("error: ioctl");
        if(ioctl(fd, UI_SET_KEYBIT, KEY_UP) < 0)  	KEY_EMULATOR_DIE("error: ioctl");
        if(ioctl(fd, UI_SET_KEYBIT, KEY_DOWN) < 0)  KEY_EMULATOR_DIE("error: ioctl");
        if(ioctl(fd, UI_SET_KEYBIT, KEY_LEFT) < 0)  KEY_EMULATOR_DIE("error: ioctl");
        if(ioctl(fd, UI_SET_KEYBIT, KEY_RIGHT) < 0) KEY_EMULATOR_DIE("error: ioctl");

        memset(&uidev, 0, sizeof(uidev));
        snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
        uidev.id.bustype = BUS_USB;
        uidev.id.vendor  = 0xdac0;
        uidev.id.product = 0xbeed;
        uidev.id.version = 1;
        if(write(fd, &uidev, sizeof(uidev)) < 0) KEY_EMULATOR_DIE("error: write");
        if(ioctl(fd, UI_DEV_CREATE) < 0)   	KEY_EMULATOR_DIE("error: ioctl");
        memset(&sysfs_device_name, 0, sizeof(sysfs_device_name));
        ioctl(fd, UI_GET_SYSNAME(sizeof(sysfs_device_name)), sysfs_device_name);
        input = string(sysfs_device_name);
        pdir = opendir((string(SYS_PATH)+input).c_str());
        while ((pent = readdir(pdir)) != NULL) {
        	if (strstr(pent->d_name, "event") == pent->d_name) {
                  event = string(pent->d_name);
                  break;
            }
        }
        (void)closedir(pdir);
    }
    void k_write(__u16 t,__u16 c, __s32 v) {
        memset(&ev, 0, sizeof(struct input_event));
        ev.type = t;
        ev.code = c;
        ev.value = v;
        if(write(fd, &ev, sizeof(struct input_event)) < 0)
        	KEY_EMULATOR_DIE("error: write");
        usleep(100);
        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_SYN;
        if(write(fd, &ev, sizeof(struct input_event)) < 0)
        	KEY_EMULATOR_DIE("error: write");
        usleep(100);
    }
    void key_thread_emulator(bool mode) {
        struct timeval tv;
        fd_set fds;
        tv.tv_sec = 1;
        tv.tv_usec = 0; // 100 ms polling
    	if(running) KEY_EMULATOR_DIE("Already running");
    	running = true;
    	char ch;
    	while(running) {
    		FD_ZERO(&fds);
    		FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    		select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    		if(FD_ISSET(STDIN_FILENO, &fds)) {
    			ch = std::cin.get();
				current_val = 1;
				switch(ch) {
					case '2':
						current_key = KEY_DOWN;
						break;
					case '8':
						current_key = KEY_UP;
						break;
					case '6':
						current_key = KEY_RIGHT;
						break;
					case '4':
						current_key = KEY_LEFT;
						break;
					case '0':
						current_key = KEY_ENTER;
						break;
					case '.':
						running = false;
						break;
					case '+':
						current_val = 0;
						break;

					default:
						current_key = KEY_ESC;
    			}
    			if(running) {
    				k_write(EV_KEY,current_key,current_val);
    				if(mode) {
    					usleep(100000);
    					current_val = 0;
    					k_write(EV_KEY,current_key,current_val);
    				}
    			}

    		}
    	}
    }

public:
    KeyEmulator() {
    	init("uinput-sample",BUS_USB,0xdac0,0xbeed,1);
    }
    KeyEmulator(const char *name,__u16 type,__u16 vendor,__u16 product,__u16 version) {
    	init(name, type, vendor, product, version);
    }
    KeyEmulator(const char *name) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 65535);
    	init(name,BUS_USB,dis(gen),dis(gen),1);
    }


    ~KeyEmulator() {
    	if(running) stop();
    	if(ioctl(fd, UI_DEV_DESTROY) < 0) KEY_EMULATOR_DIE("error: ioctl");
   	    close(fd);

    }
	void start() {
        stdin_unbuffered();
		this->key_thread = std::thread([&] { KeyEmulator::key_thread_emulator(true); });
	}
	void stop() {
		this->running = false;
		this->key_thread.join();
		stdin_buffered();
	}
	bool is_running() { return(running); }
	string get_event() {
		return(string(EVT_PATH)+event);
	}
	string get_input() {
		return(string(SYS_PATH)+input);
	}
};


#endif /* KEYEMULATOR_HPP_ */
