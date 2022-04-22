#include "Licensing.h"

std::shared_ptr<LicensingAPI> Licensing::api() {
	std::shared_ptr<LicensingAPI> pointer{
		new LicensingAPIImplementation(new LicenseClientImplementation(), new LicenseLoaderImplementation())
	};
	return pointer;
}