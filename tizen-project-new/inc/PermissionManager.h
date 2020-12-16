/*
 * PermissionManager.h
 *
 *  Created on: Dec 15, 2020
 *      Author: Austin Hogan
 */

#ifndef PERMISSIONMANAGER_H_
#define PERMISSIONMANAGER_H_

void app_check_and_request_display_permission(void *data);

void app_check_and_request_healthinfo_permission(void *data);

void app_check_and_request_internet_permission(void *data);

void app_check_and_request_network_get_permission(void *data);

#endif /* PERMISSIONMANAGER_H_ */
