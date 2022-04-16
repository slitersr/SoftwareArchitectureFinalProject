#pragma once

#include <iostream>
#include "LicenseClientImplementation.h"

boost::optional<LicenseReserveFailure> LicenseClientImplementation::reserveEntitlement(std::string license, std::string entitlement) {
	std::cout << "[stub] Reserving license from server" << std::endl;
	return boost::none;
}
void LicenseClientImplementation::unreserveEntitlement(std::string license, std::string entitlement) {
	std::cout << "[stub] Unreserving license from server" << std::endl;
}