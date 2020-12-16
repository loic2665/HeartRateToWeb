#ifndef __heart2web_H__
#define __heart2web_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>

#include <app_i18n.h>
#include <app_preference.h>
#include <privacy_privilege_manager.h>
#include <device/power.h>
#include <sensor.h>

#include <curl/curl.h>
#include <net_connection.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-glib/json-glib.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "heart2web"

#if !defined(PACKAGE)
#define PACKAGE "com.bitstobreath.heart2web"
#endif

#endif /* __heart2web_H__ */
