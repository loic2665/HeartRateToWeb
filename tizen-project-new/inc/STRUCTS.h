/*
 * STRUCTS.h
 *
 *  Created on: Dec 15, 2020
 *      Author: Austin Hogan
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_
// preference keys
#define address_key "ip_address"
#define port_key "port"
#define interval_key "time_interval"
#define active_key "active"

// default values
#define address_default "192.168.1.139"
#define port_default 6547
#define interval_default 3000
#define active_default 1

#include "heart2web.h"

#endif /* STRUCTS_H_ */

typedef struct data_to_pass {

	Evas_Object *textbox;

} data_to_pass;

typedef struct control_data {
	char *address;
	int port;
	int interval;
	bool active;
	int heartRate;
	sensor_listener_h listener;
	CURL *curl;
	connection_h connection;
} control_data;

typedef struct 	 {
    Evas_Object *win;
    Evas_Object *conform;
    Evas_Object *label;
    Evas_Object *layout;
    Evas_Object *box;
    Evas_Object *tab;
    Evas_Object *background;
	Evas_Object *naviframe;
	Evas_Object *SettingsButton;
	Evas_Object *ActiveButton;
	Evas_Object *intervalInput;
	Evas_Object *addressInput;
	Evas_Object *portInput;
	Evas_Object *popup;
	control_data ControlData;
	data_to_pass DataToPass;
} uidata_s;
