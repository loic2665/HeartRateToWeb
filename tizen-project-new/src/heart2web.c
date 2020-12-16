#include "heart2web.h"
#include "STRUCTS.h"
#include "PermissionManager.h"

#define EXPANDFILL(x) { \
	   evas_object_size_hint_weight_set(x, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
	   evas_object_size_hint_align_set(x, EVAS_HINT_FILL, EVAS_HINT_FILL); }

/* Begin Utilities */

static char*
concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static void
generate_popup(void *data, char *notice, int error)
{
	uidata_s *ui = data;
	ui->popup = elm_popup_add(ui->win);
	elm_object_text_set(ui->popup, notice);
	dlog_print(DLOG_ERROR, LOG_TAG, "heart2web_key %s code %d", notice, error);
	elm_popup_timeout_set(ui->popup, 2.0);
	evas_object_show(ui->popup);
}

/* End Utilities */

/* Begin Callbacks */

/* Begin Settings UI Generation (Callback) */

static void
get_interval(void *data)
{
	uidata_s *ui = data;
    bool key_exists = false;
    preference_is_existing(interval_key, &key_exists);
    if (!key_exists) {
    	preference_set_int(interval_key, interval_default);
    }
	preference_get_int(interval_key, &ui->ControlData.interval);
}

static void
update_interval_cb(void *data, Evas_Object *obj, void *event_info)
{
	uidata_s *ui = data;
	const char *selection;
	selection = elm_object_text_get(ui->DataToPass.textbox);
    int tmp = atoi(selection);
	if (tmp < 100) tmp = 100;
	else if (tmp > 3600000) tmp = 3600000;
	ui->ControlData.interval = tmp;
	preference_set_int(interval_key, ui->ControlData.interval);
}

static void
get_address(void *data)
{
	uidata_s *ui = data;
    bool key_exists = false;
    preference_is_existing(address_key, &key_exists);
    if (!key_exists) {
    	preference_set_string(address_key, address_default);
    }
	preference_get_string(address_key, &ui->ControlData.address);
}

static void
update_address_cb(void *data, Evas_Object *obj, void *event_info)
{
	uidata_s *ui = data;
	const char *selection = elm_object_text_get(ui->DataToPass.textbox);
	char *myString = strdup(selection);
	ui->ControlData.address = myString;
	preference_set_string(address_key, ui->ControlData.address);
}

static void
get_port(void *data)
{
	uidata_s *ui = data;
    bool key_exists = false;
    preference_is_existing(port_key, &key_exists);
    if (!key_exists) {
    	preference_set_int(port_key, port_default);
    }
	preference_get_int(port_key, &ui->ControlData.port);
}

static void
update_port_cb(void *data, Evas_Object *obj, void *event_info)
{
	uidata_s *ui = data;
	const char *selection;
	selection = elm_object_text_get(ui->DataToPass.textbox);
    int tmp = atoi(selection);
	if (tmp < 0) tmp = port_default;
	else if (tmp > 65535) tmp = port_default;
	ui->ControlData.port = tmp;
	preference_set_int(port_key, ui->ControlData.port);
}

static void
initialize_settings(void *data)
{
	get_interval(data);
	get_address(data);
	get_port(data);
}

static void
prev_btn_clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	Evas_Object *nf = data;
	elm_naviframe_item_pop(nf);
}

static void
btn_settings_interval_cb(void *data, Evas_Object *obj, void *event_info)
{
		uidata_s *ui = data;

		Evas_Object *bg;
		Evas_Object *box;
		Evas_Object *label;

		bg = elm_bg_add(ui->naviframe);

		elm_bg_color_set(bg, 0, 0, 0);

		/* Add a box and set it as the background content */
		box = elm_box_add(bg);
		//evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
		elm_box_horizontal_set(box, EINA_FALSE);
		elm_box_padding_set(box, 1, 5 * elm_config_scale_get());
		elm_object_content_set(bg, box);

		Evas_Object *tab = elm_table_add(bg);
		evas_object_size_hint_weight_set(tab, EVAS_HINT_EXPAND,
		EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(tab, EVAS_HINT_FILL, 0.4);
		evas_object_show(tab);
		elm_box_pack_end(box, tab);

		ui->intervalInput = elm_entry_add(tab);
		elm_entry_single_line_set(ui->intervalInput, EINA_TRUE);
		static Elm_Entry_Filter_Limit_Size interval_limit_size = { .max_char_count = 7,
				.max_byte_count = 0 };
		int length = snprintf(NULL, 0, "%d", ui->ControlData.interval);
		char* placeHolder = malloc(length + 1);
		snprintf(placeHolder, length + 1, "%d", ui->ControlData.interval);
		elm_entry_markup_filter_append(ui->intervalInput, elm_entry_filter_limit_size,
				&interval_limit_size);
		elm_object_part_text_set(ui->intervalInput, "elm.guide", placeHolder);
		elm_entry_input_panel_layout_set(ui->intervalInput, ELM_INPUT_PANEL_LAYOUT_NUMBERONLY);
		evas_object_size_hint_min_set(ui->intervalInput, 200, 28);
		evas_object_color_set(ui->intervalInput, 100, 100, 200, 255);
		evas_object_size_hint_weight_set(ui->intervalInput, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_show(ui->intervalInput);
		elm_table_pack(tab, ui->intervalInput, 1, 0, 1, 1);

		label = elm_label_add(box);
		char *labelText = concat("<align=center>", i18n_get_text("time_interval"));
		elm_object_text_set(label, concat(labelText,"</align>"));
		evas_object_size_hint_min_set(label, ELM_SCALE_SIZE(100), ELM_SCALE_SIZE(20));
		elm_label_line_wrap_set(label, ELM_WRAP_WORD);
		evas_object_show(label);
		elm_box_pack_end(box, label);

		elm_naviframe_item_push(ui->naviframe, "Interval", NULL, NULL, bg, "empty");
		//----END

		ui->DataToPass.textbox = ui->intervalInput;

		evas_object_smart_callback_add(ui->intervalInput, "aborted", prev_btn_clicked_cb, ui->naviframe);
		evas_object_smart_callback_add(ui->intervalInput, "activated", update_interval_cb, ui);

		eext_object_event_callback_add(bg, EEXT_CALLBACK_BACK, prev_btn_clicked_cb,
				ui->naviframe);
}

static void
btn_settings_address_cb(void *data, Evas_Object *obj, void *event_info)
{
	uidata_s *ui = data;

	Evas_Object *bg;
	Evas_Object *box;
	Evas_Object *label;

	bg = elm_bg_add(ui->naviframe);

	elm_bg_color_set(bg, 0, 0, 0);

	/* Add a box and set it as the background content */
	box = elm_box_add(bg);
	//evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_horizontal_set(box, EINA_FALSE);
	elm_box_padding_set(box, 1, 5 * elm_config_scale_get());
	elm_object_content_set(bg, box);

	Evas_Object *tab = elm_table_add(bg);
	evas_object_size_hint_weight_set(tab, EVAS_HINT_EXPAND,
	EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(tab, EVAS_HINT_FILL, 0.4);
	evas_object_show(tab);
	elm_box_pack_end(box, tab);

	ui->addressInput = elm_entry_add(tab);
	elm_entry_single_line_set(ui->addressInput, EINA_TRUE);
	static Elm_Entry_Filter_Limit_Size address_limit_size = { .max_char_count = 15,
			.max_byte_count = 0 };
	elm_entry_markup_filter_append(ui->addressInput, elm_entry_filter_limit_size,
			&address_limit_size);
	elm_object_part_text_set(ui->addressInput, "elm.guide", ui->ControlData.address);
	elm_entry_input_panel_layout_set(ui->addressInput, ELM_INPUT_PANEL_LAYOUT_IP);
	evas_object_size_hint_min_set(ui->addressInput, 240, 28);
	evas_object_color_set(ui->addressInput, 100, 100, 200, 255);
	evas_object_size_hint_weight_set(ui->addressInput, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(ui->addressInput);
	elm_table_pack(tab, ui->addressInput, 1, 0, 1, 1);

	label = elm_label_add(box);
	char *labelText = concat("<align=center>", i18n_get_text("ip_address"));
	elm_object_text_set(label, concat(labelText,"</align>"));
	evas_object_size_hint_min_set(label, ELM_SCALE_SIZE(100), ELM_SCALE_SIZE(20));
	elm_label_line_wrap_set(label, ELM_WRAP_WORD);
	evas_object_show(label);
	elm_box_pack_end(box, label);

	elm_naviframe_item_push(ui->naviframe, "Interval", NULL, NULL, bg, "empty");
	//----END

	ui->DataToPass.textbox = ui->addressInput;

	evas_object_smart_callback_add(ui->addressInput, "aborted", prev_btn_clicked_cb, ui->naviframe);
	evas_object_smart_callback_add(ui->addressInput, "activated", update_address_cb, ui);

	eext_object_event_callback_add(bg, EEXT_CALLBACK_BACK, prev_btn_clicked_cb,
			ui->naviframe);
}

static void
btn_settings_port_cb(void *data, Evas_Object *obj, void *event_info)
{
	uidata_s *ui = data;

	Evas_Object *bg;
	Evas_Object *box;
	Evas_Object *label;

	bg = elm_bg_add(ui->naviframe);

	elm_bg_color_set(bg, 0, 0, 0);

	/* Add a box and set it as the background content */
	box = elm_box_add(bg);
	//evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_horizontal_set(box, EINA_FALSE);
	elm_box_padding_set(box, 1, 5 * elm_config_scale_get());
	elm_object_content_set(bg, box);

	Evas_Object *tab = elm_table_add(bg);
	evas_object_size_hint_weight_set(tab, EVAS_HINT_EXPAND,
	EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(tab, EVAS_HINT_FILL, 0.4);
	evas_object_show(tab);
	elm_box_pack_end(box, tab);

	ui->portInput = elm_entry_add(tab);
	elm_entry_single_line_set(ui->portInput, EINA_TRUE);
	static Elm_Entry_Filter_Limit_Size port_limit_size = { .max_char_count = 5,
			.max_byte_count = 0 };
	int length = snprintf(NULL, 0, "%d", ui->ControlData.port);
	char* placeHolder = malloc(length + 1);
	snprintf(placeHolder, length + 1, "%d", ui->ControlData.port);
	elm_entry_markup_filter_append(ui->portInput, elm_entry_filter_limit_size,
			&port_limit_size);
	elm_object_part_text_set(ui->portInput, "elm.guide", placeHolder);
	elm_entry_input_panel_layout_set(ui->portInput, ELM_INPUT_PANEL_LAYOUT_NUMBERONLY);
	evas_object_size_hint_min_set(ui->portInput, 100, 28);
	evas_object_color_set(ui->portInput, 100, 100, 200, 255);
	evas_object_size_hint_weight_set(ui->portInput, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(ui->portInput);
	elm_table_pack(tab, ui->portInput, 1, 0, 1, 1);

	label = elm_label_add(box);
	char *labelText = concat("<align=center>", i18n_get_text("port"));
	elm_object_text_set(label, concat(labelText,"</align>"));
	evas_object_size_hint_min_set(label, ELM_SCALE_SIZE(100), ELM_SCALE_SIZE(20));
	elm_label_line_wrap_set(label, ELM_WRAP_WORD);
	evas_object_show(label);
	elm_box_pack_end(box, label);

	elm_naviframe_item_push(ui->naviframe, "Interval", NULL, NULL, bg, "empty");
	//----END

	ui->DataToPass.textbox = ui->portInput;

	evas_object_smart_callback_add(ui->portInput, "aborted", prev_btn_clicked_cb, ui->naviframe);
	evas_object_smart_callback_add(ui->portInput, "activated", update_port_cb, ui);

	eext_object_event_callback_add(bg, EEXT_CALLBACK_BACK, prev_btn_clicked_cb,
			ui->naviframe);
}

static void
settings_window_create_cb(void *data, Evas_Object *obj, void *event_info)
{
	uidata_s *ui = data;

	Evas_Object *background;
	background = elm_bg_add(ui->naviframe);
	elm_bg_color_set(background, 9, 9, 36);

	Evas_Object *scroller;

	scroller = elm_scroller_add(background);
	elm_scroller_bounce_set(scroller, EINA_FALSE, EINA_TRUE);
	elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF,
			ELM_SCROLLER_POLICY_AUTO);
	evas_object_show(scroller);

	//----------
	Evas_Object *box;

	box = elm_box_add(scroller);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_padding_set(box, 0, 5 * elm_config_scale_get());
	evas_object_show(box);

	Evas_Object *box1;

	box1 = elm_box_add(scroller);
	elm_box_padding_set(box1, 0, 5 * elm_config_scale_get());
	elm_box_horizontal_set(box1, EINA_TRUE);
	evas_object_show(box1);
	elm_box_pack_end(box, box1);

	Evas_Object *label = elm_label_add(box1);
	char *labelText = concat("<align=center><font_size=25>", i18n_get_text("settings"));
	elm_object_text_set(label, concat(labelText,"</font_size></align>"));
	elm_label_line_wrap_set(label, ELM_WRAP_WORD);
	evas_object_smart_callback_add(label, "clicked", prev_btn_clicked_cb, background);
	evas_object_show(label);
	elm_box_pack_end(box1, label);

	Evas_Object *button = elm_button_add(box);
	elm_object_text_set(button, i18n_get_text("time_interval"));
	evas_object_size_hint_min_set(button, ELM_SCALE_SIZE(180),
			ELM_SCALE_SIZE(40));
	evas_object_smart_callback_add(button, "clicked", btn_settings_interval_cb,
			ui);
	evas_object_show(button);
	elm_box_pack_end(box, button);

	Evas_Object *button1 = elm_button_add(box);
	elm_object_text_set(button1, i18n_get_text("ip_address"));
	evas_object_size_hint_min_set(button1, ELM_SCALE_SIZE(180),
			ELM_SCALE_SIZE(40));
	evas_object_show(button1);
	evas_object_smart_callback_add(button1, "clicked", btn_settings_address_cb, ui);
	elm_box_pack_end(box, button1);

	Evas_Object *button2 = elm_button_add(box);
	elm_object_text_set(button2, i18n_get_text("port"));
	evas_object_size_hint_min_set(button2, ELM_SCALE_SIZE(180),
			ELM_SCALE_SIZE(40));
	evas_object_smart_callback_add(button2, "clicked",
			btn_settings_port_cb, ui);
	evas_object_show(button2);
	elm_box_pack_end(box, button2);

	//----END

	elm_object_content_set(background, box);
	elm_naviframe_item_push(ui->naviframe, NULL, NULL, NULL, background, "empty");

	//elm_object_part_content_set(ad->layout, "elm.swallow.content", ad->nf);

	eext_object_event_callback_add(background, EEXT_CALLBACK_BACK, prev_btn_clicked_cb,
			ui->naviframe);
}

/* End Settings UI Generation (Callback) */

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	uidata_s *ui = data;
	/* Let window go to hide state. */
	elm_win_lower(ui->win);
}


static void
btn_settings_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	uidata_s *ui = data;
	settings_window_create_cb(ui, obj, event_info);
}

static void
end_monitoring_complete(void *data)
{
	int error;
	uidata_s *ui = data;
	elm_object_text_set(ui->ActiveButton,i18n_get_text("inactive"));
	evas_object_show(ui->ActiveButton);
	dlog_print(DLOG_INFO, LOG_TAG, "heart2web_key inactive");
	error = device_power_release_lock(POWER_LOCK_CPU);
	if (error == DEVICE_ERROR_PERMISSION_DENIED){
		app_check_and_request_display_permission(ui);
		error = device_power_request_lock(POWER_LOCK_CPU, 0);
		if (error != 0 ){
			generate_popup(ui, i18n_get_text("cpu_unlock_fail"), error);
		}
	}
	error = sensor_listener_unset_event_cb(ui->ControlData.listener);
	if (error != SENSOR_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "sensor_listener_unset_event_cb error: %d", error);
	}
	error = sensor_listener_stop(ui->ControlData.listener);
	if (error != 0) dlog_print(DLOG_ERROR, LOG_TAG, "heart2web_key sensor_listener_stop error: %d", error);
	error = sensor_destroy_listener(ui->ControlData.listener);
	if (error != 0) dlog_print(DLOG_ERROR, LOG_TAG, "heart2web_key sensor_destroy_listener error: %d", error);
	curl_easy_cleanup(ui->ControlData.curl);
	connection_unset_proxy_address_changed_cb(ui->ControlData.connection);
	connection_destroy(ui->ControlData.connection);
}

static void
stop_monitoring(int position, int error, void *data)
{
	dlog_print(DLOG_ERROR, LOG_TAG, "heart2web_key position: %d error: %d", position, error);
	uidata_s *ui = data;
	end_monitoring_complete(ui);
}

/* Begin Post Request */

static void
_sensor_accuracy_changed_cb(sensor_h sensor, unsigned long long timestamp,
                                 sensor_data_accuracy_e accuracy, void *data)
{
    dlog_print(DLOG_DEBUG, LOG_TAG, "Sensor accuracy change callback invoked");
}

static void
on_sensor_event(sensor_h sensor, sensor_event_s *event, void *data){
    // Select a specific sensor with a sensor handle
	uidata_s *ui = data;
    sensor_type_e type;
    sensor_get_type(sensor, &type);

    switch (type) {
    case SENSOR_HRM:
    	ui->ControlData.heartRate = (int) event->values[0];
    	dlog_print(DLOG_INFO, LOG_TAG, "heart2web_key event->values[0] %d" , (int)event->values[0]);
    	break;
    default:
        dlog_print(DLOG_ERROR, LOG_TAG, "heart2web_key Not an HRM event");
    }
}

static Eina_Bool
resolve_app_status(void *data)
{
	uidata_s *ui = data;

	/* Build string output */
	char outPut[10];
	sprintf(outPut, "%s=%d", "rate", ui->ControlData.heartRate);
	dlog_print(DLOG_INFO, LOG_TAG, "heart2web_key string built %s", outPut);

	/* Build URL */
	dlog_print(DLOG_INFO, LOG_TAG, "heart2web_key connection building URL");
	char *uri = malloc(32);
	// 7,15,1,5
	sprintf(uri, "http://%s:%d", ui->ControlData.address, ui->ControlData.port);
	dlog_print(DLOG_INFO, LOG_TAG, "heart2web_key connection: %s", uri);

	CURLcode curl_err;
	if(ui->ControlData.curl) {
	    /* First set the URL that is about to receive our POST. This URL can
	       just as well be a https:// URL if that is what should receive the
	       data. */
	    curl_easy_setopt(ui->ControlData.curl, CURLOPT_URL, uri);
	    /* Now specify the POST data */
	    curl_easy_setopt(ui->ControlData.curl, CURLOPT_POSTFIELDS, outPut);

	    /* Perform the request, res will get the return code */
	    curl_err = curl_easy_perform(ui->ControlData.curl);
	    /* Check for errors */
	    if(curl_err != CURLE_OK) {
	    	dlog_print(DLOG_WARN, LOG_TAG, "heart2web_curl_error: %d", curl_err);
	    }
	}

	if (ui->ControlData.active){
		return ECORE_CALLBACK_RENEW;
	} else {
		end_monitoring_complete(ui);
		return ECORE_CALLBACK_CANCEL;
	}
}

/* End Post Request */

static void
btn_active_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
    uidata_s *ui = data;
	int error = 0;
	ui->popup = elm_popup_add(ui->ActiveButton);
	if(ui->ControlData.active){
		ui->ControlData.active = false;
		end_monitoring_complete(ui);
		return;
	} else {
		error = device_power_request_lock(POWER_LOCK_CPU, 0);
		if (error != DEVICE_ERROR_NONE){
			app_check_and_request_display_permission(ui);
			stop_monitoring(0, error, ui);
			return;
		}
		sensor_type_e type = SENSOR_HRM;
		sensor_h sensor;

		bool supported;
		int error = sensor_is_supported(type, &supported);
	    if (error != SENSOR_ERROR_NONE) {
	    	stop_monitoring(1, error, ui);
		    return;
	    }
		error = sensor_get_default_sensor(type, &sensor);
		if (error != DEVICE_ERROR_NONE) {
			app_check_and_request_healthinfo_permission(ui);
			stop_monitoring(3, error, ui);
			return;
		}
		error = sensor_create_listener(sensor, &ui->ControlData.listener);
		if (error != SENSOR_ERROR_NONE) {
		    dlog_print(DLOG_ERROR, LOG_TAG, "heart2web_key sensor_create_listener error: %d",error);
		    stop_monitoring(4, error, ui);
		    return;
		}
		int min_interval = 0;
		double interval_seconds = 0.5;
		error = sensor_get_min_interval(sensor, &min_interval);
		if (ui->ControlData.interval < min_interval) ui->ControlData.interval = min_interval;
		interval_seconds = ui->ControlData.interval / 1000; // get seconds version of ms
		if (interval_seconds < 0.01) interval_seconds = 0.01; // no less than 100 ms or 0.1 seconds
		if (error != SENSOR_ERROR_NONE) {
			dlog_print(DLOG_ERROR, LOG_TAG, "heart2web_key sensor_get_min_interval: %d", error);
		} else {
			dlog_print(DLOG_INFO, LOG_TAG, "heart2web_key sensor_get_min_interval: min interval %f", interval_seconds);
		}
		error = sensor_listener_set_event_cb(ui->ControlData.listener, ui->ControlData.interval, on_sensor_event, ui);
		if (error != SENSOR_ERROR_NONE) {
		    dlog_print(DLOG_ERROR, LOG_TAG, "heart2web_key sensor_listener_set_event_cb error: %d", error);
		    stop_monitoring(5, error, ui);
		    return;
		}
	    error = sensor_listener_set_accuracy_cb(ui->ControlData.listener, _sensor_accuracy_changed_cb, ui);
	    if (error != SENSOR_ERROR_NONE) {
	        dlog_print(DLOG_ERROR, LOG_TAG, "sensor_listener_set_accuracy_cb error: %d", error);
		    stop_monitoring(6, error, ui);
	        return;
	    }
	    dlog_print(DLOG_DEBUG, LOG_TAG, "sensor_listener_set_accuracy_cb");
	    error = sensor_listener_set_interval(ui->ControlData.listener, ui->ControlData.interval);
	    if (error != SENSOR_ERROR_NONE) {
	        dlog_print(DLOG_ERROR, LOG_TAG, "sensor_listener_set_interval error: %d", error);
	        stop_monitoring(7, error, ui);
	        return;
	    }
	    dlog_print(DLOG_DEBUG, LOG_TAG, "sensor_listener_set_intervals");
	    error = sensor_listener_set_option(ui->ControlData.listener, SENSOR_OPTION_ALWAYS_ON);
	    if (error != SENSOR_ERROR_NONE) {
	        dlog_print(DLOG_ERROR, LOG_TAG, "sensor_listener_set_option error: %d", error);
	        stop_monitoring(8, error, ui);
	        return;
	    }
	    dlog_print(DLOG_DEBUG, LOG_TAG, "sensor_listener_set_option");

		ui->ControlData.curl = curl_easy_init();
		int conn_err;
		conn_err = connection_create(&ui->ControlData.connection);
		if (conn_err != CONNECTION_ERROR_NONE) {
		    /* Error handling */
		    dlog_print(DLOG_ERROR, LOG_TAG, "connection_create error: %d", error);
		    stop_monitoring(9, error, ui);
		    return;
		}

		ui->ControlData.active = true;
		elm_object_text_set(ui->ActiveButton,i18n_get_text("active"));
		evas_object_show(ui->ActiveButton);
		dlog_print(DLOG_INFO, LOG_TAG, "heart2web_key active");
		error = sensor_listener_start(ui->ControlData.listener);
	    if (error != SENSOR_ERROR_NONE) {
	        dlog_print(DLOG_ERROR, LOG_TAG, "sensor_listener_start error: %d", error);
	        stop_monitoring(10, error, ui);
	        return;
	    }
		ecore_timer_add(interval_seconds, resolve_app_status, ui);
	}
}

/* End Callbacks */

/* Begin UI Generation */

static void
layout_create(uidata_s *ui)
{
	ui->layout = elm_layout_add(ui->win);
	evas_object_size_hint_weight_set(ui->layout, EVAS_HINT_EXPAND,
	EVAS_HINT_EXPAND);
	elm_layout_theme_set(ui->layout, "layout", "application", "default");
	elm_win_resize_object_add(ui->win, ui->layout);
	evas_object_show(ui->layout);
}

static void
background_create(uidata_s *ui)
{
	ui->background = elm_bg_add(ui->naviframe);
	elm_bg_color_set(ui->background, 9, 9, 32);
	eext_object_event_callback_add(ui->background, EEXT_CALLBACK_BACK, win_back_cb, ui);
}

static void
box_create(uidata_s *ui)
{
	ui->box = elm_box_add(ui->background);
	EXPANDFILL(ui->box);
	evas_object_show(ui->box);
}

static void
label_create(uidata_s *ui)
{
	ui->label = elm_label_add(ui->box);
	elm_object_text_set(ui->label,
			"<align=center> <font_size=20>&lt;3</font_size></align>");
	elm_label_line_wrap_set(ui->label, ELM_WRAP_WORD);
	evas_object_show(ui->label);
	elm_box_pack_end(ui->box, ui->label);
}

static void
tab_create(uidata_s *ui)
{
	ui->tab = elm_table_add(ui->background);
	EXPANDFILL(ui->tab);
	evas_object_show(ui->tab);
 	elm_box_pack_end(ui->box, ui->tab);
}

static void
spacer_create(uidata_s *ui, int position){
	Evas_Object *top_spacer = evas_object_rectangle_add(ui->tab);
	evas_object_size_hint_min_set(top_spacer, 0, 10);
	evas_object_color_set(top_spacer, 0, 0, 0, 0);
	elm_table_pack(ui->tab, top_spacer, 0, position, 1, 1);
}

static void
setting_button_create(uidata_s *ui)
{
	ui->SettingsButton = elm_button_add(ui->box);
	elm_object_text_set(ui->SettingsButton, i18n_get_text("settings"));
	evas_object_size_hint_min_set(ui->SettingsButton, ELM_SCALE_SIZE(180),
			ELM_SCALE_SIZE(40));
	evas_object_smart_callback_add(ui->SettingsButton, "clicked", btn_settings_clicked_cb, ui);
	evas_object_show(ui->SettingsButton);
	elm_table_pack(ui->tab, ui->SettingsButton, 0, 0, 1, 1);
}

static void
active_button_create(uidata_s *ui)
{
	ui->ActiveButton = elm_button_add(ui->box);
	if (ui->ControlData.active) {
		elm_object_text_set(ui->ActiveButton,i18n_get_text("active"));
	} else {
		elm_object_text_set(ui->ActiveButton,i18n_get_text("inactive"));
	}
	evas_object_size_hint_min_set(ui->ActiveButton, ELM_SCALE_SIZE(180),
			ELM_SCALE_SIZE(40));
	evas_object_show(ui->ActiveButton);
	evas_object_smart_callback_add(ui->ActiveButton, "clicked", btn_active_clicked_cb, ui);
	elm_table_pack(ui->tab, ui->ActiveButton, 0, 2, 1, 1);
}

static void
create_base_gui(uidata_s *ui)
{
	initialize_settings(ui);
	/* Window */
	/* Create and initialize elm_win.
	   elm_win is mandatory to manipulate window. */
	ui->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_autodel_set(ui->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ui->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ui->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ui->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ui->win, EEXT_CALLBACK_BACK, win_back_cb, ui);

	elm_win_indicator_mode_set(ui->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ui->win, ELM_WIN_INDICATOR_OPAQUE);

	/* Begin Base Container Creation */

	layout_create(ui);

	elm_object_content_set(ui->win, ui->layout);
	evas_object_show(ui->win);

	/* Show window after base gui is set up */
	evas_object_show(ui->win);

	ui->naviframe = elm_naviframe_add(ui->layout);

	background_create(ui);
	box_create(ui);
	label_create(ui);
	tab_create(ui);

	/* End Base Container Creation */

	/* Begin Display Buttons */

	active_button_create(ui);
	spacer_create(ui, 1);
	setting_button_create(ui);
	spacer_create(ui, 2);

	/* End Display Buttons */

	elm_object_content_set(ui->background, ui->box);
	elm_naviframe_item_push(ui->naviframe, NULL, NULL, NULL, ui->background, "empty");

	elm_object_part_content_set(ui->layout, "elm.swallow.content", ui->naviframe);
}

/* End UI Generation */

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */

	uidata_s *ui = data;

	create_base_gui(ui);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
	end_monitoring_complete(data);
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	uidata_s ui = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ui);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ui);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ui);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ui);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ui);

	ret = ui_app_main(argc, argv, &event_callback, &ui);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "heart2web_key app_main() is failed. err = %d", ret);
	}

	return ret;
}
