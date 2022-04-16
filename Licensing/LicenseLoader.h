#pragma once

#include <vector>
#include "LicensingDLL.h"
#include "License.h"

class dll LicenseLoader {
public:
	virtual ~LicenseLoader() {};
	virtual std::vector<License> loadLicenses() = 0;
};