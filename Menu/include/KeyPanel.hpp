/*
 * KeyPanel.h
 *
 *  Created on: 14/feb/2016
 *      Author: daniele
 */

#ifndef KEYPANEL_HPP_
#define KEYPANEL_HPP_

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include "homerd.h"
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <string>
#include "Observer.h"
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>

using namespace obs;
using namespace std;
using namespace log4cplus;

namespace homerio {

/**
 * All keybutton available in homer
 */
enum Button_e {
	BUTTON_ENTER = KEY_ENTER,//!< BUTTON_ENTER
	BUTTON_UP = KEY_UP,      //!< BUTTON_UP
	BUTTON_DOWN = KEY_DOWN,  //!< BUTTON_DOWN
	BUTTON_LEFT = KEY_LEFT,  //!< BUTTON_LEFT
	BUTTON_RIGHT = KEY_RIGHT //!< BUTTON_RIGHT
};


class KeyButton {
private:
	struct input_event key;
	struct timeval t0;
	struct timeval t1;
	std::chrono::milliseconds press_time;
	std::chrono::milliseconds long_threshold;
	bool pressed;
	bool valid;

public:
	KeyButton() {
		valid = false;
		pressed = false;
		long_threshold = std::chrono::milliseconds(1000);
	}
	void load_event(struct input_event ev) {
		key = ev;
		if(ev.value == 1) {
			t0 = ev.time;
			pressed = true;
		} else {
			pressed = false;
			t1 = ev.time;
		}
		valid = false;
	};

	void validate_event(struct input_event ev) {
		valid = true;
		// SYN on button release
		if(ev.value == 0 && !pressed) {
			press_time = std::chrono::milliseconds(t1.tv_usec - t0.tv_usec)/1000+std::chrono::milliseconds(t1.tv_sec - t0.tv_sec)*1000;
		}
	}

	bool isPressEvent() { return(key.value == 1); }
	bool isLongEvent() { return (this->press_time >=  long_threshold); }
	const Button_e get_key() { return ((Button_e) key.code); }
	std::chrono::milliseconds get_press_ms() { return(press_time); }

	const std::chrono::milliseconds& getLongThreshold() const {
		return long_threshold;
	}

	void setLongThreshold(const std::chrono::milliseconds& longThreshold) {
		long_threshold = longThreshold;
	}
	operator std::string() const {
		return(string("Key=")+std::to_string(key.code));
	}
};



class KeyPanel {

  private:
    KeyButton key;
    // Observers
    obs::Subject<void (KeyButton& k )> key_press_obs;	// Triggered on key press
    obs::Subject<void (KeyButton& k )> key_release_obs; // Triggered on key released
    obs::Subject<void (KeyButton& k )> key_long_obs;	// Triggered on long time duration
    // Observer Registration
    Registration reg_press_obs;
    Registration reg_release_obs;
    Registration reg_long_obs;

    struct timeval tout;
    string event_dev;
    bool running;
    std::thread key_thread;

    /**
     * event reader
     */
    void key_thread_reader() {
    	Logger logdev = Logger::getInstance(LOGDEVICE);
    	this->running = true;
    	struct input_event ev;
        int size = sizeof(ev);

    	while(running) {
    			// blocking event reader
    	    	read(this->fd, &ev, size);
    	    	LOG4CPLUS_DEBUG(logdev, "code=" << ev.code <<
    	    							"type=" <<  ev.type <<
    									"value="<< ev.value <<
    									"s=" << ev.time.tv_sec <<
    									"u=" << ev.time.tv_usec);

    	    	switch(ev.type){
				case EV_KEY:
					this->key.load_event(ev);
					break;
				case EV_SYN:
					this->key.validate_event(ev);
					key_counter++;
					if(this->key.isPressEvent())  key_press_obs(this->key);
					else 						  key_release_obs(this->key);
					if(this->key.isLongEvent())   key_long_obs(this->key);
					break;
					}

    	}
    	LOG4CPLUS_DEBUG(logdev, "KeyPanel thread exit\n");
    }

    int fd;
    unsigned int key_counter;

  public:
    KeyPanel(string& dev) {
    	this->event_dev = dev;
    	this->running = false;
    	this->fd = -1;
    	this->key_counter = 0;
    }
    KeyPanel() {
    	this->event_dev = string("");
    	this->running = false;
    	this->fd = -1;
    	this->key_counter = 0;
    }
    ~KeyPanel() {

    }
    void key_attach(std::function<void (KeyButton& k)> f) {
    	reg_release_obs = key_release_obs.registerObserver(f);
    }
    void key_release_attach(std::function<void (KeyButton& k)> f) {
    	reg_release_obs = key_release_obs.registerObserver(f);
    }
    void key_press_attach(std::function<void (KeyButton& k)> f) {
    	reg_press_obs = key_press_obs.registerObserver(f);
    }
    void key_long_attach(std::function<void (KeyButton& k)> f) {
    	reg_long_obs = key_long_obs.registerObserver(f);
    }


    int start() {
    	Logger logdev = Logger::getInstance(LOGDEVICE);

    	if(running) {
    		LOG4CPLUS_ERROR(logdev, "KeyPanel thread already active\n");
    		return(-1);
    	}
    	if(this->event_dev.empty()) {
    		LOG4CPLUS_ERROR(logdev, "event device not set\n");
    		return(-1);
    	}
    	fd = open(this->event_dev.c_str(), O_RDONLY);
    	if(fd < 0) {
    		LOG4CPLUS_ERROR(logdev, (string("unable to open ")+ this->event_dev+string(strerror(errno))).c_str());
    		throw std::runtime_error((string("unable to open ")+ this->event_dev+string(strerror(errno))).c_str());
    	}
    	this->key_thread = std::thread([&] { KeyPanel::key_thread_reader(); });
    	LOG4CPLUS_DEBUG(logdev, "key reader started\n");
    	return(0);
    }
    int stop() {
    	Logger logdev = Logger::getInstance(LOGDEVICE);
    	if(!running) {
    		LOG4CPLUS_WARN(logdev, "KeyPanel thread not active\n");
    		return(1);
    	}
    	this->running = false;
    	this->key_thread.join();
    	close(this->fd);
    	this->fd = -1;
    	return(0);
    }


    unsigned int get_key_counter() { return(key_counter); }


    void set_event_filename(const char *dev) {
    	this->event_dev = dev;
    }
    string& get_event_filename() {return(this->event_dev); }
};

}

#endif /* KEYPANEL_HPP_ */
