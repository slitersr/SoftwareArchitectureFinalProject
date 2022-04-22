#pragma once

#include <vector>
#include "LicensingDLL.h"
#include "License.h"

class dll LicenseLoader {
public:
	virtual ~LicenseLoader() {};
	/**
	* Reads all licenses and their associated signatures from the "licenses" directory.
	* Loads each license if valid and ensure that the signatue is correct. Logs to the 
	* console if there is an issue. Returns a vector of only the successfully loaded
	* and verified licenses.
	*/
	virtual std::vector<License> loadLicenses() = 0;

	/**
	* Validate the signaute of an individual license. Returns true if the embedded
	* signature is valid, and false otherwise.
	*/
	virtual bool validateSignature(License license) = 0;
};