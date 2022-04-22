#pragma once

#include <string>
#include <boost/optional.hpp>
#include "LicensingDLL.h"
#include "LicenseReserveFailure.h"

class dll LicenseClient {
public:
	virtual ~LicenseClient() {}
	/**
	* Attempt to reserve an entitlement for a specific license ID from the
	* remote license server. Return empty if successfull, or the associated
	* error otherwise.
	*/
	virtual boost::optional<LicenseReserveFailure> reserveEntitlement(std::string license, std::string entitlement) = 0;
	/**
	* Unreserve the given entitlement for a specific license ID on the
	* remote license server. This method does nothing if the specified
	* entitlement is not reserved.
	*/
	virtual void unreserveEntitlement(std::string license, std::string entitlement) = 0;
};