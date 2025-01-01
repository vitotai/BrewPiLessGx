#include <Arduino.h>
#include <WiFi.h>
#include "hap.h"
#include "hap_apple_servs.h"
#include "hap_apple_chars.h"

#include "TemperatureFormats.h"
#include "TempControl.h"
#include "BrewPiProxy.h"
#include "ExternalData.h"
#include "BPLSettings.h"
#if SupportPressureTransducer
#include "PressureMonitor.h"
#endif
#if EnableHumidityControlSupport
#include "HumidityControl.h"
#endif
#include "GravityTracker.h"
#include "ExternalData.h"
#include "BrewKeeper.h"


extern BrewPiProxy brewPi;
#include "Homekit.h"

#define ValidGravity(v) ((v) > 0.5 && (v)<1.5)
#define ValidPlato(v) ((v) > 0 && (v) > 100)

#define TEMPERATURE_REPORT_PERIOD 60000
#define GRAVITY_REPORT_MAX_COUNT 10

#define GRAVITY_CHANGE_PERIOD_1 6
#define GRAVITY_CHANGE_PERIOD_2 24

static uint8_t _gravity_report_count;
static uint8_t _homekitStatus=HomekitStatus_Inactive;
static uint32_t _last_temp_update;
static hap_char_t* _current_temperature_char;
static hap_char_t* _gravity_char;
static hap_char_t* _gravitychange_detected_char;

static hap_char_t* _target_temperature_char;


static float getGravityReading(void){
	float value;
	if(theSettings.GravityConfig()->usePlato){
		value =  externalData.plato();
		if(!ValidPlato(value)) return 0;
		value = value *10;
	}else{
		value = externalData.gravity();
		if(!ValidGravity(value)) return 0;
		value = value * 1000;
	}
	return value;
}

static int sensor_identify(hap_acc_t *ha)
{
    return HAP_SUCCESS;
}
static int beertemp_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv)
{
	const char* char_uuid=hap_char_get_type_uuid(hc);

    if (!strcmp(char_uuid, HAP_CHAR_UUID_CURRENT_TEMPERATURE)) {
        hap_val_t new_val;
        new_val.f = brewPi.getBeerTemp();
		if(IS_FLOAT_TEMP_VALID(new_val.f)){
        	hap_char_update_val(hc, &new_val);
    	    *status_code = HAP_STATUS_SUCCESS;
	        return HAP_SUCCESS;
		}else{
			*status_code = HAP_STATUS_VAL_INVALID;	
		}
    }else if (!strcmp(char_uuid, HAP_CHAR_UUID_TARGET_TEMPERATURE)) {
        hap_val_t new_val;
		new_val.f = brewPi.getBeerSet();
		if(IS_FLOAT_TEMP_VALID(new_val.f)){
			hap_char_update_val(hc, &new_val);
			*status_code = HAP_STATUS_SUCCESS;
			return HAP_SUCCESS;
		}else{
			*status_code = HAP_STATUS_VAL_INVALID;	
		}
    }else if (!strcmp(char_uuid, HAP_CHAR_UUID_CURRENT_HEATING_COOLING_STATE)) {
        hap_val_t new_val;

        uint8_t state = brewPi.getState();

		if(state==COOLING || state==COOLING_MIN_TIME){
			new_val.i = 2; // cooling
		}else if (state==HEATING || state==HEATING_MIN_TIME){
			new_val.i = 1; //heating
		}else{
			new_val.i = 0; // off
		}

        hap_char_update_val(hc, &new_val);
    	*status_code = HAP_STATUS_SUCCESS;
	    return HAP_SUCCESS;
    }else if (!strcmp(char_uuid, HAP_CHAR_UUID_TARGET_HEATING_COOLING_STATE)) {
        hap_val_t new_val;
		// 0: off
		// 1, heat, 
		// 2, cool
		// 3. auot. we will allow only OFF or Auto
		if(brewPi.getMode() == BrewPiModeOff){
			new_val.i = 0;
		}else if(brewPi.getMode() == BrewPiModeBeerProfile){
			new_val.i = 3;
		}else{
			const hap_val_t* old_val=hap_char_get_val(hc);
			new_val.i = old_val->i;
		}
        hap_char_update_val(hc, &new_val);
    	*status_code = HAP_STATUS_SUCCESS;
	    return HAP_SUCCESS;
    }else if (!strcmp(char_uuid, HAP_CHAR_UUID_TEMPERATURE_DISPLAY_UNITS)) {
        hap_val_t new_val;
		// 0: Celsius
		// 1, Fahrenheit
		new_val.i = (brewPi.getUnit() == 'C')? 0:1;
        hap_char_update_val(hc, &new_val);
    	*status_code = HAP_STATUS_SUCCESS;
	    return HAP_SUCCESS;
    }else {
        *status_code = HAP_STATUS_RES_ABSENT;
    }
    return HAP_FAIL;
}
static int beertemp_write(hap_write_data_t write_data[], int count,
        void *serv_priv, void *write_priv){
    int i, ret = HAP_SUCCESS;
    hap_write_data_t *write;
    for (i = 0; i < count; i++) {
        write = &write_data[i];
		if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_TARGET_TEMPERATURE)) {
			DBG_PRINTF("Homekit:setTemp:%d\n",(int)write->val.f);
            if ( write->val.f > brewPi.getMaxSetTemp() || write->val.f < brewPi.getMinSetTemp()) {
                *(write->status) = HAP_STATUS_VAL_INVALID;
                ret = HAP_FAIL;
            } else {
				DBG_PRINTF("Homekit:setTemp in mode %c\n",brewPi.getMode());

				if(brewPi.getMode() == BrewPiModeBeerConstant){
					brewPi.setBeerSet(write->val.f);
    	            hap_char_update_val(write->hc, &(write->val));
        	        *(write->status) = HAP_STATUS_SUCCESS;
				}
            }
		}else if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_TARGET_HEATING_COOLING_STATE)) {
			// 0: off   off
			// 1, heat, beer constant
			// 2, cool, beer constant
			// 3. auot. beer profile 
			// will accept off or ON only, everything other than off will considered as Beer Constant.
			hap_val_t new_val;
			if(write->val.i == 0){
				brewPi.setMode(BrewPiModeOff);
			}else if(write->val.i == 3){
				if(brewPi.getMode() != BrewPiModeBeerProfile){
					brewKeeper.setModeFromRemote(BrewPiModeBeerProfile);
				}
			}else{
				brewPi.setMode(BrewPiModeBeerConstant);
			}
			DBG_PRINTF("Homekit:setMode to %c, req:%d\n",brewPi.getMode(),write->val.i);
			new_val.i = write->val.i;
            hap_char_update_val(write->hc, &new_val);
            *(write->status) = HAP_STATUS_SUCCESS;
		}else if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_TEMPERATURE_DISPLAY_UNITS)) {
			// suprisingly, it is writable.
			//
	        hap_val_t new_val;
			// 0: Celsius
			// 1, Fahrenheit
			new_val.i = (brewPi.getUnit() == 'C')? 0:1;
			hap_char_update_val(write->hc, &new_val);
			*(write->status) = HAP_STATUS_SUCCESS;
        } else {
            *(write->status) = HAP_STATUS_RES_ABSENT;
        }
    }
    return ret;
}

static int gravity_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv)
{
	const char* char_uuid=hap_char_get_type_uuid(hc);

    if (!strcmp(char_uuid, HAP_CHAR_UUID_CURRENT_AMBIENT_LIGHT_LEVEL)) {
			hap_val_t new_val;
			new_val.f = getGravityReading();
			hap_char_update_val(hc, &new_val);
			*status_code = HAP_STATUS_SUCCESS;
			return HAP_SUCCESS;
    } else {
        *status_code = HAP_STATUS_RES_ABSENT;	
    }
    return HAP_FAIL;
}
static int gravitychange_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv)
{
	const char* char_uuid=hap_char_get_type_uuid(hc);
    if (!strcmp(char_uuid, HAP_CHAR_UUID_CARBON_DIOXIDE_DETECTED)) {
		int value=gravityTracker.gravityDecreasedIn(GRAVITY_CHANGE_PERIOD_1); // 6hours
	   	hap_val_t new_val;
		new_val.i = ( value == InvalidGravityChangeValue)? 0:1;
		hap_char_update_val(hc, &new_val);
		*status_code = HAP_STATUS_SUCCESS;
    }else if (!strcmp(char_uuid, HAP_CHAR_UUID_CARBON_DIOXIDE_LEVEL)) {
	    int value=gravityTracker.gravityDecreasedIn(GRAVITY_CHANGE_PERIOD_1); // 6hours
    	if( value == InvalidGravityChangeValue){
//			*status_code = HAP_STATUS_VAL_INVALID;
			value =0;
		}//else{
			hap_val_t new_val;
			new_val.f =(float) value;
			hap_char_update_val(hc, &new_val);
			*status_code = HAP_STATUS_SUCCESS;
		    return HAP_SUCCESS;
//		}
	}else if (!strcmp(char_uuid, HAP_CHAR_UUID_CARBON_DIOXIDE_PEAK_LEVEL)) {
	    int value=gravityTracker.gravityDecreasedIn(GRAVITY_CHANGE_PERIOD_2); // 24hours
    	if( value == InvalidGravityChangeValue){
//			*status_code = HAP_STATUS_VAL_INVALID;
			value =0;
		}//else{
	        hap_val_t new_val;
			new_val.f = (float) value;
        	hap_char_update_val(hc, &new_val);
    		*status_code = HAP_STATUS_SUCCESS;
		//}
    } else {
        *status_code = HAP_STATUS_RES_ABSENT;
    }
    return HAP_FAIL;
}

static void hap_event_handler(hap_event_t event, void *data){
	switch(event){
		/** A new controller was paired/added/modified.
		 * Associated data is a NULL terminated controller identifier string.
		 */
		case HAP_EVENT_CTRL_PAIRED:
		/** A controller was removed
		 * Associated data is a NULL terminated controller identifier string.
		 */
		case HAP_EVENT_CTRL_UNPAIRED:
			break; // not handled for now

		/** A paired controller connected to the accessory (extablished a pair verified session).
		 * Associated data is a NULL terminated controller identifier string.
		 */
		case HAP_EVENT_CTRL_CONNECTED:
			_homekitStatus = HomekitStatus_Connected;
			break;

		/** A controller disconnected from the accessory. This event is reported before the
		 * actual disconnection, because for cases like pair-remove, the controller information
		 * gets erased before the disconnection, and so the controller id is not available
		 * after disconnection.
		 * Associated data is a NULL terminated controller identifier string.
		 */
		case HAP_EVENT_CTRL_DISCONNECTED:
			_homekitStatus = (hap_get_paired_controller_count()>0)? HomekitStatus_Paired:HomekitStatus_Inactive;
			break;

		/** A Pair Setup attempt has started. Waiting for Setup Code */
		case HAP_EVENT_PAIRING_STARTED:
			_homekitStatus = HomekitStatus_Pairing;
			break;

		/** Pair Setup was aborted because of inactivity or a wrong setup code */
		case HAP_EVENT_PAIRING_ABORTED:
			_homekitStatus = HomekitStatus_Inactive;
			break;

		/** A GET on /accessories was successfully completed */
		case HAP_EVENT_GET_ACC_COMPLETED:
		/** A GET on /characteristics was successfully completed */
		case HAP_EVENT_GET_CHAR_COMPLETED:
		/** A PUT (Set value) on /characteristics was successfully completed.
		 * This event can also mean that notifications were enabled for some
		 * characteristics as the same is also done in PUT /characteristics */
		case HAP_EVENT_SET_CHAR_COMPLETED:
		/* Accessory is about to reboot. Will be triggered for operations like hap_reset_to_factory(),
		* hap_reboot_accessory(), hap_reset_network(), hap_reset_pairings() and hap_reset_homekit_data()
		* just before rebooting. Associated data is a pointer to a string indicating the reboot reason.
		* Reefer the HAP_REBOOT_REASON_* macros for possible values.
		*/
		case HAP_EVENT_ACC_REBOOTING:
		/* Accessory is no more available for pairing. This will be triggered if an accessory is in
		* an unpaired state for more than the time specified in HAP Spec R16.
		*/
		case HAP_EVENT_PAIRING_MODE_TIMED_OUT:
			_homekitStatus = HomekitStatus_Inactive;
			break;
	}
}

void homekit_setup(void)
{
    hap_acc_t *accessory;
    hap_serv_t *service;

    /* Configure HomeKit core to make the Accessory name (and thus the WAC SSID) unique,
     * instead of the default configuration wherein only the WAC SSID is made unique.
     */
    hap_cfg_t hap_cfg;
    hap_get_config(&hap_cfg);
    hap_cfg.unique_param = UNIQUE_NAME;
    hap_set_config(&hap_cfg);

    /* Initialize the HAP core */
    hap_init(HAP_TRANSPORT_WIFI);

    /* Initialise the mandatory parameters for Accessory which will be added as
     * the mandatory services internally
     */
    hap_acc_cfg_t cfg = {
        .name = "BPL",
        .model = "BrewPilessGx",
        .manufacturer = "HomeDiy",
        .serial_num = "001122334455",
        .fw_rev = "0.9.0",
        .hw_rev = NULL,
        .pv = "1.1.0",
        .cid = HAP_CID_THERMOSTAT,
        .identify_routine = sensor_identify,
    };
    /* Create accessory object */
    accessory = hap_acc_create(&cfg);

    /* Add a dummy Product Data */
    uint8_t product_data[] = {'E','S','P','3','2','H','A','P'};
    hap_acc_add_product_data(accessory, product_data, sizeof(product_data));

    /* Add Wi-Fi Transport service required for HAP Spec R16 */
    hap_acc_add_wifi_transport_service(accessory, 0);

    /* Create the Sensor Service. Include the "name" since this is a user visible service  */
	service= hap_serv_thermostat_create(0,3,20,20,(brewPi.getUnit()=='C')? 0:1);

	_current_temperature_char=hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_CURRENT_TEMPERATURE);
	
	_target_temperature_char=hap_serv_get_char_by_uuid(service,HAP_CHAR_UUID_TARGET_TEMPERATURE);
	// over write the limit
	hap_char_float_set_constraints(_target_temperature_char, brewPi.getMinSetTemp(),brewPi.getMaxSetTemp(), 0.1);

    hap_serv_add_char(service, hap_char_name_create("Beer Temperature"));

    /* Set the read callback for the service */
    hap_serv_set_read_cb(service, beertemp_read);
	hap_serv_set_write_cb(service, beertemp_write);

    /* Add the Sensor Service to the Accessory Object */
    hap_acc_add_serv(accessory, service);

	// gravity
	service = hap_serv_light_sensor_create(0.0001);
    hap_serv_add_char(service, hap_char_name_create("Gravity"));
	_gravity_char = hap_serv_get_char_by_uuid(service,HAP_CHAR_UUID_CURRENT_AMBIENT_LIGHT_LEVEL);

    hap_serv_set_read_cb(service, gravity_read);
    hap_acc_add_serv(accessory, service);
	// gravity change 12H or 24H?
	service = hap_serv_carbon_dioxide_sensor_create(1);
    hap_serv_add_char(service, hap_char_name_create("Gravity Change"));
	_gravitychange_detected_char = hap_serv_get_char_by_uuid(service,HAP_CHAR_UUID_CARBON_DIOXIDE_DETECTED);

	hap_serv_add_char(service, hap_char_carbon_dioxide_level_create(32));
	hap_serv_add_char(service, hap_char_carbon_dioxide_peak_level_create(1056));
    hap_serv_set_read_cb(service, gravitychange_read);
    hap_acc_add_serv(accessory, service);

    /* Add the Accessory to the HomeKit Database */
    hap_add_accessory(accessory);

    /* Unique Setup code of the format xxx-xx-xxx. Default: 111-22-333 */
    hap_set_setup_code("111-22-333");
    /* Unique four character Setup Id. Default: ES32 */
    hap_set_setup_id("ES32");

    /* Enable Hardware MFi authentication (applicable only for MFi variant of SDK) */
    hap_enable_mfi_auth(HAP_MFI_AUTH_HW);

	hap_register_event_handler(hap_event_handler);
    /* After all the initializations are done, start the HAP core */
    hap_start();
}

void homekit_reboot(void){
	hap_reboot_accessory();
}


bool homekit_reset_pairing(void){
	return  HAP_SUCCESS==hap_reset_pairings();
}

void homekit_restart_pairing(void){
	hap_pair_setup_re_enable();
}

uint8_t homekit_status(void){
	
	if( _homekitStatus == HomekitStatus_Inactive	
	 	&& hap_get_paired_controller_count() > 0){
			_homekitStatus = HomekitStatus_Paired;
		}

	return _homekitStatus;
}


void homekit_loop(void){
	uint32_t now=millis();
	// periodically update Current temperature
	if( (now - _last_temp_update) > TEMPERATURE_REPORT_PERIOD){
		_last_temp_update = now;

        hap_val_t new_val;
        new_val.f = brewPi.getBeerTemp();
		if(IS_FLOAT_TEMP_VALID(new_val.f)){
        	hap_char_update_val(_current_temperature_char, &new_val);
		}
		
		// report Gravity and Gravity Change

		_gravity_report_count ++;
		if(_gravity_report_count >=GRAVITY_REPORT_MAX_COUNT){
			_gravity_report_count =0;
			new_val.f = getGravityReading();
			hap_char_update_val(_gravity_char, &new_val);
			// Gravity Change
			// it seems to be updated automatically.
			int value=gravityTracker.gravityDecreasedIn(GRAVITY_CHANGE_PERIOD_1); // 6hours
			new_val.i = ( value == InvalidGravityChangeValue)? 0:1;
			hap_char_update_val(_gravitychange_detected_char, &new_val);
		}
	}
/*	updating target temperature seems making no difference. 
	even without notification, the Home app gets latest value
	const hap_val_t* target=hap_char_get_val(_target_temperature_char);
	if(target->f != brewPi.getBeerSet()){
        hap_val_t new_val;
        new_val.f = brewPi.getBeerSet();
		hap_char_update_val(_target_temperature_char, &new_val);
	} */
	// TODO: report Control Mode change
}