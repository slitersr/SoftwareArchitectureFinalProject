#pragma once

#include "LicensingAPI.h"
#include "LicensingAPIImplementation.h"
#include "LicenseClientImplementation.h"
#include "LicenseLoaderImplementation.h"

class Licensing {
public:
	/**
	* Obtain the LicensingAPI singleton.
	*/
	static std::shared_ptr<LicensingAPI> api() {
		static std::shared_ptr<LicensingAPI> pointer{
			new LicensingAPIImplementation(new LicenseClientImplementation(), new LicenseLoaderImplementation())
		};
		return pointer;
	}
};