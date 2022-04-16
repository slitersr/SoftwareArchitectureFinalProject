#include "LicensingAPIImplementation.h"

#include <iostream>

void LicensingAPIImplementation::initialize() {
	std::cout << "[stub] API initialize" << std::endl;
}

boost::optional<LicenseReserveFailure> LicensingAPIImplementation::reserve(std::string context, std::string entitlement) {
	std::cout << "[stub] API reserve" << std::endl;
	return boost::none;
}

void LicensingAPIImplementation::unreserve(std::string context, std::string entitlement) {
	std::cout << "[stub] API unreserve" << std::endl;
}

void LicensingAPIImplementation::check(std::string context, std::string entitlement) {
	std::cout << "[stub] API check" << std::endl;
}

void LicensingAPIImplementation::shutdown() {
	std::cout << "[stub] API shutdown" << std::endl;
}