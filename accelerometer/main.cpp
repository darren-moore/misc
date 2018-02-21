

#include <windows.h>
#include <initguid.h>
#include <propkeydef.h>

#include <Sensors.h>
#include <Sensorsapi.h>
#include <Objbase.h>

#include <iostream>
#include <propvarutil.h>
#include <functiondiscoverykeys.h>
#include <assert.h>

// NOTE you must include these libraries: Sensorsapi.lib, Ole32.lib

// This Sensor API guide guided me: https://msdn.microsoft.com/en-us/library/windows/desktop/dd318964(v=vs.85).aspx
int main(void) {
	CoInitialize(nullptr); // NULL if using older VC++

	// We use this variable to check if something goes wrong.
	HRESULT hr = S_OK;

	// Sensor interface pointers
	ISensorManager* pSensorManager = NULL;
	ISensorCollection* pSensorColl = NULL;
	ISensor* pSensor = NULL;
	ISensorDataReport* pReport = NULL;

	// Accelerometer sensor data field variables
	double ex, why, zed = 0;

	// Create the sensor manager.
	hr = CoCreateInstance(CLSID_SensorManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pSensorManager));

	if (hr == HRESULT_FROM_WIN32(ERROR_ACCESS_DISABLED_BY_POLICY)) {
		// Unable to retrieve sensor manager due to 
		// group policy settings. Alert the user.
	}

	// Get the accelerometer.
	// You can find other sensor types and categories here: https://msdn.microsoft.com/en-us/library/windows/desktop/dd318969(v=vs.85).aspx
	hr = pSensorManager->GetSensorsByType(SENSOR_TYPE_ACCELEROMETER_3D, &pSensorColl);
	if (SUCCEEDED(hr)) {
		ULONG ulCount = 0;
		// Verify that the collection contains at least one sensor.
		hr = pSensorColl->GetCount(&ulCount);

		if (SUCCEEDED(hr)) {
			if (ulCount < 1) {
				wprintf_s(L"\nNo sensors of the requested type.\n");
				hr = E_UNEXPECTED;
			}
		}
	}

	if (SUCCEEDED(hr)){
		// Get the first available sensor.
		hr = pSensorColl->GetAt(0, &pSensor);
	}

	// We will poll the sensor many times, sleeping for a second between each poll.
	while (true) {
		if (SUCCEEDED(hr)) {
			// Get the data report.
			hr = pSensor->GetData(&pReport);
		}


		if (SUCCEEDED(hr)) {
			PROPVARIANT var = {};	// We will put data from the sensor in here.

			// Get x coord
			// The sensor constant is from: https://msdn.microsoft.com/en-us/library/windows/desktop/dd318985(v=vs.85).aspx
			hr = pReport->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_X_G, &var);
			if (SUCCEEDED(hr)) {
				if (var.vt == VT_R8) {
					ex = var.dblVal;
					// dblVal means the value is a double.
					// If you try to look at the wrong type like "ulVal", you will get garbage.
					// I'm not sure where to find info on what data type each sensor outputs.
					// Perhaps it's always double?
				}
			}
			PropVariantClear(&var);		// Clear the way for the next bit of data.

			// Get y coord
			hr = pReport->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Y_G, &var);
			if (SUCCEEDED(hr)) {
				if (var.vt == VT_R8) {
					why = var.dblVal;
				}
			}
			PropVariantClear(&var);

			// Get z coord
			hr = pReport->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Z_G, &var);
			if (SUCCEEDED(hr)) {
				if (var.vt == VT_R8) {
					zed = var.dblVal;
				}
			}
			PropVariantClear(&var);

			if (SUCCEEDED(hr)) {
				// Print the data.
				wprintf_s(L"(%f,%f,%f)\n", ex, why, zed);
			}
		}

		Sleep(1000);
	}
	return 0;
}
