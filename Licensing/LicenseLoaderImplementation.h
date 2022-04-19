#pragma once

#include "LicensingDLL.h"
#include "LicenseLoader.h"

class dll LicenseLoaderImplementation : public LicenseLoader {
public:
	virtual ~LicenseLoaderImplementation() {};
	virtual std::vector<License> loadLicenses();
	virtual bool validateSignature(License license);
};