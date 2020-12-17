/*
 * PermissionManager.c
 *
 *  Created on: Dec 15, 2020
 *      Author: Austin Hogan
 */

#include "PermissionManager.h"
#include "STRUCTS.h"


void app_request_response_cb(ppm_call_cause_e cause,
		ppm_request_result_e result, const char *privilege, void *user_data) {

	//uidata_s *ui = user_data;

	if (cause == PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ERROR) {
		/* Log and handle errors */

		return;
	}

	switch (result) {
	case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER:
		/* Update UI and start accessing protected functionality */

		break;
	case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_FOREVER:
		/* Show a message and terminate the application */

		break;
	case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE:
		/* Show a message with explanation */

		break;
	}
}


void app_check_and_request_display_permission(void *data) {
	uidata_s *ui = data;

	ppm_check_result_e result;
	const char *privilege = "http://tizen.org/privilege/display";

	int ret = ppm_check_permission(privilege, &result);
	if (ret == PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE) {
		switch (result) {
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW:
			//Update UI and start accessing protected functionality

			break;
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY:
			//Show a message and terminate the application

			break;
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK:
			ret = ppm_request_permission(privilege, app_request_response_cb,
					ui);
			//Log and handle errors

			break;
		}
	} else {
		//ret != PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE
		//Handle errors

	}

}

void app_check_and_request_healthinfo_permission(void *data) {
	uidata_s *ui = data;

	ppm_check_result_e result;
	const char *privilege = "http://tizen.org/privilege/healthinfo";

	int ret = ppm_check_permission(privilege, &result);
	if (ret == PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE) {
		switch (result) {
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW:
			//Update UI and start accessing protected functionality

			break;
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY:
			//Show a message and terminate the application

			break;
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK:
			ret = ppm_request_permission(privilege, app_request_response_cb,
					ui);
			//Log and handle errors

			break;
		}
	} else {
		//ret != PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE
		//Handle errors

	}

}

void app_check_and_request_internet_permission(void *data) {
	uidata_s *ui = data;

	ppm_check_result_e result;
	const char *privilege = "http://tizen.org/privilege/internet";

	int ret = ppm_check_permission(privilege, &result);
	if (ret == PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE) {
		switch (result) {
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW:
			//Update UI and start accessing protected functionality

			break;
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY:
			//Show a message and terminate the application

			break;
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK:
			ret = ppm_request_permission(privilege, app_request_response_cb, ui);
			//Log and handle errors

			break;
		}
	} else {
		//ret != PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE
		//Handle errors

	}

}

void app_check_and_request_network_get_permission(void *data) {
	uidata_s *ui = data;

	ppm_check_result_e result;
	const char *privilege = "http://tizen.org/privilege/network.get";

	int ret = ppm_check_permission(privilege, &result);
	if (ret == PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE) {
		switch (result) {
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW:
			//Update UI and start accessing protected functionality

			break;
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY:
			//Show a message and terminate the application

			break;
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK:
			ret = ppm_request_permission(privilege, app_request_response_cb, ui);
			//Log and handle errors

			break;
		}
	} else {
		//ret != PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE
		//Handle errors

	}

}

