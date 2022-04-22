#include <iostream>

#include "LicensingAPIImplementation.h"
#include "LicenseLoaderImplementation.h"

void LicensingAPIImplementation::initialize() {
	licenses = loader->loadLicenses();
	reservations.clear();
	running = true;
}

LicenseReserveFailure fail(std::string context, std::string entitlement, std::string message, bool allowContinue) {
	LicenseReserveFailure result;
	result.setContext(context);
	result.setEntitlement(entitlement);
	result.setAllowContinue(allowContinue);
	result.setMessage(message);
	return result;
}

std::vector<License>& LicensingAPIImplementation::reservationsByEntitlement(std::string entitlement) {
	if (reservations.find(entitlement) == reservations.end()) {
		reservations[entitlement] = std::vector<License>();
	}
	return reservations[entitlement];
}

boost::optional<LicenseReserveFailure> LicensingAPIImplementation::reserve(std::string context, std::string entitlement) {
	if (!running) return fail(context, entitlement, "The licensing service is not running.", false);

	std::vector<License>& current = reservationsByEntitlement(entitlement);
	// Check if a suitable license is already reserved
	for (License& reserved : current) {
		// Only reuse a license if it supports the requested context
		for (std::string& allowed : reserved.getContexts()) {
			if (context == allowed) {
				// License can be reused, increment its countex tounter
				reserved.setCurrentContexts(reserved.getCurrentContexts() + 1);
				return boost::none;
			}
		}
	}

	// No suitable license already reserved. Try to reserve a new one.
	for (License& license : licenses) {
		// Correct entitlement and context capabilities
		bool foundEntitlement = false, foundContext = false;
		for (std::string& allowed : license.getContexts()) {
			if (context == allowed) {
				foundContext = true;
			}
		}
		if (!foundContext) continue;
		for (std::string& allowed : license.getEntitlements()) {
			if (entitlement == allowed) {
				foundEntitlement = true;
			}
		}
		if (!foundEntitlement) continue;
		// Check not already reserved for this entitlement.
		for (License& reserved : current) {
			if (reserved.getRaw() == license.getRaw()) {
				// Already reserved.
				continue;
			}
		}
		// Found appropriate license. Reserve it.
		auto result = client->reserveEntitlement(license.getId(), entitlement);
		if (!result.is_initialized()) {
			// Only add to the reserved list if the server actually reserved the license.
			License copy = license;
			copy.setCurrentContexts(copy.getCurrentContexts() + 1);
			current.push_back(copy);
		}
		return result;
	}

	// No license was found.
	return fail(context, entitlement, "You don't have a license that is able to reserved the " + entitlement + " entitlement in the " + context + " context. Please install an appropriate license.", false);
}

bool LicensingAPIImplementation::unreserve(std::string context, std::string entitlement) {
	std::vector<License>& current = reservationsByEntitlement(entitlement);

	for (int i = 0; i < current.size(); i++) {
		// Find a checked out license that has the target entitlement and context
		License& license = current[i];
		bool foundEntitlement = false, foundContext = false;
		for (std::string& allowed : license.getContexts()) {
			if (context == allowed) {
				foundContext = true;
			}
		}
		if (!foundContext) continue;
		for (std::string& allowed : license.getEntitlements()) {
			if (entitlement == allowed) {
				foundEntitlement = true;
			}
		}
		if (!foundEntitlement) continue;
		// This license matches our target. Decrement the counter.
		license.setCurrentContexts(license.getCurrentContexts() - 1);
		// If no more copies are reserved, unreserve the lisence to the server.
		if (license.getCurrentContexts() == 0) {
			client->unreserveEntitlement(license.getId(), entitlement);
			current.erase(current.begin() + i);
		}
		return true;
	}

	std::cout << "Warning: Attempted to unreserve entitlement " << entitlement << " in context " << context << ", but it was not reserved." << std::endl;
	return false;
}

void LicensingAPIImplementation::check(std::string entitlement) {
	std::cout << "ATTEMPTED TO USE PROTECTED FUNCTION WITH NO RESERVED LICENSE" << std::endl;
	std::cout << "Using this function requires that the " << entitlement << " entitlement is reserved!" << std::endl;
	std::cout << "Program terminating." << std::endl;
	exit(1);
}

void LicensingAPIImplementation::shutdown() {
	std::cout << "Licensing service has been shut down" << std::endl;
	running = false;
}