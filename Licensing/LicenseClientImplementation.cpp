#pragma once

#include <iostream>
#include "LicenseClientImplementation.h"

boost::optional<LicenseReserveFailure> LicenseClientImplementation::reserveEntitlement(std::string license, std::string entitlement) {
	std::cout << "[License Client] Reserving license from server" << std::endl;
	return boost::none;
}
void LicenseClientImplementation::unreserveEntitlement(std::string license, std::string entitlement) {
	std::cout << "[License Client] Unreserving license from server" << std::endl;
}